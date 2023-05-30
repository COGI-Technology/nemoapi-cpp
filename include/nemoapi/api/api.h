#ifndef NEMOAPI_API_H
#define NEMOAPI_API_H

#include "client.h"
#include "../model.h"

using namespace std;

namespace Nemo
{
class HotwalletApi {
    public:
        HotwalletApi(APIClient* client): client_(client) {};
        ~HotwalletApi();
    
    public:
        AccountBalance balance(const char* account, void* argv[] = nullptr, size_t argc = 0, long timeout = 300L);
        InternalTransaction charge(const char* account, const char* amount, void* argv[] = nullptr, size_t argc = 0, long timeout = 300L);
        InternalTransaction award(const char* account, const char* amount, void* argv[] = nullptr, size_t argc = 0, long timeout = 300L);
        HotwalletAllowance get_allowance(const char* account, void* argv[] = nullptr, size_t argc = 0, long timeout = 300L);

    private:
        APIClient* client_;

};

class BridgeApi {
    public:
        BridgeApi(APIClient* client): client_(client) {};
        ~BridgeApi();
    
    public:
        rapidjson::Document::Array pause(
            const vector<const char*> networks,
            const vector<int> chain_ids,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );
        rapidjson::Document::Array unpause(
            const vector<const char*> networks,
            const vector<int> chain_ids,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );
        rapidjson::Document::Array block_token(
            const char* token,
            const char* network = nullptr,
            int chain_id = 0,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );
        rapidjson::Document::Array unblock_token(
            const char* token,
            const char* network = nullptr,
            int chain_id = 0,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );

    private:
        APIClient* client_;


};

class LandApi {
    public:
        LandApi(APIClient* client): client_(client) {};
        ~LandApi();
    
    public:
        string mint(
            const char* recipient,
            const char* land_id,
            const char* level,
            const char* land_x,
            const char* land_y,
            rapidjson::Document::Object metadata,
            const char* callback,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );
        string request_mint(
            const char* recipient,
            const char* land_id,
            const char* level,
            const char* land_x,
            const char* land_y,
            rapidjson::Document::Object metadata,
            const char* callback,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
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
        rapidjson::Document::Array request_mints(rapidjson::Document::Array lands, void* argv[] = nullptr, size_t argc = 0, long timeout = 300L);
        rapidjson::Document::Array mints(rapidjson::Document::Array lands, void* argv[] = nullptr, size_t argc = 0, long timeout = 300L);
        rapidjson::Document::Array request_cancelbuys(rapidjson::Document::Array lands, void* argv[] = nullptr, size_t argc = 0, long timeout = 300L);

    private:
        APIClient* client_;
};

class MysteryboxApi {
    public:
        MysteryboxApi(APIClient* client): client_(client) {};
        ~MysteryboxApi();
    
    public:
        string mint(
            const char* box_id,
            const char* recipient,
            rapidjson::Document::Object metadata,
            const char* callback,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );
        rapidjson::Document build_batch_mint(
            const vector<const char*> box_ids,
            const vector<const char*> recipients,
            const vector<rapidjson::Document::Object> metadatas,
            const vector<const char*> callbacks
        );
        rapidjson::Document::Array mints(
            rapidjson::Document::Array boxes,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );

    private:
        APIClient* client_;
};

class NemoAccountApi {
    public:
        NemoAccountApi(APIClient* client): client_(client) {};
        ~NemoAccountApi();
    
    public:
        rapidjson::Document::Array get_link(
            const char* main_account,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );
        rapidjson::Document::Object get_nemo_wallet(
            const char* sub_account,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );

    private:
        APIClient* client_;
};

class NemoIdApi {
    public:
        NemoIdApi(APIClient* client): client_(client) {};
        ~NemoIdApi();
    
    public:
        NemoIdAccount login(
            const char* code,
            const char* code_verifier,
            const char* redirect_uri,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );
        NemoIdAccount relogin(
            const char* refresh_token,
            const char* code_verifier,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );
        NemoIdAccount user_info(
            const char* access_token,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );

    private:
        APIClient* client_;
};

class NFTApi {
    public:
        NFTApi(APIClient* client): client_(client) {};
        ~NFTApi();
    
    public:
        string mint(
            const char* recipient,
            rapidjson::Document::Object metadata,
            const char* callback,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );
        string request_mint(
            const char* recipient,
            rapidjson::Document::Object metadata,
            const char* callback,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );

    private:
        APIClient* client_;
};

class OnchainApi {
    public:
        OnchainApi(APIClient* client): client_(client) {};
        ~OnchainApi();
    
    public:
        int scan_tx(
            int chain_id,
            const vector<const char*> hashes,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );

    private:
        APIClient* client_;
};

class SubgraphApi {
    public:
        SubgraphApi(APIClient* client): client_(client) {};
        ~SubgraphApi();
    
    public:
        rapidjson::Document::Object call(
            rapidjson::Document::Object params,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );
        rapidjson::Document::Object get_total_volume(
            rapidjson::Document::Object params,
            void* argv[] = nullptr,
            size_t argc = 0,
            long timeout = 300L
        );

    private:
        APIClient* client_;
};
} // namespace Nemo

#endif