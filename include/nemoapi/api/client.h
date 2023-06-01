#ifndef NEMOAPI_CLIENT_H
#define NEMOAPI_CLIENT_H

#include "../utils.h"
#include "../dsa.h"

#define NEMOAPI_POST CURLOPT_POST
#define NEMOAPI_GET CURLOPT_HTTPGET
#define NEMOAPI_PUT CURLOPT_PUT
#define COLON_IN_HEX 0x3a

#define BASE_USER_AENT_NEMO_NAMESPACE "NemoverseAPI/2.0.0/cpp/"
#define BASE_USER_AENT_LEN_NEMO_NAMESPACE 23
#define ACCESS_TIME "Access-Time"
#define ACCESS_SIGNATURE "Access-Signature"
#define ACCESS_KEY_ID "Access-Key-Id"
#define ANONYMOUS_USER_AGENT "Unkow"
#define ANONYMOUS_USER_AGENT_SIZE 6

using namespace std;

namespace Nemo
{


struct APIV2Signed {
    std::unique_ptr<uint8_t[]> access_time;
    size_t access_time_len;
    std::unique_ptr<uint8_t[]> access_key_id;
    size_t access_key_id_len;
    std::unique_ptr<uint8_t[]> access_signature;
    size_t access_signature_len;
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
            const uint8_t* host,
            size_t host_len,
            const uint8_t* key_id,
            size_t key_id_len,
            const uint8_t* public_key,
            size_t public_key_len,
            const uint8_t* private_key,
            size_t private_key_len
        );
        virtual ~APIClient();

    public:
        virtual rapidjson::Document call_api(
            const uint8_t* resource_path,
            size_t resource_path_len,
            int method,
            uint8_t auth_setting,
            const uint8_t* body_data = nullptr,
            size_t body_len = 0,
            const Nemo::APIV2Signed* signature = nullptr,
            long timeout = 300L,
            void* argv[] = nullptr,
            size_t argc = 0
        )=0;

    public:
        struct Nemo::APIV2Signed* sign(
            const uint8_t* resource_path,
            size_t resource_path_len,
            const uint8_t* body,
            size_t body_len,
            unsigned long access_time
        );

    protected:
        uint8_t* host_;
        size_t host_len_;

        uint8_t* key_id_;
        size_t key_id_len_;

        uint8_t* public_key_;
        size_t public_key_len_;

        uint8_t* private_key_;
        size_t private_key_len_;
};

struct memory {
    uint8_t* data;
    size_t size;
};

class CURLClient: public APIClient {
    public:
        CURLClient(
            const uint8_t* host,
            size_t host_len,
            const uint8_t* key_id,
            size_t key_id_len,
            const uint8_t* public_key,
            size_t public_key_len,
            const uint8_t* private_key,
            size_t private_key_len,
            CURL* curl = nullptr,
            const uint8_t* identification = nullptr,
            size_t identification_len = 0
        );
        virtual ~CURLClient();

    public:
        /**
         * @brief Function to call API
         * 
         * @param resource_path The path of the API resource
         * @param resource_path_len The length of the API resource path
         * @param method The HTTP method to use for the API request
         * @param auth_setting The authentication setting used for the API request
         * @param body_data Optional parameter containing the request body data (default = nullptr)
         * @param signature Optional parameter containing the API signature (default = nullptr)
         * @param timeout Optional parameter specifying the API request timeout (default = 300L)
         * @param argv Optional array of arguments (default = nullptr)
         * @param argc The number of arguments in argv (default = 0)
         *      - argv[0]: A void pointer to the query params
         *      - argv[1]: The length of the query params
         *      - argv[2]: A void pointer to struct curl_slist representing the header
         * @return rapidjson::Document The response document returned from the API request
         */
        rapidjson::Document call_api(
            const uint8_t* resource_path,
            size_t resource_path_len,
            int method,
            uint8_t auth_setting,
            const uint8_t* body_data = nullptr,
            size_t body_len = 0,
            const Nemo::APIV2Signed* signature = nullptr,
            long timeout = 300L,
            void* argv[] = nullptr,
            size_t argc = 0
        ) override;
        void set_default_header(const uint8_t* header);
        static size_t callback(void *data, size_t size, size_t nmemb, void *clientp);

    private:
        CURL* curl_;
        struct curl_slist* headers_;
        void update_params_for_auth(
            uint8_t* url,
            size_t url_len,
            struct curl_slist* headers,
            uint8_t auth_settings,
            const Nemo::APIV2Signed* signature= nullptr
        );
};

} // namespace Nemo

#endif