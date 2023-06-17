#ifndef __nemoapi__uv_http_client_h
#define __nemoapi__uv_http_client_h

#include "uv_future.h"
#include <curl/curl.h>
#include <functional>

namespace nemoapi {
namespace uv {

using message_cb = std::function<void(const char*,size_t)>;
using close_cb = std::function<void(const char*,size_t)>;

class http_client
{
    public:
        http_client(
            const std::string&& url,
            const std::string&& post_data,
            message_cb&& callback,
            close_cb&& close_callback,
            uv_loop_t* loop,
            curl_slist* headers
        );
        ~http_client();
        static int socket_func(CURL* easy, curl_socket_t s, int action, void* userp, void* socketp);
        static int timer_func(CURLM* multi, long timeout_ms, void* ctx);
        static size_t write_func(const void* buffer, size_t size, size_t count, void* ctx);
        int on_socket(CURL* easy, curl_socket_t s, int action);
        int on_timer(CURLM* multi, long timeout_ms);
        static void on_timeout(uv_handle_t* req);
        size_t on_write(const void* buffer, size_t size, size_t count);
        static void curl_perform(uv_poll_t* req, int status, int events);
        void check_multi_info();
        static void on_close(uv_handle_t* h);
        void shutdown();

    private:
        std::vector<std::pair<curl_socket_t, uv_poll_t*>> _curl_pool_map;
        uv_loop_t* _loop;
        uv_timer_t _timer;
        uv_async_t _async;
        CURLM* _multi_handle;
        CURL* _handle;
        std::string _url;
        std::string _post_data;
        std::string _response;
        std::string _error;
        curl_slist* _headers;
        message_cb _callback;
        close_cb _close_callback;
};

using http_client_ptr = std::shared_ptr<http_client>;

class http_client_pool {
    public:
        http_client_pool(size_t max_size, uv_loop_t* loop);
        ~http_client_pool();
        bool make_request(
            const std::string&& url,
            const std::string&& post_data,
            message_cb&& callback,
            close_cb&& close_callback,
            curl_slist* headers = nullptr
        );
    
    private:
        future_pool_ptr _pool;
        uv_loop_t* _loop;
};

using http_client_pool_ptr = std::shared_ptr<http_client_pool>;

} //namespace uv
} //namespace nemoapi
#endif