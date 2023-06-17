#include <nemoapi/client_sync.h>

namespace nemoapi {
namespace rest {

client_sync::client_sync(const std::string&& endpoint, eddsa* dsa, const std::string&& access_key_id)
    : client(endpoint, dsa, access_key_id)
    , curl_(nullptr)
    , headers_(nullptr)
{
    curl_ = curl_easy_init();
    std::string ua = "User-Agent: " + std::string(DEFAULT_USER_AGENT);
    headers_ = curl_slist_append(headers_, ua.c_str());
}

client_sync::~client_sync() {
    curl_slist_free_all(headers_);
    curl_easy_cleanup(curl_);

    headers_ = nullptr;
    curl_ = nullptr;
}

int client_sync::write_func(const void* buffer, size_t size, size_t count, void* ctx){
    auto response = reinterpret_cast<std::vector<char>*>(ctx);
    const size_t realsize = size * count;
	const char* p = reinterpret_cast<const char*>(buffer);
	response->insert(response->end(), p, p + realsize);
	return realsize;
}

bool client_sync::make_request(
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

    curl_easy_reset(curl_);
    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

    if (post_data.c_str() != nullptr) {
        curl_easy_setopt(curl_, CURLOPT_HTTPPOST, 1L);
        curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, post_data.c_str());
    }
    
    std::vector<char> response;
    std::string err;

    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_func);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (void*)&response);

    CURLcode done = curl_easy_perform(curl_);
    if (done != CURLE_OK) {
        if (response.size() != 0) {
            response.clear();
        }
        err = curl_easy_strerror(done);
    }else{
        if (response.empty()) {
            err = "empty response";
        }
    }

    delete signature;
    curl_slist_free_all(headers);

    if(err.empty()){
        cb(std::string(response.begin(), response.end()).c_str(), nullptr);
    }else{
        cb(nullptr, err.c_str());
    }
    return true;
}

} //namesapce rest
} //namespace nemoapi
