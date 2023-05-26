#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstring>
#include <regex>
#include <sodium.h>
#include <cassert>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>

#include <ctime>
#include <chrono>

#define _assert(exp, msg) assert(((void)msg, exp))

using namespace std;

struct nemoapi_memory {
    uint8_t* data;
    size_t length;
};

struct URLComponents {
    string protocol;
    string host;
    string port;
    string path;
    string query;
    string fragment;
};

inline void free(struct nemoapi_memory* memory) {
    if (memory != nullptr) {
        if (memory->data != nullptr) {
            delete[] memory->data;
        }
        delete memory;
        memory = nullptr;
    }
}

inline struct curl_slist* curl_slist_extend(struct curl_slist* des, const struct curl_slist* src) {
    const struct curl_slist *node = src;
    while (node != NULL) {
        des = curl_slist_append(des, node->data);
        node = node->next;
    }
    return des;
}

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

inline struct URLComponents urlparse(const struct nemoapi_memory* url) {
    return urlparse(string(reinterpret_cast<char*>(url->data)));
}

inline string base64_encode(const struct nemoapi_memory* raw){
    const size_t ret_max_len = sodium_base64_encoded_len(raw->length, sodium_base64_VARIANT_ORIGINAL);
    char* ret = new char[ret_max_len];
    _assert(sodium_bin2base64(
            ret,
            ret_max_len,
            raw->data,
            raw->length,
            sodium_base64_VARIANT_ORIGINAL
    ) != NULL, "Failed sodium_bin2base64");
    return string(ret);
}

inline size_t calc_decode_len(const struct nemoapi_memory* b64_input) {
    size_t padding = 0;

    if (b64_input->length > 0 && b64_input->data[b64_input->length-1] == '=') {
        padding++;
        if (b64_input->length > 1 && b64_input->data[b64_input->length-2] == '=') {
            padding++;
        }
    }

    return (b64_input->length * 3 / 4) - padding;
}

inline struct nemoapi_memory* base64_decode(const struct nemoapi_memory* encoded) {
    // Determine the maximum possible length of the decoded string:
    size_t raw_len = calc_decode_len(encoded);
    _assert(raw_len > 0, "base64_decode: error getting max length");

    // Allocate a buffer for the decoded data:
    uint8_t* raw = new uint8_t[raw_len];

    // Decode the data:
    _assert(sodium_base642bin(
            raw,
            raw_len,
            reinterpret_cast<const char*>(encoded->data),
            encoded->length,
            nullptr,
            &raw_len,
            nullptr,
            sodium_base64_VARIANT_ORIGINAL
    ) == 0, "base64_decode: decoding error");

    struct nemoapi_memory *ret = new nemoapi_memory{nullptr, 0};
    ret->data = raw;
    ret->length = raw_len;
    return ret;
}

inline void concat(uint8_t* dest, const uint8_t* src, size_t offset, size_t src_len) {
    if (dest == nullptr || src == nullptr) {
        return;
    }
    if (offset < 0 || src_len <= 0) {
        return;
    }

    memcpy(dest + offset, src, src_len);
}

inline size_t len(const uint8_t* buffer) {
    size_t length = 0;
    while (*buffer++)
    {
        ++length;
    }
    return length;
}

inline struct nemoapi_memory* combine(
    const struct nemoapi_memory* a,
    const struct nemoapi_memory* b
) {
    struct nemoapi_memory* ret = new nemoapi_memory{nullptr, 0};
    ret->length = a->length + b->length;
    uint8_t* data = new uint8_t[ret->length];

    concat(data, a->data, 0, a->length);
    concat(data, b->data, a->length, b->length);
    memset(data + ret->length, 0, len(data) - ret->length);
    ret->data = data;
    return ret;
}

inline struct nemoapi_memory* nemoapi_memory_from_str(const unsigned char* str, size_t str_len) {
    struct nemoapi_memory* ret = new nemoapi_memory{nullptr, 0};
    uint8_t* data = new uint8_t[str_len];
    memcpy(data, str, str_len);
    memset(data + str_len, 0, len(data) - str_len);
    ret->data = data;
    ret->length = str_len;
    return ret;
}

inline struct nemoapi_memory* nemoapi_memory_from_str(const char* str) {
    auto ret = nemoapi_memory_from_str(reinterpret_cast<const unsigned char *>(str), strlen(str));
    return ret;
}

template<typename T>
inline rapidjson::Value vector_to_rapidjson_value(vector<T> arr, rapidjson::MemoryPoolAllocator<>& allocator) {
    rapidjson::Value ret(rapidjson::kArrayType);

    for (const auto& e: arr) {
        rapidjson::Value val(e, allocator);
        ret.PushBack(val, allocator);
    }

    return ret;
}

inline void replaceAll(string& str, const string old_s, const string new_s) {
    size_t pos = 0;

    while ((pos = str.find(old_s, pos)) != string::npos) {
        str.replace(pos, old_s.length(), new_s);
        pos += new_s.length();
    }
}

inline struct nemoapi_memory* json_decode(rapidjson::Document& doc) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    string decoded(buffer.GetString());
    
    replaceAll(decoded, "/", "\\/");
    
    struct nemoapi_memory* ret = nemoapi_memory_from_str(decoded.c_str());

    return ret;
}

#endif