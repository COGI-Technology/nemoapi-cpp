#ifndef __nemoapi__types_h
#define __nemoapi__types_h

#include "utils.h"
#include "enum.h"

namespace nemoapi {
namespace rest {

struct err_t{
    const char* p;
    bool is_null(){ return p == nullptr; }
    std::string as_string(){ return std::string(p); }
    friend std::ostream &operator<<(std::ostream &os, const err_t &f);
};

template<typename T>
using result_cb = std::function<bool(status_e status, err_t&& err, T&& res)>;

struct signature_t {
    signature_t()
        :ttl{0}
    {}
    std::string message;
    std::string hash;
    int ttl;
    static signature_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const signature_t &f);
};

//api/nemoid.md#user_info
struct nemoid_user_info_t {
    nemoid_user_info_t()
        :expires_in{0}
    {}
    std::string sub;
    std::string email;
    bool email_verified;
    std::string name;
    std::string gender;
    std::string birthday;
    std::string profile_picture;
    std::string public_key;
    std::string redirect_uri;
    std::string client_id;
    std::string access_token;
    int expires_in;
    std::string id_token;
    std::string refresh_token;
    std::string token_type;
    bool google_two_factor_authentication;
    bool fund_password;
    signature_t signature;
    std::string nemo_address;
    std::vector<std::string> wallet_address;
    static nemoid_user_info_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const nemoid_user_info_t &f);
};

//api/hotwallet.md#balance
struct hotwallet_balance_t {
    std::string balance;
    std::string available_balance;
    std::string pending_balance;
    std::string pending_claim;
    static hotwallet_balance_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const hotwallet_balance_t &f);
};

//api/hotwallet.md#award
struct hotwallet_award_t {
    std::string balance;
    std::string available_balance;
    std::string pending_balance;
    std::string pending_claim;
    std::string tx_hash;
    std::string amount;
    static hotwallet_award_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const hotwallet_award_t &f);
};

//api/hotwallet.md#charge
using hotwallet_charge_t = hotwallet_award_t;

//api/hotwallet.md#allowance
struct hotwallet_allowance_t {
    std::string account;
    std::string spender;
    std::string amount;
    static hotwallet_allowance_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const hotwallet_allowance_t &f);
};

//api/nft.md#mint
struct nft_mint_t {
    std::string uuid;
    static nft_mint_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const nft_mint_t &f);
};

//api/nft.md#request_mint
using nft_request_mint_t = nft_mint_t;

} //namespace rest
} //namespace nemoapi

#endif