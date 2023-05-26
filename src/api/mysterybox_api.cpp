#include "nemoapi.h"

MysteryboxApi::~MysteryboxApi() {
    delete _client;
    _client = nullptr;
}

string MysteryboxApi::mint(
    const char* box_id,
    const char* recipient,
    rapidjson::Document::Object metadata,
    const char* callback
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _recipient(recipient, allocator);
    params.AddMember("recipient", _recipient, allocator);
    rapidjson::Value _box_id(box_id, allocator);
    params.AddMember("boxid", _box_id, allocator);
    rapidjson::Value _callback(callback, allocator);
    params.AddMember("callback", _callback, allocator);
    params.AddMember("data", metadata, allocator);
    
    struct nemoapi_memory* data = json_decode(params);
    struct nemoapi_memory* path = nemoapi_memory_from_str("/mysterybox/mint");
    
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

rapidjson::Document MysteryboxApi::build_batch_mint(
    const vector<const char*> box_ids,
    const vector<const char*> recipients,
    const vector<rapidjson::Document::Object> metadatas,
    const vector<const char*> callbacks
) {
    rapidjson::Document ret(rapidjson::kArrayType);
    auto& allocator = ret.GetAllocator();

    for(auto i = 0; i < recipients.size(); i++) {
        rapidjson::Value e(rapidjson::kObjectType);

        rapidjson::Value recipient(recipients[i], allocator);
        e.AddMember("recipient", recipient, allocator);
        rapidjson::Value boxid(box_ids[i], allocator);
        e.AddMember("boxid", boxid, allocator);
        rapidjson::Value callback(callbacks[i], allocator);
        e.AddMember("callback", callback, allocator);
        e.AddMember("data", metadatas[i], allocator);

        ret.PushBack(e, allocator);
    }

    return ret;
}

rapidjson::Document::Array MysteryboxApi::mints(rapidjson::Document::Array boxes) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    params.AddMember("boxs", boxes, allocator);

    struct nemoapi_memory* data = json_decode(params);
    struct nemoapi_memory* path = nemoapi_memory_from_str("/mysterybox/mints");
    
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