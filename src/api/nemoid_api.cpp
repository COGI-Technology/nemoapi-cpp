#include "nemoapi.h"

namespace Nemo
{
NemoIdApi::~NemoIdApi() {
    delete client_;
    client_ = nullptr;
}

NemoIdAccount NemoIdApi::login(
    const char* code,
    const char* code_verifier,
    const char* redirect_uri,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    
    rapidjson::Value _code(code, allocator);
    params.AddMember("code", _code, allocator);
    rapidjson::Value _code_verifier(code_verifier, allocator);
    params.AddMember("code_verifier", _code_verifier, allocator);
    rapidjson::Value _redirect_uri(redirect_uri, allocator);
    params.AddMember("redirect_uri", _redirect_uri, allocator);

    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/nemoid/login";
    size_t path_size = 13;
    NemoIdAccount ret;
    
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
        ret.from_obj(res["params"].GetObject());
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
    delete[] data;
    return ret;
}

NemoIdAccount NemoIdApi::relogin(
    const char* refresh_token,
    const char* code_verifier,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _refresh_token(refresh_token, allocator);
    params.AddMember("refresh_token", _refresh_token, allocator);
    rapidjson::Value _code_verifier(code_verifier, allocator);
    params.AddMember("code_verifier", _code_verifier, allocator);

    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/nemoid/relogin";
    size_t path_size = 15;
    NemoIdAccount ret;
    
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
        ret.from_obj(res["params"].GetObject());
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
    delete[] data;
    return ret;
}

NemoIdAccount NemoIdApi::user_info(
    const char* access_token,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _access_token(access_token, allocator);
    params.AddMember("access_token", _access_token, allocator);

    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/nemoid/user_info";
    size_t path_size = 17;
    NemoIdAccount ret;
    
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
        ret.from_obj(res["params"].GetObject());
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
    delete[] data;
    return ret;
}
} // namespace Nemo
