#ifndef __nemoapi__uv_future_h
#define __nemoapi__uv_future_h

#include <uv.h>
#include <mutex>
#include <vector>
#include <memory>
#include <map>
#include <functional>

namespace nemoapi {
namespace uv {

using coro_f = std::function<void()>;
using done_cb = std::function<void()>;

class future {
	public:
		future(uv_loop_t* loop, coro_f&& coro);
		~future();
		static void async_cb(uv_async_t* handle);
		void add_done_callback(done_cb&& cb);
	private:
		uv_loop_t* _loop;		
		std::vector<done_cb> _done_callbacks;
	public:
		uv_async_t* m_async;
		coro_f m_coro;
};
using future_ptr = std::shared_ptr<future>;

class future_pool {
	public:
		future_pool(size_t max_size, uv_loop_t* loop);
		~future_pool();
		size_t size(){ return _running.size(); };
		bool is_empty() { return size() == 0; };
		bool is_full() { return size() >= _max_size; };
		bool spawn(std::string&& name, coro_f&& coro, done_cb&& cb);
		bool spawn_nowait(std::string&& name, coro_f&& coro, done_cb&& cb);

	private:
		uv_loop_t* _loop;
		size_t _max_size;
		std::map<std::string, future*> _running;
		std::mutex _mutex;
	private:
		bool _spawn(std::string&& name, coro_f&& coro, done_cb&& cb);
};
using future_pool_ptr = std::shared_ptr<future_pool>;

} //namespace uv
} //namespace nemoapi

#endif