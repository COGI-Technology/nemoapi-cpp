#include "nemoapi.h"

namespace Nemo
{
BridgeApi::~BridgeApi() {
    delete client_;
    client_ = nullptr;
}

rapidjson::Document::Array BridgeApi::pause(
    const vector<const char*> networks,
    vector<int> chain_ids,
    void* argv[],
    size_t argc,
    long timeout
) {
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

    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/bridge/pause";
    size_t path_size = 13;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data,
                data_size,
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data,
            data_size,
            signature.get(),
            timeout,
            argv,
            argc
        );
        delete[] data;
        rapidjson::Document::Array ret = res["params"].GetArray();
        return ret;
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
}

rapidjson::Document::Array BridgeApi::unpause(
    const vector<const char*> networks,
    vector<int> chain_ids,
    void* argv[],
    size_t argc,
    long timeout
) {
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

    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/bridge/unpause";
    size_t path_size = 15;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data,
                data_size,
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data,
            data_size,
            signature.get(),
            timeout,
            argv,
            argc
        );
        delete[] data;
        rapidjson::Document::Array ret = res["params"].GetArray();
        return ret;
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
}

rapidjson::Document::Array BridgeApi::block_token(
    const char* token,
    const char* network,
    int chain_id,
    void* argv[],
    size_t argc,
    long timeout
) {
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

    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/bridge/block_token";
    size_t path_size = 19;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data,
                data_size,
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data,
            data_size,
            signature.get(),
            timeout,
            argv,
            argc
        );
        delete[] data;
        rapidjson::Document::Array ret = res["params"].GetArray();
        return ret;
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
}

rapidjson::Document::Array BridgeApi::unblock_token(
    const char* token,
    const char* network,
    int chain_id,
    void* argv[],
    size_t argc,
    long timeout
) {
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

    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/bridge/unblock_token";
    size_t path_size = 21;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data,
                data_size,
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data,
            data_size,
            signature.get(),
            timeout,
            argv,
            argc
        );
        delete[] data;
        rapidjson::Document::Array ret = res["params"].GetArray();
        return ret;
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
}
} // namespace Nemo
