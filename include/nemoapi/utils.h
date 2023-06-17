#ifndef __nemoapi__utils_h
#define __nemoapi__utils_h

#include "logging.h"

#include <string>
#include <cstring>
#include <regex>
#include <memory>
#include <sodium.h>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>

#include <ctime>
#include <chrono>

namespace nemoapi 
{

inline std::string base64_encode(const uint8_t* raw, size_t raw_len){
    const size_t ret_len = sodium_base64_encoded_len(raw_len, sodium_base64_VARIANT_ORIGINAL);
    std::string ret(ret_len-1, 0);
    if (sodium_bin2base64(const_cast<char*>(ret.c_str()), ret_len, raw, raw_len, sodium_base64_VARIANT_ORIGINAL) == NULL) {
        ret.clear();
        LOG_ERROR << "Failed sodium_bin2base64";
        return "";
    }
    return ret;
}

inline std::string base64_decode(const char* enc, size_t enc_len) {
    size_t ret_len = enc_len*3/4;
    std::string ret(ret_len-1, 0);
    if(sodium_base642bin(reinterpret_cast<uint8_t*>(const_cast<char*>(ret.c_str())), ret_len, enc, enc_len, nullptr, &ret_len, nullptr, sodium_base64_VARIANT_ORIGINAL) != 0){
        LOG_FATAL << "base64_decode: decoding error";
        ret.clear();
        return "";
    }
    ret.erase(ret.end() - (ret.length() - ret_len), ret.end());
    return ret;
}

inline std::string base64_encode(std::string raw) {
    return base64_encode(reinterpret_cast<const uint8_t*>(raw.c_str()), raw.length());
}

inline std::string base64_decode(std::string enc) {
    return base64_decode(enc.c_str(), enc.length());
}

inline std::time_t timestamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
}

inline void replaceAll(std::string& str, const std::string old_s, const std::string new_s) {
    size_t pos = 0;

    while ((pos = str.find(old_s, pos)) != std::string::npos) {
        str.replace(pos, old_s.length(), new_s);
        pos += new_s.length();
    }
}

inline std::string json_encode(rapidjson::Document& doc) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::string enc(buffer.GetString());
    replaceAll(enc, "/", "\\/");

    return enc;
}

inline struct curl_slist* curl_slist_extend(struct curl_slist* des, const struct curl_slist* src) {
    const struct curl_slist *node = src;
    while (node != NULL) {
        des = curl_slist_append(des, node->data);
        node = node->next;
    }
    return des;
}

} //namespace nemoapi

#endif