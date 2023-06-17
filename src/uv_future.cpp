#include <nemoapi/logging.h>
#include <nemoapi/uv_future.h>

namespace nemoapi {
namespace uv {

future::future(uv_loop_t* loop, coro_f&& coro)
	: _loop(loop)
    , _done_callbacks{}
	, m_async(new uv_async_t{})
	, m_coro(coro)
{
	const int err = uv_async_init(_loop, m_async, async_cb);
	if(err != 0){
		LOG_FATAL << "uv_async_init failed, error " << uv_err_name(err);
		return;
	}
	m_async->data = this;
	_done_callbacks.reserve(2);
}

future::~future()
{
	uv_close(reinterpret_cast<uv_handle_t*>(m_async), [](uv_handle_t* handle) {
		delete reinterpret_cast<uv_async_t*>(handle);
	});
};

void future::async_cb(uv_async_t* handle)
{
	future* fut = reinterpret_cast<future*>(handle->data);
	fut->m_coro();
	
	for (done_cb cb : fut->_done_callbacks) {
		cb();
	}
}

void future::add_done_callback(done_cb&& cb){
	_done_callbacks.push_back(std::move(cb));
}

future_pool::future_pool(size_t max_size, uv_loop_t* loop)
	: _loop(loop)
	, _max_size(max_size)
	, _running{}
{}

future_pool::~future_pool(){
	for (auto it = _running.begin(); it != _running.end();) {
		delete it->second;
		it = _running.erase(it);
	}
}

bool future_pool::spawn(std::string&& name, coro_f&& coro, done_cb&& cb){
	if(is_full()) {
		return false;
	}
    bool ret = false;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        auto it = _running.find(name);
        if (it != _running.end()) {
            LOG_ERROR << "exist \"" << name << "\" failed";		
            return false;
        }

        ret = _spawn(std::move(name), std::move(coro), std::move(cb));
    }
	return ret;
}

bool future_pool::spawn_nowait(std::string&& name, coro_f&& coro, done_cb&& cb){
	if(is_full()) {
		LOG_ERROR << "over max_size:" << _max_size;
		return false;
	}

	auto it = _running.find(name);
	if (it != _running.end()) {
		LOG_ERROR << "already:" << name;
		return false;
	}

	return _spawn(std::move(name), std::move(coro), std::move(cb));
}

bool future_pool::_spawn(std::string&& name, coro_f&& coro, done_cb&& cb){
	future* fut = new future(_loop, std::move(coro));
	_running.insert({name, fut});
	if(cb != nullptr){
		fut->add_done_callback(std::move(coro));
	}
	fut->add_done_callback([=](){
		_running.erase(name);
		delete fut;
	});
	const int done = uv_async_send(fut->m_async);
	if (done < 0) { //on failure, so cleanup
		LOG_ERROR << "uv_async_send failed, error " << uv_err_name(done);		
		_running.erase(name);
		delete fut;
		return false;
	}
	return true;
}

} //namespace rest
} //namespace nemoapi