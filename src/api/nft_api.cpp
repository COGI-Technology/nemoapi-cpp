#include "nemoapi.h"

namespace Nemo
{
NFTApi::~NFTApi() {
    delete client_;
    client_ = nullptr;
}

string NFTApi::mint(
    const char* recipient,
    rapidjson::Document::Object metadata,
    const char* callback,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _recipient(recipient, allocator);
    params.AddMember("recipient", _recipient, allocator);
    rapidjson::Value _callback(callback, allocator);
    params.AddMember("callback", _callback, allocator);
    params.AddMember("data", metadata, allocator);

    size_t data_size;
    unique_ptr<uint8_t[]> data(json_decode(params, &data_size));

    uint8_t resource_path[] = "/nft/mint";
    size_t path_size = 9;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data.get(),
                data_size,
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data.get(),
            data_size,
            signature.get(),
            timeout,
            argv,
            argc
        );
        return res["uuid"].GetString();
    } catch (const std::exception& e) {
        throw;
    }
}

string NFTApi::request_mint(
    const char* recipient,
    rapidjson::Document::Object metadata,
    const char* callback,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _recipient(recipient, allocator);
    params.AddMember("recipient", _recipient, allocator);
    rapidjson::Value _callback(callback, allocator);
    params.AddMember("callback", _callback, allocator);
    params.AddMember("data", metadata, allocator);
    
    size_t data_size;
    unique_ptr<uint8_t[]> data(json_decode(params, &data_size));

    uint8_t resource_path[] = "/nft/request_mint";
    size_t path_size = 17;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data.get(),
                data_size,
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data.get(),
            data_size,
            signature.get(),
            timeout,
            argv,
            argc
        );

        return res["uuid"].GetString();
    } catch (const std::exception& e) {
        throw;
    }
}
} // namespace Nemo
