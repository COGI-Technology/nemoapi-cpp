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

using land_mint_t = nft_mint_t;
using land_request_mint_t = nft_mint_t;

struct land_mints_response_t {
    std::string landid;
    std::string uuid;
    int status;
    static land_mints_response_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const land_mints_response_t &f);
};

using land_request_mints_response_t = land_mints_response_t;
struct land_mints_t {
    std::vector<land_mints_response_t> uuids;
    static land_mints_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const land_mints_t &f);
};
using land_request_mints_t = land_mints_t;

struct land_request_cancelbuys_response_t {
    std::string cid;
    int status;
    static land_request_cancelbuys_response_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const land_request_cancelbuys_response_t &f);
};
struct land_request_cancelbuys_t {
    std::vector<land_request_cancelbuys_response_t> cids;
    static land_request_cancelbuys_t from_obj(const rapidjson::Document::Array obj);
    friend std::ostream &operator<<(std::ostream &os, const land_request_cancelbuys_t &f);
};

// mysterybox
using mysterybox_mint_t = nft_mint_t;
struct mysterybox_mints_response_t {
    std::string boxid;
    std::string uuid;
    int status;
    static mysterybox_mints_response_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const mysterybox_mints_response_t &f);
};
struct mysterybox_mints_t {
    std::vector<mysterybox_mints_response_t> uuids;
    static mysterybox_mints_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const mysterybox_mints_t &f);
};

// nemoaccount
struct nemoaccount_get_link_t {
    std::vector<std::string> accounts;
    static nemoaccount_get_link_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const nemoaccount_get_link_t &f);
};
struct nemoaccount_get_nemo_wallet_t {
    std::string nemo_wallet;
    static nemoaccount_get_nemo_wallet_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const nemoaccount_get_nemo_wallet_t &f);
};

// bridge
struct bridge_pause_response_t {
    std::string _namespace;
    int status;
    std::string params;
    static bridge_pause_response_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const bridge_pause_response_t &f);
};

using bridge_unpause_response_t = bridge_pause_response_t;

struct bridge_pause_t {
    std::vector<bridge_pause_response_t> responses;
    static bridge_pause_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const bridge_pause_t &f);
};

using bridge_unpause_t = bridge_pause_t;

using bridge_block_token_response_t = bridge_pause_response_t;
using bridge_unblock_token_response_t = bridge_pause_response_t;
using bridge_block_token_t = bridge_pause_t;
using bridge_unblock_token_t = bridge_pause_t;

//subgraph
struct subgraph_call_t {
    std::string data;
    static subgraph_call_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const subgraph_call_t &f);
};
struct subgraph_get_total_volume_t {
    std::string totalvolumn;
    static subgraph_get_total_volume_t from_obj(const rapidjson::Document::Object obj);
    friend std::ostream &operator<<(std::ostream &os, const subgraph_get_total_volume_t &f);
};

} //namespace rest
} //namespace nemoapi

#endif