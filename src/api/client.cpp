#include "nemoapi.h"

namespace Nemo
{
 APIClient::APIClient(
    const uint8_t* host,
    size_t host_len,
    const uint8_t* key_id,
    size_t key_id_len,
    const uint8_t* public_key,
    size_t public_key_len,
    const uint8_t* private_key,
    size_t private_key_len
):
    host_(new uint8_t[host_len + 1]),
    host_len_(host_len),
    key_id_(new uint8_t[key_id_len + 1]),
    key_id_len_(key_id_len),
    public_key_(new uint8_t[public_key_len + 1]),
    public_key_len_(public_key_len),
    private_key_(new uint8_t[private_key_len + 1]),
    private_key_len_(private_key_len)
{
    memcpy(host_, host, host_len);
    host_[host_len] = '\0';
    memcpy(key_id_, key_id, key_id_len);
    key_id_[key_id_len] = '\0';
    memcpy(public_key_, public_key, public_key_len);
    public_key_[public_key_len] = '\0';
    memcpy(private_key_, private_key, private_key_len);
    private_key_[private_key_len] = '\0';
}

APIClient::~APIClient() {
    delete[] host_;
    delete[] key_id_;
    delete[] private_key_;
    delete[] public_key_;

    host_ = nullptr;
    key_id_ = nullptr;
    private_key_ = nullptr;
    public_key_ = nullptr;
}

struct APIV2Signed* APIClient::sign(
    const uint8_t* resource_path,
    size_t resource_path_len,
    const uint8_t* body,
    size_t body_len,
    unsigned long access_time
) {
    uint8_t* url = combine(host_, host_len_, resource_path, resource_path_len);
    string access_id = urlparse(url).path;
    delete[] url;
    
    string access_time_in_str = to_string(access_time);
    auto message_hash_length = access_id.length() + 1 + access_time_in_str.length() + 1 + body_len;
    uint8_t* message_hash = new uint8_t[message_hash_length + 1];
    size_t cur_offset = 0;

    // message_hash += access_id;
    memcpy(message_hash + cur_offset, access_id.c_str(), access_id.length());
    cur_offset += access_id.length();

    // message_hash += ":";
    message_hash[cur_offset] = COLON_IN_HEX;
    cur_offset += 1;

    // message_hash += access_time;
    memcpy(message_hash + cur_offset, access_time_in_str.c_str(), access_time_in_str.length());
    cur_offset += access_time_in_str.length();

    // message_hash += ":";
    message_hash[cur_offset] = COLON_IN_HEX;
    cur_offset += 1;

    memcpy(message_hash + cur_offset, body, body_len);
    cur_offset += body_len;
    message_hash[cur_offset] = '\0';

    EDDSA* dsa = EDDSA::from_prv(private_key_);

    size_t s_size = S_SIZE;
    uint8_t* s = new uint8_t[s_size + 1];
    dsa->sign(message_hash, message_hash_length, s, &s_size);
    s[s_size] = '\0';
    string sb64 = base64_encode(s, s_size);
    delete dsa;
    delete[] s;
    
    struct APIV2Signed* ret = new APIV2Signed{nullptr, 0, nullptr, 0, nullptr, 0};

    ret->access_key_id = make_unique<uint8_t[]>(key_id_len_ + 1);
    memcpy(ret->access_key_id.get(), key_id_, key_id_len_ + 1);
    ret->access_key_id_len = key_id_len_;

    ret->access_signature = unique_ptr<uint8_t[]>(to_bytes(sb64.c_str(), sb64.length()));
    ret->access_signature_len = s_size;

    ret->access_time = make_unique<uint8_t[]>(access_time_in_str.length() + 1);
    memcpy(ret->access_time.get(), access_time_in_str.c_str(), access_time_in_str.length() + 1);
    ret->access_time_len = access_time_in_str.length();

    return ret;
}

CURLClient::CURLClient(
    const uint8_t* host,
    size_t host_len,
    const uint8_t* key_id,
    size_t key_id_len,
    const uint8_t* public_key,
    size_t public_key_len,
    const uint8_t* private_key,
    size_t private_key_len,
    CURL* curl,
    const uint8_t* identification,
    size_t identification_len
):
    APIClient(host, host_len, key_id, key_id_len, public_key, public_key_len, private_key, private_key_len),
    curl_(curl),
    headers_(nullptr)
{
    if (curl == nullptr)
        curl_ = curl_easy_init();

    uint8_t* _an = to_bytes(ANONYMOUS_USER_AGENT, ANONYMOUS_USER_AGENT_SIZE);
    if (identification == nullptr || identification_len <= 0) {
        identification = _an;
        identification_len = ANONYMOUS_USER_AGENT_SIZE;
    }

    string _ba = string("User-Agent: ") + BASE_USER_AENT_NEMO_NAMESPACE;
    size_t _id_len = _ba.length() + identification_len;
    uint8_t* _id = new uint8_t[_id_len + 1];
    memcpy(_id, _ba.c_str(), _ba.length());
    memcpy(_id + _ba.length(), identification, identification_len);
    _id[_id_len] = '\0';

    set_default_header(_id);

    delete[] _an;
    delete[] _id;
}

CURLClient::~CURLClient() {
    curl_slist_free_all(headers_);
    curl_easy_cleanup(curl_);

    headers_ = nullptr;
    curl_ = nullptr;
}

void CURLClient::set_default_header(const uint8_t* header) {
    headers_ = curl_slist_append(headers_, reinterpret_cast<const char*>(header));
}

size_t CURLClient::callback(void *data, size_t size, size_t nmemb, void *clientp) {
    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory *)clientp;
    
    uint8_t *ptr = (uint8_t*)realloc(mem->data, mem->size + realsize + 1);
    if(ptr == NULL)
        return 0;  /* out of memory! */
    
    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), data, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;
    
    return realsize;
}

void CURLClient::update_params_for_auth(
    uint8_t* url,
    size_t url_len,
    struct curl_slist* headers,
    uint8_t auth_settings,
    const Nemo::APIV2Signed* signature
) {
    if (auth_settings & NemoApiV2Auth) {
        _assert(signature != nullptr, "invalid signature");

        string access_key_id =
            string(ACCESS_KEY_ID) + string(": ") + reinterpret_cast<char*>(signature->access_key_id.get());
        headers = curl_slist_append(headers, access_key_id.c_str());
        string access_time =
            string(ACCESS_TIME) + string(": ") + reinterpret_cast<char*>(signature->access_time.get());
        headers = curl_slist_append(headers, access_time.c_str());
        string access_signature =
            string(ACCESS_SIGNATURE) + string(": ") + reinterpret_cast<char*>(signature->access_signature.get());
        headers = curl_slist_append(headers, access_signature.c_str());
    }
}


rapidjson::Document CURLClient::call_api(
    const uint8_t* resource_path,
    size_t resource_path_len,
    int method,
    uint8_t auth_setting,
    const uint8_t* body_data,
    size_t body_len,
    const Nemo::APIV2Signed* signature,
    long timeout,
    void* argv[],
    size_t argc
) {
    auto _method = static_cast<CURLoption>(method);

    auto _url_len = host_len_ + resource_path_len;
    uint8_t* url = combine(host_, host_len_, resource_path, resource_path_len);
    bool url_use_realloc = false;
    if (argc > 1 && argv[0] != nullptr) {
        size_t _pp_len = *static_cast<size_t*>(argv[1]);
        if (_pp_len > 0) {
            url = (uint8_t*)realloc(url, _url_len + _pp_len + 1);
            uint8_t* _path_params = static_cast<uint8_t*>(argv[0]);
            memcpy(url + _url_len, _path_params, _pp_len);
            _url_len += _pp_len;
            url[_url_len] = '\0';
            url_use_realloc = true;
        }
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    if (argc > 2 && argv[2] != nullptr) {
        struct curl_slist* header_params = static_cast<struct curl_slist*>(argv[2]);
        headers = curl_slist_extend(headers, header_params);
    }
    headers = curl_slist_extend(headers, headers_);
    
    update_params_for_auth(url, _url_len, headers, auth_setting, signature);

    curl_easy_reset(curl_);
    curl_easy_setopt(curl_, CURLOPT_URL, url);
    if (url_use_realloc) free(url);
    else delete[] url;
    curl_easy_setopt(curl_, _method, 1L);
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

    if (body_data == nullptr || body_len <= 0) {
        curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, "{}");
    } else {
        curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, body_data);
    }
    
    struct memory chunk = {nullptr, 0};

    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (void*)&chunk);

    CURLcode res = curl_easy_perform(curl_);
    if (res != CURLE_OK) {
        if (chunk.data != nullptr) {
            free(chunk.data);
        }
        curl_slist_free_all(headers);
        throw runtime_error(curl_easy_strerror(res));
    }

    uint8_t* _res = new uint8_t[chunk.size + 1];
    if (chunk.data != nullptr) {
        memcpy(_res, chunk.data, chunk.size);
        _res[chunk.size] = '\0';
        free(chunk.data);
    }

    rapidjson::Document response;
    response.Parse(reinterpret_cast<char*>(_res));    

    bool missing_params = !response.HasMember("params");
    if (!response.HasMember("status")
        || response["status"].GetInt() < 0
        || (missing_params && !response.HasMember("uuid"))) {
        curl_slist_free_all(headers);
        auto err = runtime_error(reinterpret_cast<char*>(_res));
        delete[] _res;
        throw err;
    }
    delete[] _res;
    
    if (!missing_params && response["params"].IsObject()) {
        auto response_params = response["params"].GetObject();
        if (response_params.HasMember("error")) {
            curl_slist_free_all(headers);
            throw runtime_error(response_params["error"].GetString());
        }
        if (response_params.HasMember("err")) {
            curl_slist_free_all(headers);
            throw runtime_error(response_params["err"].GetString());
        }
    }
    curl_slist_free_all(headers);
    return response;
}
} // namespace Nemo
