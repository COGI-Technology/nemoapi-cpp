#include "nemoapi.h"

namespace Nemo
{
AccountBalance::AccountBalance() {}
AccountBalance::~AccountBalance() {}

void AccountBalance::from_obj(rapidjson::Document::Object data) {
    balance = data["balance"].GetString();
    available_balance = data["available_balance"].GetString();
    pending_balance = data["pending_balance"].GetString();
    pending_claim = data["pending_claim"].GetString();
}

InternalTransaction::InternalTransaction() {}
InternalTransaction::~InternalTransaction() {}

void InternalTransaction::from_obj(rapidjson::Document::Object data) {
    AccountBalance::from_obj(data);
    tx_hash = data["tx_hash"].GetString();
    amount = data["amount"].GetString();
}

HotwalletAllowance::HotwalletAllowance() {}
HotwalletAllowance::~HotwalletAllowance() {}

void HotwalletAllowance::from_obj(rapidjson::Document::Object data) {
    spender = data["spender"].GetString();
    amount = data["amount"].GetString();
}

Signature::Signature() {}
Signature::~Signature() {}

void Signature::from_obj(rapidjson::Document::Object data) {
    message = data["message"].GetString();
    hash = data["hash"].GetString();
    ttl = data["ttl"].GetInt();
}

NemoIdAccount::NemoIdAccount() {}
NemoIdAccount::~NemoIdAccount() {}

void NemoIdAccount::from_obj(rapidjson::Document::Object data) {
    sub = data.HasMember("sub") ? data["sub"].GetString() : "";
    email = data.HasMember("email") ? data["email"].GetString() : "";
    email_verified = data.HasMember("email_verified") ? data["email_verified"].GetBool() : false;
    name = data.HasMember("name") ? data["name"].GetString() : "";
    gender = data.HasMember("gender") ? data["gender"].GetString() : "";
    birthday = data.HasMember("birthday") ? data["birthday"].GetString() : "";
    profile_picture = data.HasMember("profile_picture") ? data["profile_picture"].GetString() : "";
    public_key = data.HasMember("public_key") ? data["public_key"].GetString() : "";
    redirect_uri = data.HasMember("redirect_uri") ? data["redirect_uri"].GetString() : "";
    client_id = data.HasMember("client_id") ? data["client_id"].GetString() : "";
    access_token = data.HasMember("access_token") ? data["access_token"].GetString() : "";
    expires_in = data.HasMember("expires_in") ? data["expires_in"].GetInt() : -1;
    id_token = data.HasMember("id_token") ? data["id_token"].GetString() : "";
    refresh_token = data.HasMember("refresh_token") ? data["refresh_token"].GetString() : "";
    token_type = data.HasMember("token_type") ? data["token_type"].GetString() : "";
    google_two_factor_authentication = data.HasMember("google_two_factor_authentication") ? data["google_two_factor_authentication"].GetBool() : false;
    fund_password = data.HasMember("fund_password") ? data["fund_password"].GetBool() : false;
    if (data.HasMember("signature"))
        signature.from_obj(data["signature"].GetObject());
    nemo_address = data.HasMember("nemo_address") ? data["nemo_address"].GetString() : "";
    if (data.HasMember("wallet_address")) {
        auto arr = data["wallet_address"].GetArray();
        for (auto& value: arr) {
            wallet_address.push_back(value.GetString());
        }
    }
}
} // namespace Nemo

