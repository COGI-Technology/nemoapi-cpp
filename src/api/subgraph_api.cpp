#include "nemoapi.h"

SubgraphApi::~SubgraphApi() {
    delete _client;
    _client = nullptr;
}

rapidjson::Value::Object SubgraphApi::call(rapidjson::Value::Object params) {
    rapidjson::Document doc(rapidjson::kObjectType);
    auto& allocator = doc.GetAllocator();
    rapidjson::Value val(params);
    doc.CopyFrom(val, allocator);
    
    struct nemoapi_memory* data = json_decode(doc);
    struct nemoapi_memory* path = nemoapi_memory_from_str("/subgraph/call");
    
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
        return res.GetObject();
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
}

rapidjson::Value::Object SubgraphApi::get_total_volume(rapidjson::Value::Object params) {
    rapidjson::Document doc(rapidjson::kObjectType);
    auto& allocator = doc.GetAllocator();
    rapidjson::Value val(params);
    doc.CopyFrom(val, allocator);
    
    struct nemoapi_memory* data = json_decode(doc);
    struct nemoapi_memory* path = nemoapi_memory_from_str("/subgraph/getTotalVolume");
    
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
        return res.GetObject();
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
}