#include "nemoapi.h"

NemoAccountApi::~NemoAccountApi() {
    delete _client;
    _client = nullptr;
}

rapidjson::Document::Array NemoAccountApi::get_link(const char* main_account) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _main_account(main_account, allocator);
    params.AddMember("main_account", _main_account, allocator);
    
    struct nemoapi_memory* data = json_decode(params);
    struct nemoapi_memory* path = nemoapi_memory_from_str("/account/get_link");
    
    try {
        auto res = _client->call_api(
            path,
            NEMOAPI_POST,
            NemoApiV2Auth,
            nullptr,
            nullptr,
            data
        );
        free(data);
        free(path);
        return res["params"].GetArray();
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
}

rapidjson::Document::Object NemoAccountApi::get_nemo_wallet(const char* sub_account) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _sub_account(sub_account, allocator);
    params.AddMember("sub_account", _sub_account, allocator);
    
    struct nemoapi_memory* data = json_decode(params);
    struct nemoapi_memory* path = nemoapi_memory_from_str("/account/get_nemo_wallet");
    
    try {
        auto res = _client->call_api(
            path,
            NEMOAPI_POST,
            NemoApiV2Auth,
            nullptr,
            nullptr,
            data
        );
        free(data);
        free(path);
        return res["params"].GetObject();
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
}