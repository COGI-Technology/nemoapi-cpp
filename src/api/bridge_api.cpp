#include "nemoapi.h"

BridgeApi::~BridgeApi() {
    delete _client;
    _client = nullptr;
}

rapidjson::Document::Array BridgeApi::pause(const vector<const char*> networks, vector<int> chain_ids) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    if (networks.size() > 0) {
        rapidjson::Value ns(rapidjson::kArrayType);
        for (auto i = 0; i < networks.size(); i++) {
            rapidjson::Value val(networks[i], allocator);
            ns.PushBack(val, allocator);
        }
        params.AddMember("networks", ns, allocator);
    }
    if (chain_ids.size() > 0) {
        rapidjson::Value ns(rapidjson::kArrayType);
        for (auto i = 0; i < chain_ids.size(); i++) {
            ns.PushBack(chain_ids[i], allocator);
        }
        params.AddMember("chainIds", ns, allocator);
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    printf("%s\n", buffer.GetString());
    struct nemoapi_memory* data = nemoapi_memory_from_str(buffer.GetString());
    struct nemoapi_memory* path = nemoapi_memory_from_str("/bridge/pause");
    
    try {
        auto res = _client->call_api(
            path,
            NEMOAPI_POST,
            NemoApiV2Auth,
            nullptr,
            nullptr,
            data
        );
        
        rapidjson::Document::Array ret = res["params"].GetArray();
        free(data);
        free(path);
        return ret;
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
}

rapidjson::Document::Array BridgeApi::unpause(const vector<const char*> networks, vector<int> chain_ids) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    if (networks.size() > 0) {
        rapidjson::Value ns(rapidjson::kArrayType);
        for (auto i = 0; i < networks.size(); i++) {
            rapidjson::Value val(networks[i], allocator);
            ns.PushBack(val, allocator);
        }
        params.AddMember("networks", ns, allocator);
    }
    if (chain_ids.size() > 0) {
        rapidjson::Value ns(rapidjson::kArrayType);
        for (auto i = 0; i < chain_ids.size(); i++) {
            ns.PushBack(chain_ids[i], allocator);
        }
        params.AddMember("chainIds", ns, allocator);
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    printf("%s\n", buffer.GetString());
    struct nemoapi_memory* data = nemoapi_memory_from_str(buffer.GetString());
    struct nemoapi_memory* path = nemoapi_memory_from_str("/bridge/unpause");
    
    try {
        auto res = _client->call_api(
            path,
            NEMOAPI_POST,
            NemoApiV2Auth,
            nullptr,
            nullptr,
            data
        );
        
        rapidjson::Document::Array ret = res["params"].GetArray();
        free(data);
        free(path);
        return ret;
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
}

rapidjson::Document::Array BridgeApi::block_token(const char* token, const char* network, int chain_id) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    rapidjson::Value t(token, allocator);
    params.AddMember("token", t, allocator);
    if (network != nullptr) {
        rapidjson::Value n(network, allocator);
        params.AddMember("network", n, allocator);
    } else if (chain_id > 0) {
        params.AddMember("chainId", chain_id, allocator);
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    printf("%s\n", buffer.GetString());
    struct nemoapi_memory* data = nemoapi_memory_from_str(buffer.GetString());
    struct nemoapi_memory* path = nemoapi_memory_from_str("/bridge/block_token");
    
    try {
        auto res = _client->call_api(
            path,
            NEMOAPI_POST,
            NemoApiV2Auth,
            nullptr,
            nullptr,
            data
        );
        
        rapidjson::Document::Array ret = res["params"].GetArray();
        free(data);
        free(path);
        return ret;
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
}

rapidjson::Document::Array BridgeApi::unblock_token(const char* token, const char* network, int chain_id) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    rapidjson::Value t(token, allocator);
    params.AddMember("token", t, allocator);
    if (network != nullptr) {
        rapidjson::Value n(network, allocator);
        params.AddMember("network", n, allocator);
    } else if (chain_id > 0) {
        params.AddMember("chainId", chain_id, allocator);
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    printf("%s\n", buffer.GetString());
    struct nemoapi_memory* data = nemoapi_memory_from_str(buffer.GetString());
    struct nemoapi_memory* path = nemoapi_memory_from_str("/bridge/unblock_token");
    
    try {
        auto res = _client->call_api(
            path,
            NEMOAPI_POST,
            NemoApiV2Auth,
            nullptr,
            nullptr,
            data
        );
        
        rapidjson::Document::Array ret = res["params"].GetArray();
        free(data);
        free(path);
        return ret;
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
}