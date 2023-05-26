#include "nemoapi.h"

APIClient::APIClient(
    APIConfiguration* config,
    CURL* curl,
    const struct nemoapi_memory* identification
): _config(nullptr), _curl(nullptr), _headers(nullptr) {
    if (curl == nullptr)
        curl = curl_easy_init();

    auto base_ua = BASE_USER_AGENT;
    struct nemoapi_memory* user_agent = nullptr;
    if (identification == nullptr) {
        auto id = nemoapi_memory_from_str("Unknow");
        user_agent = combine(base_ua, id);
        free(id);
    } else {
        user_agent = combine(base_ua, identification);
    }
        
    _config = config;
    _curl = curl;
    set_default_header(user_agent);

    free(user_agent);
    free(base_ua);
}

APIClient::~APIClient() {
    free(_config->host);
    free(_config->key_id);
    free(_config->public_key);
    free(_config->private_key);
    delete _config;

    curl_slist_free_all(_headers);
    curl_easy_cleanup(_curl);

    _config = nullptr;
    _curl =nullptr;
    _headers = nullptr;
}

rapidjson::Document APIClient::call_api(
    const struct nemoapi_memory* resource_path,
    CURLoption method,
    uint8_t auth_setting,
    const struct curl_slist* header_params,
    const struct nemoapi_memory* path_params,
    const struct nemoapi_memory* post_parmas,
    long timeout
) {
    struct nemoapi_memory* url = combine(_config->host, resource_path);
    if (path_params != nullptr) {
        struct nemoapi_memory* tmp = combine(url, path_params);
        free(url);
        url = tmp;
    }

    printf("url: %s\n", reinterpret_cast<char*>(url->data));

    curl_easy_setopt(_curl, CURLOPT_URL, url->data);
    curl_easy_setopt(_curl, method, 1L);

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_extend(headers, header_params);
    headers = curl_slist_extend(headers, _headers);
    
    update_params_for_auth(url, nullptr, headers, path_params, post_parmas, auth_setting);
    curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, headers);

    if (post_parmas != nullptr) {
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, reinterpret_cast<char*>(post_parmas->data));
    }
    
    struct nemoapi_memory* chunk = new nemoapi_memory{nullptr, 0};

    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, chunk);

    CURLcode res = curl_easy_perform(_curl);
    if (res != CURLE_OK) {
        free(url);
        curl_slist_free_all(headers);
        curl_easy_reset(_curl);
        free(chunk);
        throw runtime_error(curl_easy_strerror(res));
    }

    printf("response: %s\n", reinterpret_cast<char*>(chunk->data));

    rapidjson::Document response;
    response.Parse(reinterpret_cast<char*>(chunk->data));

    bool missing_params = !response.HasMember("params");
    if (!response.HasMember("status")
        || response["status"].GetInt() < 0
        || (missing_params && !response.HasMember("uuid"))) {
        throw runtime_error(reinterpret_cast<char*>(chunk->data));
    }
    
    if (!missing_params && response["params"].IsObject()) {
        auto response_params = response["params"].GetObject();
        if (response_params.HasMember("error")) {
            free(url);
            curl_slist_free_all(headers);
            curl_easy_reset(_curl);
            free(chunk);
            throw runtime_error(response_params["error"].GetString());
        }
        if (response_params.HasMember("err")) {
            free(url);
            curl_slist_free_all(headers);
            curl_easy_reset(_curl);
            free(chunk);
            throw runtime_error(response_params["err"].GetString());
        }
    }

    free(url);
    curl_slist_free_all(headers);
    curl_easy_reset(_curl);
    free(chunk);
    return response;
}

void APIClient::set_default_header(const struct nemoapi_memory* header) {
    _headers = curl_slist_append(_headers, reinterpret_cast<char*>(header->data));
}

size_t APIClient::callback(void *data, size_t size, size_t nmemb, void *clientp) {
    size_t realsize = size * nmemb;
    struct nemoapi_memory *mem = (struct nemoapi_memory *)clientp;
    
    uint8_t* ptr = (uint8_t*)realloc(mem->data, mem->length + realsize + 1);
    if(ptr == NULL)
        return 0;  /* out of memory! */
    
    mem->data = ptr;
    memcpy(&(mem->data[mem->length]), data, realsize);
    mem->length += realsize;
    mem->data[mem->length] = 0;
    
    return realsize;
}

void APIClient::update_params_for_auth(
    struct nemoapi_memory* url,
    struct nemoapi_memory* method,
    struct curl_slist* headers,
    const struct nemoapi_memory* query_params,
    const struct nemoapi_memory* body,
    uint8_t auth_settings
) {
    if (auth_settings & NemoApiV2Auth) {
        auto millisec_since_epoch = chrono::duration_cast<chrono::milliseconds>
            (chrono::system_clock::now().time_since_epoch()).count();
        // auto millisec_since_epoch = 123456;
        struct APIV2Signed* res = sign(url, body, millisec_since_epoch);
        headers = curl_slist_append(headers, reinterpret_cast<char*>(res->access_key_id->data));
        free(res->access_key_id);
        headers = curl_slist_append(headers, reinterpret_cast<char*>(res->access_signature->data));
        free(res->access_signature);
        headers = curl_slist_append(headers, reinterpret_cast<char*>(res->access_time->data));
        free(res->access_time);
        delete res;
    }
}

struct APIV2Signed* APIClient::sign(
    const struct nemoapi_memory* url,
    const struct nemoapi_memory* body,
    unsigned long access_time
) {
    string access_id = urlparse(url).path;
    
    struct APIV2Signed* ret = new APIV2Signed{nullptr, nullptr, nullptr};
    
    if (body == nullptr) {
        body = nemoapi_memory_from_str("{}");
    }

    string access_time_in_str = to_string(access_time);
    auto message_hash_length = access_id.length() + 1 + access_time_in_str.length() + 1 + body->length;
    uint8_t* message_hash = new uint8_t[message_hash_length];

    concat(message_hash, reinterpret_cast<const uint8_t*>(access_id.c_str()), 0, access_id.length());
    message_hash[access_id.length()] = 0x3a; // hex of :
    concat(
        message_hash,
        reinterpret_cast<const uint8_t*>(access_time_in_str.c_str()),
        access_id.length() + 1,
        access_time_in_str.length()
    );
    message_hash[access_id.length() + 1 + access_time_in_str.length()] = 0x3a;
    concat(
        message_hash,
        body->data,
        access_id.length() + 1 + access_time_in_str.length() + 1,
        body->length
    );
    memset(message_hash + message_hash_length, 0, len(message_hash) - message_hash_length);

    struct nemoapi_memory* mh = new nemoapi_memory{nullptr, 0};
    mh->data = message_hash;
    mh->length = message_hash_length;
    
    printf("message hash: %s\n", reinterpret_cast<char*>(mh->data));

    EDDSA* dsa = EDDSA::from_prv(_config->private_key);
    struct nemoapi_memory* access_signature = dsa->sign(mh);
    struct nemoapi_memory* tmp = nemoapi_memory_from_str(base64_encode(access_signature).c_str());
    free(access_signature);
    
    struct nemoapi_memory* base = BASE_ACCESS_SIGNATURE;
    access_signature = combine(base, tmp);
    free(tmp);
    free(base);

    base = BASE_ACCESS_KEY_ID;
    struct nemoapi_memory* access_key_id = combine(base, _config->key_id);
    free(base);

    base = BASE_ACCESS_TIME;
    tmp = nemoapi_memory_from_str(access_time_in_str.c_str());
    struct nemoapi_memory* access_time_im = combine(base, tmp);
    free(tmp);
    free(base);

    ret->access_key_id = access_key_id;
    ret->access_signature = access_signature;
    ret->access_time = access_time_im;

    free(mh);
    delete dsa;

    return ret;
}