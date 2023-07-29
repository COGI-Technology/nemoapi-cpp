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

bool land::mint(
    const std::string recipient,
    const std::string land_id,
    const std::string level,
    const std::string land_x,
    const std::string land_y,
    const rapidjson::Document::Object metadata,
    const std::string callback,
    result_cb<land_mint_t>&& cb,
    const std::time_t timeout  
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("recipient", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("landid", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("level", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("land_x", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("land_y", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("callback", rapidjson::Value(callback.c_str(), allocator), allocator);
    params.AddMember("data", metadata, allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/land/mint";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool land::request_mint(
    const std::string recipient,
    const std::string land_id,
    const std::string level,
    const std::string land_x,
    const std::string land_y,
    const rapidjson::Document::Object metadata,
    const std::string callback,
    result_cb<land_request_mint_t>&& cb,
    const std::time_t timeout  
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("recipient", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("landid", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("level", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("land_x", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("land_y", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("callback", rapidjson::Value(callback.c_str(), allocator), allocator);
    params.AddMember("data", metadata, allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/land/request_mint";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool land::mints(
    const rapidjson::Document::Array lands,
    result_cb<land_mints_t>&& cb,
    const std::time_t timeout  
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("lands", lands, allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/land/mints";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool land::request_mints(
    const rapidjson::Document::Array lands,
    result_cb<land_request_mints_t>&& cb,
    const std::time_t timeout  
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("lands", lands, allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/land/request_mints";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool mysterybox::mint(
    const std::string recipient,
    const std::string box_id,
    const rapidjson::Document::Object metadata,
    const std::string callback,
    result_cb<mysterybox_mint_t>&& cb,
    const std::time_t timeout  
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("recipient", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("boxid", rapidjson::Value(recipient.c_str(), allocator), allocator);
    params.AddMember("callback", rapidjson::Value(callback.c_str(), allocator), allocator);
    params.AddMember("data", metadata, allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/mysterybox/mint";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool mysterybox::mints(
    const rapidjson::Document::Array boxes,
    result_cb<mysterybox_mints_t>&& cb,
    const std::time_t timeout  
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("boxs", boxes, allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/mysterybox/mints";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool nemoaccount::get_link(
    const std::string main_account,
    result_cb<nemoaccount_get_link_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("main_account", rapidjson::Value(main_account.c_str(), allocator), allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/account/get_link";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool nemoaccount::get_nemo_wallet(
    const std::string sub_account,
    result_cb<nemoaccount_get_nemo_wallet_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("sub_account", rapidjson::Value(sub_account.c_str(), allocator), allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/account/get_nemo_wallet";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool bridge::pause(
    const rapidjson::Document::Array networks,
    result_cb<bridge_pause_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("networks", networks, allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/bridge/pause";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool bridge::unpause(
    const rapidjson::Document::Array networks,
    result_cb<bridge_unpause_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("networks", networks, allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/bridge/unpause";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool bridge::block_token(
    const std::string token,
    const std::string network,
    result_cb<bridge_block_token_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("token", rapidjson::Value(token.c_str(), allocator), allocator);
    params.AddMember("network", rapidjson::Value(network.c_str(), allocator), allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/bridge/block_token";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool bridge::unblock_token(
    const std::string token,
    const std::string network,
    result_cb<bridge_unblock_token_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    params.AddMember("token", rapidjson::Value(token.c_str(), allocator), allocator);
    params.AddMember("network", rapidjson::Value(network.c_str(), allocator), allocator);

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/bridge/unblock_token";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
}

bool subgraph::call(
    const rapidjson::Document::Object variables,
    result_cb<subgraph_call_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    for (auto& v: variables) {
        params.AddMember(v.name, v.value, allocator);
    }

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/subgraph/call";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
};

bool subgraph::get_total_volume(
    const rapidjson::Document::Object variables,
    result_cb<subgraph_get_total_volume_t>&& cb,
    const std::time_t timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();
    for (auto& v: variables) {
        params.AddMember(v.name, v.value, allocator);
    }

    const std::string data = json_encode(params);
    const std::string resource_path = "/v2/subgraph/getTotalVolume";
    
    return make_request(std::move(resource_path), std::move(cb), std::move(data), timeout);
};

} //namespace rest
} //namespace nemoapi
