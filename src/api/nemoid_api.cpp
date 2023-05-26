#include "nemoapi.h"

NemoIdApi::~NemoIdApi() {
    delete _client;
    _client = nullptr;
}

NemoIdAccount NemoIdApi::login(
    const char* code,
    const char* code_verifier,
    const char* redirect_uri
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    
    rapidjson::Value _code(code, allocator);
    params.AddMember("code", _code, allocator);
    rapidjson::Value _code_verifier(code_verifier, allocator);
    params.AddMember("code_verifier", _code_verifier, allocator);
    rapidjson::Value _redirect_uri(redirect_uri, allocator);
    params.AddMember("redirect_uri", _redirect_uri, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    printf("%s\n", buffer.GetString());
    struct nemoapi_memory* data = nemoapi_memory_from_str(buffer.GetString());
    struct nemoapi_memory* path = nemoapi_memory_from_str("/nemoid/login");
    NemoIdAccount ret;
    
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
        free(data);
        free(path);
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
    
    return ret;
}

NemoIdAccount NemoIdApi::relogin(
    const char* refresh_token,
    const char* code_verifier
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _refresh_token(refresh_token, allocator);
    params.AddMember("refresh_token", _refresh_token, allocator);
    rapidjson::Value _code_verifier(code_verifier, allocator);
    params.AddMember("code_verifier", _code_verifier, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    printf("%s\n", buffer.GetString());
    struct nemoapi_memory* data = nemoapi_memory_from_str(buffer.GetString());
    struct nemoapi_memory* path = nemoapi_memory_from_str("/nemoid/relogin");
    NemoIdAccount ret;
    
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
        free(data);
        free(path);
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
    
    return ret;
}

NemoIdAccount NemoIdApi::user_info(const char* access_token) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _access_token(access_token, allocator);
    params.AddMember("access_token", _access_token, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);

    printf("%s\n", buffer.GetString());
    struct nemoapi_memory* data = nemoapi_memory_from_str(buffer.GetString());
    struct nemoapi_memory* path = nemoapi_memory_from_str("/nemoid/user_info");
    NemoIdAccount ret;
    
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
        free(data);
        free(path);
    } catch (const std::exception& e) {
        free(data);
        free(path);
        throw;
    }
    
    return ret;
}