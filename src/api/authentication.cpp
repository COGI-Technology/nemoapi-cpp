// #include "nemoapi.h"

// HttpBasicAuth::HttpBasicAuth(const unsigned char* username, const unsigned char* password) {
//     auto username_len = strlen((const char*)username);
//     auto password_len = strlen((const char*)password);

//     _username = new uint8_t[username_len];
//     _password = new uint8_t[password_len];

//     memcpy(_username, username, username_len);
//     memcpy(_password, password, password_len);
// }

// HttpBasicAuth::~HttpBasicAuth() {
//     delete[] _username;
//     delete[] _password;
//     _username = nullptr;
//     _password = nullptr;
// }

// string HttpBasicAuth::get_auth() {
//     auto user_len = len(_username);
//     auto pass_len = len(_password);
//     auto auth_len = user_len + pass_len + 1;
//     uint8_t* auth = new uint8_t[auth_len];

//     concat(auth, _username, 0, user_len);
//     concat(auth, reinterpret_cast<const uint8_t*>(":"), user_len, 1);
//     concat(auth, _password, user_len + 1, pass_len);
//     string ret = base64_encode(auth, auth_len);
//     delete[] auth;
//     return ret;
// }

// int HttpBasicAuth::append_to_headers(curl_slist** headers) {
//     string auth = "Authorization: Basic " + get_auth();
//     *headers = curl_slist_append(*headers, auth.c_str());
//     return 1;
// }