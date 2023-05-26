#include "nemoapi.h"

NFTApi::~NFTApi() {
    delete _client;
    _client = nullptr;
}

string NFTApi::mint(
    const char* recipient,
    rapidjson::Document::Object metadata,
    const char* callback
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _recipient(recipient, allocator);
    params.AddMember("recipient", _recipient, allocator);
    rapidjson::Value _callback(callback, allocator);
    params.AddMember("callback", _callback, allocator);
    params.AddMember("data", metadata, allocator);
    
    struct nemoapi_memory* data = json_decode(params);
    struct nemoapi_memory* path = nemoapi_memory_from_str("/nft/mint");
    
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

        return res["uuid"].GetString();
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
}

string NFTApi::request_mint(
    const char* recipient,
    rapidjson::Document::Object metadata,
    const char* callback
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _recipient(recipient, allocator);
    params.AddMember("recipient", _recipient, allocator);
    rapidjson::Value _callback(callback, allocator);
    params.AddMember("callback", _callback, allocator);
    params.AddMember("data", metadata, allocator);
    
    struct nemoapi_memory* data = json_decode(params);
    struct nemoapi_memory* path = nemoapi_memory_from_str("/nft/request_mint");
    
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

        return res["uuid"].GetString();
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
}