#include "nemoapi.h"

namespace Nemo
{
OnchainApi::~OnchainApi() {
    delete client_;
    client_ = nullptr;
}

int OnchainApi::scan_tx(
    int chain_id,
    const vector<const char*> hashes,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value ns(rapidjson::kArrayType);
    for (auto i = 0; i < hashes.size(); i++) {
        rapidjson::Value val(hashes[i], allocator);
        ns.PushBack(val, allocator);
    }

    params.AddMember("hashes", ns, allocator);
    params.AddMember("chain_id", chain_id, allocator);

    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/onchain/scan_tx";
    size_t path_size = 16;
    
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
        return res["status"].GetInt();
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
}
} // namespace Nemo
