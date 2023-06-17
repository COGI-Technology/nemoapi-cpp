#ifndef __nemoapi__client_sync_h
#define __nemoapi__client_sync_h

#include "client.h"
#include "utils.h"
#include "dsa.h"

#include <memory>

namespace nemoapi {
namespace rest {

class client_sync: public client {
    public:
        client_sync(const std::string&& endpoint, eddsa* dsa, const std::string&& access_key_id);
        virtual ~client_sync();

    public:
        /**
         * @brief Function to call API
         * 
         * @param resource_path The path of the API resource
         * @param cb The callback function used for the API request
         * @param post_data Optional parameter containing the request post data (default = "")
         * @param timeout Optional parameter specifying the API request timeout (default = 300L)
         * @return bool The response document returned from the API request
        */
        bool make_request(
            const std::string&& resource_path,
            request_cb&& cb,
            const std::string&& post_data = nullptr,
            const std::time_t timeout = 300L
        ) override;
        static int write_func(const void* buffer, size_t size, size_t count, void* ctx);

    private:
        CURL* curl_;
        struct curl_slist* headers_;
};

} //namespace rest
} //namespace nemoapi

#endif