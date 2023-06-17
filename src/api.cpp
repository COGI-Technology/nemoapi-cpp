#include <nemoapi/api.h>
#include <nemoapi/logging.h>

namespace nemoapi {
namespace rest {

bool hotwallet::balance(const std::string account, result_cb<hotwallet_balance_t>&& cb, const std::time_t timeout) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("account", rapidjson::Value(account.c_str(), allocator), allocator);

    const std::string&& data = json_encode(params);
    const std::string&& resource_path = "/v2/nft/request_mint";

    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool hotwallet::charge(
    const std::string account,
    const std::string amount,
    result_cb<hotwallet_charge_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("account", rapidjson::Value(account.c_str(), allocator), allocator);
    params.AddMember("amount", rapidjson::Value(amount.c_str(), allocator), allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/nft/request_mint";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool hotwallet::award(
    const std::string account,
    const std::string amount,
    result_cb<hotwallet_award_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("account", rapidjson::Value(account.c_str(), allocator), allocator);
    params.AddMember("amount", rapidjson::Value(amount.c_str(), allocator), allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/nft/request_mint";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool hotwallet::get_allowance(
    const std::string account,
    result_cb<hotwallet_allowance_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("account", rapidjson::Value(account.c_str(), allocator), allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/nft/request_mint";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool nemoid::login(
    const std::string code,
    const std::string code_verifier,
    const std::string redirect_uri,
    result_cb<nemoid_user_info_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("code", rapidjson::Value(code.c_str(), allocator), allocator);
    params.AddMember("code_verifier", rapidjson::Value(code_verifier.c_str(), allocator), allocator);
    params.AddMember("redirect_uri", rapidjson::Value(redirect_uri.c_str(), allocator), allocator);

    const std::string&& data = json_encode(params);
    const std::string&& resource_path = "/v2/nft/request_mint";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool nemoid::relogin(
    const std::string refresh_token,
    const std::string code_verifier,
    result_cb<nemoid_user_info_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("refresh_token", rapidjson::Value(refresh_token.c_str(), allocator), allocator);
    params.AddMember("code_verifier", rapidjson::Value(code_verifier.c_str(), allocator), allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/nft/request_mint";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool nemoid::user_info(
    const std::string access_token,
    result_cb<nemoid_user_info_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("access_token", rapidjson::Value(access_token.c_str(), allocator), allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/nft/request_mint";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool nft::mint(
    const std::string recipient,
    const rapidjson::Document::Object metadata,
    const std::string callback,
    result_cb<nft_mint_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("recipient", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("callback", rapidjson::Value(callback.c_str(), allocator), allocator);
    params.AddMember("data", metadata, allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/nft/request_mint";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool nft::request_mint(
    const std::string recipient,
    const rapidjson::Document::Object metadata,
    const std::string callback,
    result_cb<nft_request_mint_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("recipient", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("callback", rapidjson::Value(callback.c_str(), allocator), allocator);
    params.AddMember("data", metadata, allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/nft/request_mint";

    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

} //namespace rest
} //namespace nemoapi
