#include "nemoapi.h"

LandApi::~LandApi() {
    delete _client;
    _client = nullptr;
}

string LandApi::mint(
    const char* recipient,
    const char* land_id,
    const char* level,
    const char* land_x,
    const char* land_y,
    rapidjson::Document::Object metadata,
    const char* callback
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _recipient(recipient, allocator);
    params.AddMember("recipient", _recipient, allocator);
    rapidjson::Value _land_id(land_id, allocator);
    params.AddMember("landid", _land_id, allocator);
    rapidjson::Value _level(level, allocator);
    params.AddMember("level", _level, allocator);
    rapidjson::Value _land_x(land_x, allocator);
    params.AddMember("land_x", _land_x, allocator);
    rapidjson::Value _land_y(land_y, allocator);
    params.AddMember("land_y", _land_y, allocator);
    rapidjson::Value _callback(callback, allocator);
    params.AddMember("callback", _callback, allocator);
    params.AddMember("data", metadata, allocator);
    
    struct nemoapi_memory* data = json_decode(params);
    struct nemoapi_memory* path = nemoapi_memory_from_str("/land/mint");
    
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

string LandApi::request_mint(
    const char* recipient,
    const char* land_id,
    const char* level,
    const char* land_x,
    const char* land_y,
    rapidjson::Document::Object metadata,
    const char* callback
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _recipient(recipient, allocator);
    params.AddMember("recipient", _recipient, allocator);
    rapidjson::Value _land_id(land_id, allocator);
    params.AddMember("landid", _land_id, allocator);
    rapidjson::Value _level(level, allocator);
    params.AddMember("level", _level, allocator);
    rapidjson::Value _land_x(land_x, allocator);
    params.AddMember("land_x", _land_x, allocator);
    rapidjson::Value _land_y(land_y, allocator);
    params.AddMember("land_y", _land_y, allocator);
    rapidjson::Value _callback(callback, allocator);
    params.AddMember("callback", _callback, allocator);
    params.AddMember("data", metadata, allocator);
    
    struct nemoapi_memory* data = json_decode(params);
    struct nemoapi_memory* path = nemoapi_memory_from_str("/land/request_mint");
    
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

rapidjson::Document LandApi::build_batch_mint(
    const vector<const char*> recipients,
    const vector<const char*> landids,
    const vector<const char*> levels,
    const vector<const char*> land_xs,
    const vector<const char*> land_ys,
    const vector<rapidjson::Document::Object> metadatas,
    const vector<const char*> callbacks
) {
    rapidjson::Document ret(rapidjson::kArrayType);
    auto& allocator = ret.GetAllocator();

    for(auto i = 0; i < recipients.size(); i++) {
        rapidjson::Value e(rapidjson::kObjectType);

        rapidjson::Value recipient(recipients[i], allocator);
        e.AddMember("recipient", recipient, allocator);
        rapidjson::Value landid(landids[i], allocator);
        e.AddMember("landid", landid, allocator);
        rapidjson::Value level(levels[i], allocator);
        e.AddMember("level", level, allocator);
        rapidjson::Value land_x(land_xs[i], allocator);
        e.AddMember("land_x", land_x, allocator);
        rapidjson::Value land_y(land_ys[i], allocator);
        e.AddMember("land_y", land_y, allocator);
        rapidjson::Value callback(callbacks[i], allocator);
        e.AddMember("callback", callback, allocator);
        e.AddMember("data", metadatas[i], allocator);

        ret.PushBack(e, allocator);
    }

    return ret;
}

rapidjson::Document::Array LandApi::request_mints(rapidjson::Document::Array lands) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    params.AddMember("lands", lands, allocator);

    struct nemoapi_memory* data = json_decode(params);
    struct nemoapi_memory* path = nemoapi_memory_from_str("/land/request_mints");
    
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
        
        return res["uuid"].GetArray();
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
}

rapidjson::Document::Array LandApi::mints(rapidjson::Document::Array lands) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    params.AddMember("lands", lands, allocator);

    struct nemoapi_memory* data = json_decode(params);
    struct nemoapi_memory* path = nemoapi_memory_from_str("/land/mints");
    
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
        
        return res["uuid"].GetArray();
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
}

rapidjson::Document::Array LandApi::request_cancelbuys(rapidjson::Document::Array lands) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    params.AddMember("lands", lands, allocator);

    struct nemoapi_memory* data = json_decode(params);
    struct nemoapi_memory* path = nemoapi_memory_from_str("/land/request_cancelbuys");
    
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