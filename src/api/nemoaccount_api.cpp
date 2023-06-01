#include "nemoapi.h"

namespace Nemo
{
NemoAccountApi::~NemoAccountApi() {
    delete client_;
    client_ = nullptr;
}

rapidjson::Document::Array NemoAccountApi::get_link(
    const char* main_account,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _main_account(main_account, allocator);
    params.AddMember("main_account", _main_account, allocator);

    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/account/get_link";
    size_t path_size = 17;
    
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
        return res["params"].GetArray();
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
}

rapidjson::Document::Object NemoAccountApi::get_nemo_wallet(
    const char* sub_account,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _sub_account(sub_account, allocator);
    params.AddMember("sub_account", _sub_account, allocator);
    
    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/account/get_nemo_wallet";
    size_t path_size = 24;
    
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
        return res["params"].GetObject();
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
}
} // namespace Nemo
