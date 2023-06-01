#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstring>
#include <regex>
#include <memory>
#include <sodium.h>
#include <cassert>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>

#include <ctime>
#include <chrono>

#define _assert(exp, msg) assert(((void)msg, exp))

using namespace std;

namespace Nemo 
{
struct URLComponents {
    string protocol;
    string host;
    string port;
    string path;
    string query;
    string fragment;
};
inline struct URLComponents urlparse(string url) {
    struct URLComponents ret;
    
    // Regular expression pattern for parsing URLs
    regex pattern(R"(^(?:(\w+):\/\/)?([^\/:]+)(?::(\d+))?([^\?\#]*)(?:\?([^#]*))?(?:#(.*))?$)");
    
    // Match results
    smatch match;
    
    // Execute the regular expression search on the URL string
    if (regex_search(url, match, pattern)) {
        // Extract components from the match results
        ret.protocol = match[1];
        ret.host = match[2];
        ret.port = match[3];
        ret.path = match[4];
        ret.query = match[5];
        ret.fragment = match[6];
    }
    return ret;
}

inline struct URLComponents urlparse(const uint8_t* url) {
    return urlparse(string(reinterpret_cast<const char*>(url)));
}

inline string base64_encode(const uint8_t* raw, size_t raw_len){
    const size_t ret_max_len = sodium_base64_encoded_len(raw_len, sodium_base64_VARIANT_ORIGINAL);
    char* b64 = new char[ret_max_len];
    if (sodium_bin2base64(b64, ret_max_len, raw,raw_len, sodium_base64_VARIANT_ORIGINAL) == NULL) {
        delete[] b64;
        _assert(false, "Failed sodium_bin2base64");
    }
    string ret(b64);
    delete[] b64;
    return ret;
}

inline size_t calc_decode_len(const char* b64_input, size_t len) {
    size_t padding = 0;

    if (len > 0 && b64_input[len-1] == '=') {
        padding++;
        if (len > 1 && b64_input[len-2] == '=') {
            padding++;
        }
    }

    return (len * 3 / 4) - padding;
}

inline void base64_decode(const char* encoded, size_t encoded_len, uint8_t* raw, size_t* raw_len) {
    // Determine the maximum possible length of the decoded string:
    *raw_len = calc_decode_len(encoded, encoded_len);
    _assert(raw_len > 0, "base64_decode: error getting max length");
    if (raw == nullptr)
        return;

    // Allocate a buffer for the decoded data:
    raw = (uint8_t*)realloc(raw, *raw_len);

    // Decode the data:
    _assert(sodium_base642bin(
            raw,
            *raw_len,
            encoded,
            encoded_len,
            nullptr,
            raw_len,
            nullptr,
            sodium_base64_VARIANT_ORIGINAL
    ) >= 0, "base64_decode: decoding error");
}

inline uint8_t* to_bytes(const char* data, size_t size, bool include_null_character = true) {
    uint8_t* ret = nullptr;
    if (include_null_character) {
        ret = new uint8_t[size + 1];
        ret[size] = '\0';
    }
    memcpy(ret, data, size);
    return ret;
}

inline uint8_t* to_bytes(const char* data) {
    return to_bytes(data, strlen(data), true);
}

/**
 * @brief Concatenates two uint8_t arrays into a single array.
 * 
 * @param a - Pointer to the first array
 * @param a_len - Length of the first array
 * @param b - Pointer to the second array
 * @param b_len - Length of the second array
 * 
 * @return A new uint8_t array containing the concatenated data.
 * This memory is allocated using new operator, so make sure to deallocate it when done.
 */
inline uint8_t* combine(
    const void* a,
    size_t a_len,
    const void* b,
    size_t b_len
) {
    // Allocate new memory for the combined array
    auto ret_len = a_len + b_len;
    uint8_t* ret = new uint8_t[ret_len + 1];

    // Copy the contents of 'a' and 'b' into the newly allocated memory
    memcpy(ret, a, a_len);
    memcpy(ret + a_len, b, b_len);
    ret[ret_len] = '\0';

    // Return the newly created and concatenated array
    return ret;
}

inline unsigned long timestamp() {
    auto millisec_since_epoch = chrono::duration_cast<chrono::milliseconds>
            (chrono::system_clock::now().time_since_epoch()).count();
    return millisec_since_epoch;
}

inline void replaceAll(string& str, const string old_s, const string new_s) {
    size_t pos = 0;

    while ((pos = str.find(old_s, pos)) != string::npos) {
        str.replace(pos, old_s.length(), new_s);
        pos += new_s.length();
    }
}

inline uint8_t* json_decode(rapidjson::Document& doc, size_t* decoded_size = nullptr) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    string decoded(buffer.GetString());
    
    replaceAll(decoded, "/", "\\/");
    
    uint8_t* ret = to_bytes(decoded.c_str(), decoded.length());
    if (decoded_size != nullptr) {
        *decoded_size = decoded.length();
    }

    return ret;
}

inline struct curl_slist* curl_slist_extend(struct curl_slist* des, const struct curl_slist* src) {
    const struct curl_slist *node = src;
    while (node != NULL) {
        des = curl_slist_append(des, node->data);
        node = node->next;
    }
    return des;
}
} // namespace Nemo

#endif