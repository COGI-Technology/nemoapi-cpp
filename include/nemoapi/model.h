#ifndef NEMOAPI_MODEL_H
#define NEMOAPI_MODEL_H

#include "utils.h"


using namespace std;

namespace Nemo
{

enum InternalTransactionKind {
    TRANSACTION_INTERNAL = 207,
    TRANSACTION_ONCHAIN = 203,
    TRANSACTION_OFFCHAIN = 202,
    TRANSACTION_WITHDRAW = 201,
    TRANSACTION_DEPOSIT = 200,
};

class BaseModel{
    public:
        virtual void from_obj(rapidjson::Document::Object)=0;
};

class AccountBalance: public BaseModel {
    public:
        AccountBalance();
        ~AccountBalance();

    public:
        string account;
        string balance;
        string available_balance;
        string pending_balance;
        string pending_claim;

    public:
        void from_obj(rapidjson::Document::Object data);
};

class InternalTransaction: public AccountBalance {
    public:
        InternalTransaction();
        ~InternalTransaction();

    public:
        string tx_hash;
        string amount;
        InternalTransactionKind kind;

    public:
        void from_obj(rapidjson::Document::Object data);
};

class HotwalletAllowance: public BaseModel {
    public:
        HotwalletAllowance();
        ~HotwalletAllowance();

    public:
        string account;
        string spender;
        string amount;

    public:
        void from_obj(rapidjson::Document::Object data);
};

class Signature: public BaseModel {
    public:
        Signature();
        ~Signature();
    
    public:
        string message;
        string hash;
        int ttl;

    public:
        void from_obj(rapidjson::Document::Object data);
};

class NemoIdAccount: public BaseModel {
    public:
        NemoIdAccount();
        ~NemoIdAccount();

    public:
        string sub;
        string email;
        bool email_verified;
        string name;
        string gender;
        string birthday;
        string profile_picture;
        string public_key;
        string redirect_uri;
        string client_id;
        string access_token;
        int expires_in;
        string id_token;
        string refresh_token;
        string token_type;
        bool google_two_factor_authentication;
        bool fund_password;
        Signature signature;
        string nemo_address;
        vector<string> wallet_address;

    public:
        void from_obj(rapidjson::Document::Object data);
};
} // namespace Nemo

#endif