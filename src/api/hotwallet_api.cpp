#include "nemoapi.h"
#include <iostream>

namespace Nemo
{
HotwalletApi::~HotwalletApi() {
    delete client_;
    client_ = nullptr;
}

AccountBalance HotwalletApi::balance(const char* account, void* argv[], size_t argc, long timeout) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    rapidjson::Value acct(account, allocator);
    params.AddMember("account", acct, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/hotwallet/balance";
    size_t path_size = 18;
    AccountBalance ret;
    
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
        ret.account = string(account);
        delete[] data;
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
    
    return ret;
}

InternalTransaction HotwalletApi::charge(
    const char* account,
    const char* amount,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    rapidjson::Value acct(account, allocator);
    rapidjson::Value amt(amount, allocator);
    params.AddMember("account", acct, allocator);
    params.AddMember("amount", amt, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/hotwallet/charge";
    size_t path_size = 17;
    InternalTransaction ret;
    
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
        ret.account = string(account);
        delete[] data;
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
    
    return ret;
}

InternalTransaction HotwalletApi::award(
    const char* account,
    const char* amount,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    rapidjson::Value acct(account, allocator);
    rapidjson::Value amt(amount, allocator);
    params.AddMember("account", acct, allocator);
    params.AddMember("amount", amt, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/hotwallet/award";
    size_t path_size = 16;
    InternalTransaction ret;
    
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
        ret.account = string(account);
        delete[] data;
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
    
    return ret;
}

HotwalletAllowance HotwalletApi::get_allowance(
    const char* account,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    rapidjson::Value acct(account, allocator);
    params.AddMember("account", acct, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    size_t data_size;
    uint8_t* data = json_decode(params, &data_size);

    uint8_t resource_path[] = "/hotwallet/get_allowance";
    size_t path_size = 24;
    HotwalletAllowance ret;
    
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
        ret.account = string(account);
        delete[] data;
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
    
    return ret;
}
} // namespace Nemo