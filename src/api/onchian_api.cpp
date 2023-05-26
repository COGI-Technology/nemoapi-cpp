#include "nemoapi.h"

OnchainApi::~OnchainApi() {
    delete _client;
    _client = nullptr;
}

int OnchainApi::scan_tx(int chain_id, const vector<const char*> hashes) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value ns(rapidjson::kArrayType);
    for (auto i = 0; i < hashes.size(); i++) {
        rapidjson::Value val(hashes[i], allocator);
        ns.PushBack(val, allocator);
    }

    params.AddMember("hashes", ns, allocator);
    params.AddMember("chain_id", chain_id, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    printf("%s\n", buffer.GetString());
    struct nemoapi_memory* data = nemoapi_memory_from_str(buffer.GetString());
    struct nemoapi_memory* path = nemoapi_memory_from_str("/onchain/scan_tx");
    
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
        return res["status"].GetInt();
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
}