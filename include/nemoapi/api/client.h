#ifndef NEMOAPI_CLIENT_H
#define NEMOAPI_CLIENT_H

#include <curl/curl.h>
#include <cstdint>
#include "../utils.h"

#define BASE_USER_AGENT nemoapi_memory_init("User-Agent: NemoverseAPI/2.0.0/cpp/")
#define BASE_ACCESS_TIME nemoapi_memory_init("Access-Time: ")
#define BASE_ACCESS_SIGNATURE nemoapi_memory_init("Access-Signature: ")
#define BASE_ACCESS_KEY_ID nemoapi_memory_init("Access-Key-Id: ")
#define NEMOAPI_POST CURLOPT_POST
#define NEMOAPI_GET CURLOPT_HTTPGET
#define NEMOAPI_PUT CURLOPT_PUT

using namespace std;

struct APIConfiguration {
    struct nemoapi_memory* host;
    struct nemoapi_memory* key_id;
    struct nemoapi_memory* public_key;
    struct nemoapi_memory* private_key;
};


enum AuthticationType {
    NemoApiV2Auth = 0x01, // 00000001
    NemoOAuth = 0x02, // 00000010
    NemoApiKeyAuth = 0x04, // 00000100
    NemoHttpBearerAuth = 0x08, // 00001000
    NemoHttpBasicAuth = 0x20, // 00100000
};

class APIClient {
    public:
        APIClient(
            APIConfiguration* config,
            CURL* curl = nullptr,
            const struct nemoapi_memory* identification = nullptr
        );
        ~APIClient();
    
    public:
        struct nemoapi_memory* call_api(
            const struct nemoapi_memory* resource_path,
            CURLoption method,
            uint8_t auth_setting,
            const struct curl_slist* header_params = nullptr,
            const struct nemoapi_memory* path_params = nullptr,
            const struct nemoapi_memory* post_parmas = nullptr,
            const struct nemoapi_memory* response_type = nullptr,
            long timeout = 300L
        );
        void set_default_header(const struct nemoapi_memory* header);
        static size_t callback(void *data, size_t size, size_t nmemb, void *clientp);

    private:
        CURL* _curl;
        APIConfiguration* _config;
        struct curl_slist* _headers;
        struct nemoapi_memory** sign(const struct nemoapi_memory* url, const struct nemoapi_memory* body, unsigned long access_time);
        void update_params_for_auth(
            struct nemoapi_memory* url,
            struct nemoapi_memory* method,
            struct curl_slist* headers,
            const struct nemoapi_memory* query_params,
            const struct nemoapi_memory* body,
            uint8_t auth_settings
        );

};

#endif