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

    unique_ptr<uint8_t[]> data(to_bytes(buffer.GetString(), buffer.GetLength()));

    uint8_t resource_path[] = "/hotwallet/balance";
    size_t path_size = 18;
    AccountBalance ret;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data.get(),
                buffer.GetLength(),
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data.get(),
            buffer.GetLength(),
            signature.get(),
            timeout,
            argv,
            argc
        );
        ret.from_obj(res["params"].GetObject());
        ret.account = string(account);
    } catch (const std::exception& e) {
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

    unique_ptr<uint8_t[]> data(to_bytes(buffer.GetString(), buffer.GetLength()));

    uint8_t resource_path[] = "/hotwallet/charge";
    size_t path_size = 17;
    InternalTransaction ret;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data.get(),
                buffer.GetLength(),
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data.get(),
            buffer.GetLength(),
            signature.get(),
            timeout,
            argv,
            argc
        );
        ret.from_obj(res["params"].GetObject());
        ret.account = string(account);
    } catch (const std::exception& e) {
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

    unique_ptr<uint8_t[]> data(to_bytes(buffer.GetString(), buffer.GetLength()));

    uint8_t resource_path[] = "/hotwallet/award";
    size_t path_size = 16;
    InternalTransaction ret;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data.get(),
                buffer.GetLength(),
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data.get(),
            buffer.GetLength(),
            signature.get(),
            timeout,
            argv,
            argc
        );
        ret.from_obj(res["params"].GetObject());
        ret.account = string(account);
    } catch (const std::exception& e) {
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

    unique_ptr<uint8_t[]> data(to_bytes(buffer.GetString(), buffer.GetLength()));

    uint8_t resource_path[] = "/hotwallet/get_allowance";
    size_t path_size = 24;
    HotwalletAllowance ret;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data.get(),
                buffer.GetLength(),
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data.get(),
            buffer.GetLength(),
            signature.get(),
            timeout,
            argv,
            argc
        );

        ret.from_obj(res["params"].GetObject());
        ret.account = string(account);
    } catch (const std::exception& e) {
        throw;
    }
    
    return ret;
}
} // namespace Nemo