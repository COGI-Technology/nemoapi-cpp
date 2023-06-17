#include <nemoapi/client_async.h>

namespace nemoapi {
namespace rest {

client_async::client_async(const std::string&& endpoint, eddsa* dsa, const std::string&& access_key_id, nemoapi::uv::http_client_pool_ptr pool)
    : client(endpoint, dsa, access_key_id)
    , pool_(pool)
    , headers_(nullptr)
{
    std::string ua = "User-Agent: " + std::string(DEFAULT_USER_AGENT);
    headers_ = curl_slist_append(headers_, ua.c_str());
}

client_async::~client_async() {
    curl_slist_free_all(headers_);

    headers_ = nullptr;
}

bool client_async::make_request(
    const std::string&& resource_path,
    request_cb&& cb,
    const std::string&& post_data,
    const std::time_t timeout
) {
    v2_signature_t* signature = sign(resource_path, post_data, nemoapi::timestamp());
    std::string&& url = m_endpoint + std::move(resource_path);

    struct curl_slist* headers = nullptr;
    {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_extend(headers, headers_);
        std::string&& _access_key_id = std::string(ACCESS_KEY_ID) + ": " + m_access_key_id;
        headers = curl_slist_append(headers, _access_key_id.c_str());
        std::string&& _access_time = std::string(ACCESS_TIME) + ": " + signature->access_time;
        headers = curl_slist_append(headers, _access_time.c_str());
        std::string&& _access_signature = std::string(ACCESS_SIGNATURE) + ": " + signature->access_signature;
        headers = curl_slist_append(headers, _access_signature.c_str());
    }

    return pool_->make_request(std::move(url), std::move(post_data), [=](const char* res, size_t len){
        delete signature;
        cb(res, nullptr);
    }, [=](const char* err, size_t len){
        if(len >0){//when failed
            delete signature;
            cb(nullptr, err);
        }
    }, headers);
}

}//namespace rest
}//namespace nemoapi