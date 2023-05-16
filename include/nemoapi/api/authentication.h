#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <curl/curl.h>
#include <cstdint>

#define K_BASIC_AUTH_SIZE 21 // length of "Authorization: Basic "

using namespace std;
class Authentication {
    public:
        virtual int append_to_headers(curl_slist** headers) = 0;
};

class HttpBasicAuth: public Authentication {
    public:
        HttpBasicAuth(const unsigned char* username, const unsigned char* password);
        ~HttpBasicAuth();
        string get_auth();
        int append_to_headers(curl_slist** headers) override;

    private:
        uint8_t* _username;
        uint8_t* _password;
};

class NemoApiV2Auth: public Authentication {
    public:
        NemoApiV2Auth(const unsigned char* key_id, const unsigned char* username, const unsigned char* password);
        ~NemoApiV2Auth();
        int append_to_headers(curl_slist** headers) override;

    private:
        uint8_t* key_id;
        uint8_t* public_key;
        uint8_t* private_key;
};

#endif