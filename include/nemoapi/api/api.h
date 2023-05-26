#ifndef NEMOAPI_API_H
#define NEMOAPI_API_H

#include "client.h"
#include "../model.h"

using namespace std;

class HotwalletApi {
    public:
        HotwalletApi(APIClient* client): _client(client) {};
        ~HotwalletApi();
    
    public:
        AccountBalance balance(const char* account);
        InternalTransaction charge(const char* account, unsigned long amount);
        InternalTransaction award(const char* account, unsigned long amount);
        HotwalletAllowance get_allowance(const char* account);

    private:
        APIClient* _client;


};

class BridgeApi {
    public:
        BridgeApi(APIClient* client): _client(client) {};
        ~BridgeApi();
    
    public:
        rapidjson::Document::Array pause(const vector<const char*> networks, const vector<int> chain_ids);
        rapidjson::Document::Array unpause(const vector<const char*> networks, const vector<int> chain_ids);
        rapidjson::Document::Array block_token(const char* token, const char* network = nullptr, int chain_id = 0);
        rapidjson::Document::Array unblock_token(const char* token, const char* network = nullptr, int chain_id = 0);

    private:
        APIClient* _client;


};

class LandApi {
    public:
        LandApi(APIClient* client): _client(client) {};
        ~LandApi();
    
    public:
        string mint(
            const char* recipient,
            const char* land_id,
            const char* level,
            const char* land_x,
            const char* land_y,
            rapidjson::Document::Object metadata,
            const char* callback
        );
        string request_mint(
            const char* recipient,
            const char* land_id,
            const char* level,
            const char* land_x,
            const char* land_y,
            rapidjson::Document::Object metadata,
            const char* callback
        );
        rapidjson::Document build_batch_mint(
            const vector<const char*> recipient,
            const vector<const char*> land_id,
            const vector<const char*> level,
            const vector<const char*> land_x,
            const vector<const char*> land_y,
            const vector<rapidjson::Document::Object> metadata,
            const vector<const char*> callback
        );
        rapidjson::Document::Array request_mints(rapidjson::Document::Array lands);
        rapidjson::Document::Array mints(rapidjson::Document::Array lands);
        rapidjson::Document::Array request_cancelbuys(rapidjson::Document::Array lands);

    private:
        APIClient* _client;
};

class MysteryboxApi {
    public:
        MysteryboxApi(APIClient* client): _client(client) {};
        ~MysteryboxApi();
    
    public:
        string mint(
            const char* box_id,
            const char* recipient,
            rapidjson::Document::Object metadata,
            const char* callback
        );
        rapidjson::Document build_batch_mint(
            const vector<const char*> box_ids,
            const vector<const char*> recipients,
            const vector<rapidjson::Document::Object> metadatas,
            const vector<const char*> callbacks
        );
        rapidjson::Document::Array mints(rapidjson::Document::Array boxes);

    private:
        APIClient* _client;
};

class NemoAccountApi {
    public:
        NemoAccountApi(APIClient* client): _client(client) {};
        ~NemoAccountApi();
    
    public:
        rapidjson::Document::Array get_link(const char* main_account);
        rapidjson::Document::Object get_nemo_wallet(const char* sub_account);

    private:
        APIClient* _client;
};

class NemoIdApi {
    public:
        NemoIdApi(APIClient* client): _client(client) {};
        ~NemoIdApi();
    
    public:
        NemoIdAccount login(
            const char* code,
            const char* code_verifier,
            const char* redirect_uri
        );
        NemoIdAccount relogin(
            const char* refresh_token,
            const char* code_verifier
        );
        NemoIdAccount user_info(const char* access_token);

    private:
        APIClient* _client;
};

class NFTApi {
    public:
        NFTApi(APIClient* client): _client(client) {};
        ~NFTApi();
    
    public:
        string mint(
            const char* recipient,
            rapidjson::Document::Object metadata,
            const char* callback
        );
        string request_mint(
            const char* recipient,
            rapidjson::Document::Object metadata,
            const char* callback
        );

    private:
        APIClient* _client;
};

class OnchainApi {
    public:
        OnchainApi(APIClient* client): _client(client) {};
        ~OnchainApi();
    
    public:
        int scan_tx(int chain_id, const vector<const char*> hashes);

    private:
        APIClient* _client;
};

class SubgraphApi {
    public:
        SubgraphApi(APIClient* client): _client(client) {};
        ~SubgraphApi();
    
    public:
        rapidjson::Document::Object call(rapidjson::Document::Object params);
        rapidjson::Document::Object get_total_volume(rapidjson::Document::Object params);

    private:
        APIClient* _client;
};

#endif