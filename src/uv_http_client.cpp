#include <nemoapi/logging.h>
#include <nemoapi/uv_http_client.h>

#include <algorithm>
#include <sstream>
#include <chrono>
#include <cstring>

namespace nemoapi {
namespace uv {

http_client::http_client(
	const std::string&& url,
	const std::string&& post_data,
	message_cb&& callback,
	close_cb&& close_callback,
	uv_loop_t* loop,
	curl_slist* headers
)
	: _loop(loop)
	, _timer{}
	, _async{}
	, _multi_handle(nullptr)
	, _handle(nullptr)
	, _url(url)
	, _post_data(post_data)
	, _headers(headers)
	, _callback(callback)
	, _close_callback(close_callback)
{
	const int timeout = 10;
	_curl_pool_map.reserve(2);

	int err = uv_timer_init(_loop, &_timer);
	if (err) {
		std::string _err = "uv_timer_init failed, error" + std::string(uv_err_name(err));
		LOG_FATAL << _err;
		_close_callback(_err.c_str(), _err.length());
		return;
	}
	_timer.data = this;

	err = uv_async_init(_loop, &_async, reinterpret_cast<uv_async_cb>(on_timeout));
	if (err) {
		uv_close(reinterpret_cast<uv_handle_t*>(&_timer), nullptr);
		std::string _err = "uv_async_init failed, error " + std::string(uv_err_name(err));
		LOG_FATAL << _err;
		_close_callback(_err.c_str(), _err.length());
		return;
	}
	_async.data = this;

	_multi_handle = curl_multi_init();
	if (!_multi_handle) {
		uv_close(reinterpret_cast<uv_handle_t*>(&_async), nullptr);
		uv_close(reinterpret_cast<uv_handle_t*>(&_timer), nullptr);
		std::string _err = "curl_multi_init() failed";
		LOG_FATAL << _err;
		_close_callback(_err.c_str(), _err.length());
		return;
	}

	curl_multi_setopt(_multi_handle, CURLMOPT_SOCKETFUNCTION, socket_func);
	curl_multi_setopt(_multi_handle, CURLMOPT_SOCKETDATA, this);
	
	curl_multi_setopt(_multi_handle, CURLMOPT_TIMERFUNCTION, timer_func);
	curl_multi_setopt(_multi_handle, CURLMOPT_TIMERDATA, this);

	_handle = curl_easy_init();
	if (!_handle) {
		curl_multi_cleanup(_multi_handle);
		uv_close(reinterpret_cast<uv_handle_t*>(&_async), nullptr);
		uv_close(reinterpret_cast<uv_handle_t*>(&_timer), nullptr);
		std::string _err = "curl_easy_init() failed";
		LOG_FATAL << _err;
		_close_callback(_err.c_str(), _err.length());
		return;
	}

	curl_easy_setopt(_handle, CURLOPT_WRITEFUNCTION, write_func);
	curl_easy_setopt(_handle, CURLOPT_WRITEDATA, this);

    curl_easy_setopt(_handle, CURLOPT_URL, _url.c_str());
    if (_post_data.c_str() != nullptr) {
        curl_easy_setopt(_handle, CURLOPT_HTTPPOST, 1L);
        curl_easy_setopt(_handle, CURLOPT_POSTFIELDS, _post_data.c_str());
    }

	curl_easy_setopt(_handle, CURLOPT_CONNECTTIMEOUT, timeout);
	curl_easy_setopt(_handle, CURLOPT_TIMEOUT, timeout * 10);

    _headers = curl_slist_append(_headers, "Content-Type: application/json");
	if (_headers) {
		curl_easy_setopt(_handle, CURLOPT_HTTPHEADER, _headers);
	}

	CURLMcode curl_err = curl_multi_add_handle(_multi_handle, _handle);
	if (curl_err != CURLM_OK) {
		curl_easy_cleanup(_handle);
		curl_multi_cleanup(_multi_handle);
		uv_close(reinterpret_cast<uv_handle_t*>(&_async), nullptr);
		uv_close(reinterpret_cast<uv_handle_t*>(&_timer), nullptr);
		std::string _err = "curl_multi_add_handle failed, error " + std::string(curl_multi_strerror(curl_err));
		LOG_FATAL << _err;
		_close_callback(_err.c_str(), _err.length());
		return;
	}
}

http_client::~http_client()
{
	if (_error.empty() && !_response.empty()) {
		//_response.insert(_response.end(), '\0', 1);
		_callback(_response.c_str(), _response.size());
	}

	if (_response.empty()) {
		if (_error.empty()) {
			_error = "empty response";
		}
	}

	_close_callback(_error.c_str(), _error.length());

	curl_slist_free_all(_headers);
}

int http_client::on_socket(CURL* easy, curl_socket_t s, int action)
{
	auto it = std::find_if(_curl_pool_map.begin(), _curl_pool_map.end(), [s](const std::pair<curl_socket_t, uv_poll_t*>& v) { return v.first == s; });

	switch (action) {
	case CURL_POLL_IN:
	case CURL_POLL_OUT:
	case CURL_POLL_INOUT:
		{
			uv_poll_t* h = nullptr;

			if (it != _curl_pool_map.end()) {
				h = it->second;
			}
			else {
				h = new uv_poll_t{};

				// cppcheck-suppress nullPointer
				h->data = this;

				const int result = uv_poll_init_socket(_loop, h, s);
				if (result < 0) {
                    LOG_ERROR << "uv_poll_init_socket failed: " << uv_err_name(result);			
					delete h;
					h = nullptr;
				}
				else {
					_curl_pool_map.emplace_back(s, h);
				}
			}

			if (h) {
				const CURLMcode err = curl_multi_assign(_multi_handle, s, this);
				if (err != CURLM_OK) {
                    LOG_ERROR << "curl_multi_assign(action = " << action << ") failed: " << curl_multi_strerror(err);
				}

				int events = 0;
				if (action != CURL_POLL_IN)  events |= UV_WRITABLE;
				if (action != CURL_POLL_OUT) events |= UV_READABLE;

				const int result = uv_poll_start(h, events, curl_perform);
				if (result < 0) {
                    LOG_ERROR << "uv_poll_start failed with error " << uv_err_name(result);
				}
			}
			else {
                LOG_ERROR << "failed to start polling on socket " << static_cast<int>(s);
			}
		}
		break;

	case CURL_POLL_REMOVE:
	default:
		{
			if (it != _curl_pool_map.end()) {
				uv_poll_t* h = it->second;
				_curl_pool_map.erase(it);

				uv_poll_stop(h);
				uv_close(reinterpret_cast<uv_handle_t*>(h), [](uv_handle_t* h) { delete reinterpret_cast<uv_poll_t*>(h); });
			}

			const CURLMcode err = curl_multi_assign(_multi_handle, s, nullptr);
			if (err != CURLM_OK) {
                LOG_ERROR << "curl_multi_assign(action = " << action << ") failed: " << curl_multi_strerror(err);
			}
		}
		break;
	}

	return 0;
}

int http_client::on_timer(CURLM* multi, long timeout_ms)
{
	if (timeout_ms < 0) {
		uv_timer_stop(&_timer);
		return 0;
	}

	if ((timeout_ms == 0) && !uv_is_closing(reinterpret_cast<uv_handle_t*>(&_async))) {
		// 0 ms timeout, but we can't just call on_timeout() here - we have to kick the UV loop
		const int result = uv_async_send(&_async);
		if (result < 0) {
            LOG_ERROR << "uv_async_send failed with error " << uv_err_name(result);
			// if async call didn't work, try to use the timer with 1 ms timeout
			timeout_ms = 1;
		}
		else {
			return 0;
		}
	}

	const int result = uv_timer_start(&_timer, reinterpret_cast<uv_timer_cb>(on_timeout), timeout_ms, 0);
	if (result < 0) {
        LOG_ERROR << "uv_timer_start failed with error " << uv_err_name(result);
		return -1;
	}

	return 0;
}

int http_client::socket_func(CURL* easy, curl_socket_t s, int action, void* userp, void* socketp)
{
    http_client* ctx = reinterpret_cast<http_client*>(socketp ? socketp : userp);
    return ctx->on_socket(easy, s, action);
}

int http_client::timer_func(CURLM* multi, long timeout_ms, void* ctx)
{
    return reinterpret_cast<http_client*>(ctx)->on_timer(multi, timeout_ms);
}

size_t http_client::write_func(const void* buffer, size_t size, size_t count, void* ctx)
{
    return reinterpret_cast<http_client*>(ctx)->on_write(buffer, size, count);
}

void http_client::on_timeout(uv_handle_t* req)
{
	http_client* ctx = reinterpret_cast<http_client*>(req->data);

	int running_handles = 0;
	const CURLMcode err = curl_multi_socket_action(ctx->_multi_handle, CURL_SOCKET_TIMEOUT, 0, &running_handles);
	if (err != CURLM_OK) {
        LOG_ERROR << "curl_multi_socket_action failed, error " << curl_multi_strerror(err);
	}

	ctx->check_multi_info();

	if (running_handles == 0) {
		ctx->shutdown();
	}
}

size_t http_client::on_write(const void* buffer, size_t size, size_t count)
{
	const size_t realsize = size * count;
	const char* p = reinterpret_cast<const char*>(buffer);
	_response.insert(_response.end(), p, p + realsize);
	return realsize;
}

void http_client::curl_perform(uv_poll_t* req, int status, int events)
{
	int flags = 0;
	if (status < 0) {
		flags |= CURL_CSELECT_ERR;
        LOG_ERROR << "uv_poll_start returned error " << uv_err_name(status);
	}
	else {
		if (events & UV_READABLE) flags |= CURL_CSELECT_IN;
		if (events & UV_WRITABLE) flags |= CURL_CSELECT_OUT;
	}

	http_client* ctx = reinterpret_cast<http_client*>(req->data);

	int running_handles = 0;
	auto it = std::find_if(ctx->_curl_pool_map.begin(), ctx->_curl_pool_map.end(), [req](const std::pair<curl_socket_t, uv_poll_t*>& v) { return v.second == req; });
	if (it != ctx->_curl_pool_map.end()) {
		const CURLMcode err = curl_multi_socket_action(ctx->_multi_handle, it->first, flags, &running_handles);
		if (err != CURLM_OK) {
        	LOG_ERROR << "curl_multi_socket_action failed, error " << curl_multi_strerror(err);
		}
	}

	ctx->check_multi_info();

	if (running_handles == 0) {
		ctx->shutdown();
	}
}

void http_client::check_multi_info()
{
	int pending;
	while (CURLMsg* message = curl_multi_info_read(_multi_handle, &pending)) {
		if (message->msg == CURLMSG_DONE) {
			if (message->data.result != CURLE_OK) {
				_error = curl_easy_strerror(message->data.result);
			}
			else {
				long http_code = 0;
				curl_easy_getinfo(message->easy_handle, CURLINFO_RESPONSE_CODE, &http_code);

				if (http_code != 200) {
					std::stringstream s;
					s << "HTTP error " << static_cast<int>(http_code) << '\0';
					_error = s.str();
				}
				else if (_response.empty()) {
					_error = "empty response";
				}
			}

			curl_multi_remove_handle(_multi_handle, _handle);
			curl_easy_cleanup(_handle);
			curl_multi_cleanup(_multi_handle);
			return;
		}
	}
}

void http_client::on_close(uv_handle_t* h)
{
	http_client* ctx = reinterpret_cast<http_client*>(h->data);
	h->data = nullptr;

	if (ctx->_timer.data || ctx->_async.data) {
		return;
	}
	delete ctx;
}

void http_client::shutdown()
{
	for (const auto& p : _curl_pool_map) {
		uv_poll_stop(p.second);
		uv_close(reinterpret_cast<uv_handle_t*>(p.second), [](uv_handle_t* h) { delete reinterpret_cast<uv_poll_t*>(h); });
	}
	_curl_pool_map.clear();

	if (_async.data && !uv_is_closing(reinterpret_cast<uv_handle_t*>(&_async))) {
		uv_close(reinterpret_cast<uv_handle_t*>(&_async), on_close);
	}

	if (_timer.data && !uv_is_closing(reinterpret_cast<uv_handle_t*>(&_timer))) {
		uv_close(reinterpret_cast<uv_handle_t*>(&_timer), on_close);
	}
}

http_client_pool::http_client_pool(size_t max_size, uv_loop_t* loop):_pool(nullptr){
	_loop = loop;
	_pool = future_pool_ptr(new future_pool(max_size, loop));
}

http_client_pool::~http_client_pool(){}

bool http_client_pool::make_request(
	const std::string&& url,
	const std::string&& post_data,
	message_cb&& callback,
	close_cb&& close_callback,
	curl_slist* headers
){
	std::string&& n = std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
	return _pool->spawn(std::move(n), [=](){
		new http_client(
			std::move(url),
			std::move(post_data),
			message_cb{std::move(callback)},
			close_cb{std::move(close_callback)},
			_loop,
			headers
		);
	}, nullptr);
}

} //namespace rest
} //namespace nemoapi