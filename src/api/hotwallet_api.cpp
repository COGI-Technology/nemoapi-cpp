#include "nemoapi.h"

HotwalletApi::~HotwalletApi() {
    delete _client;
    _client = nullptr;
}

AccountBalance HotwalletApi::balance(const char* account) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    rapidjson::Value acct(account, allocator);
    params.AddMember("account", acct, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    printf("%s\n", buffer.GetString());
    struct nemoapi_memory* data = nemoapi_memory_from_str(buffer.GetString());
    struct nemoapi_memory* path = nemoapi_memory_from_str("/hotwallet/balance");
    AccountBalance ret;
    
    try {
        auto res = _client->call_api(
            path,
            NEMOAPI_POST,
            NemoApiV2Auth,
            nullptr,
            nullptr,
            data
        );
        ret.from_obj(res["params"].GetObject());
        // ret.account = account;
        free(data);
        free(path);
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
    
    return ret;
}

InternalTransaction HotwalletApi::charge(const char* account, unsigned long amount) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    rapidjson::Value acct(account, allocator);
    rapidjson::Value amt(to_string(amount).c_str(), allocator);
    params.AddMember("account", acct, allocator);
    params.AddMember("amount", amt, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    printf("%s\n", buffer.GetString());
    struct nemoapi_memory* data = nemoapi_memory_from_str(buffer.GetString());
    struct nemoapi_memory* path = nemoapi_memory_from_str("/hotwallet/charge");
    InternalTransaction ret;
    
    try {
        auto res = _client->call_api(
            path,
            NEMOAPI_POST,
            NemoApiV2Auth,
            nullptr,
            nullptr,
            data
        );
        ret.from_obj(res["params"].GetObject());
        ret.account = account;
        ret.kind = TRANSACTION_WITHDRAW;
        free(data);
        free(path);
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
    
    return ret;
}

InternalTransaction HotwalletApi::award(const char* account, unsigned long amount) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    rapidjson::Value acct(account, allocator);
    rapidjson::Value amt(to_string(amount).c_str(), allocator);
    params.AddMember("account", acct, allocator);
    params.AddMember("amount", amt, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    printf("%s\n", buffer.GetString());
    struct nemoapi_memory* data = nemoapi_memory_from_str(buffer.GetString());
    struct nemoapi_memory* path = nemoapi_memory_from_str("/hotwallet/award");
    InternalTransaction ret;
    
    try {
        auto res = _client->call_api(
            path,
            NEMOAPI_POST,
            NemoApiV2Auth,
            nullptr,
            nullptr,
            data
        );
        ret.from_obj(res["params"].GetObject());
        ret.account = account;
        ret.kind = TRANSACTION_DEPOSIT;
        free(data);
        free(path);
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
    
    return ret;
}

HotwalletAllowance HotwalletApi::get_allowance(const char* account) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    rapidjson::Value acct(account, allocator);
    params.AddMember("account", acct, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    printf("%s\n", buffer.GetString());
    struct nemoapi_memory* data = nemoapi_memory_from_str(buffer.GetString());
    struct nemoapi_memory* path = nemoapi_memory_from_str("/hotwallet/get_allowance");
    HotwalletAllowance ret;
    
    try {
        auto res = _client->call_api(
            path,
            NEMOAPI_POST,
            NemoApiV2Auth,
            nullptr,
            nullptr,
            data
        );
        ret.from_obj(res["params"].GetObject());
        ret.account = account;
        free(data);
        free(path);
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
    
    return ret;
}