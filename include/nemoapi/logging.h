#ifndef __nemoapi__logging_h
#define __nemoapi__logging_h
#include <iostream>
#include <sstream>

struct logger {
    std::stringstream s;
    logger() {}
    ~logger() {std::cout << s.str() << std::endl;}
    template<class T>
    logger &operator<<(const T &x) {s << x; return *this;}
};

#define LOG_TRACE logger() << __FILE__ << ":" << __LINE__ << " "
#define LOG_DEBUG logger() << __FILE__ << ":" << __LINE__ << " "
#define LOG_INFO  logger() << __FILE__ << ":" << __LINE__ << " "
#define LOG_WARN  logger() << __FILE__ << ":" << __LINE__ << " "
#define LOG_ERROR logger() << __FILE__ << ":" << __LINE__ << " "
#define LOG_FATAL logger() << __FILE__ << ":" << __LINE__ << " "
#define _assert(expr)  { if (!(expr)) { LOG_FATAL << #expr ;} }

#endif