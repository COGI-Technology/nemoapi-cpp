#ifndef __nemoapi__api_h
#define __nemoapi__api_h

#if defined(NEMOAPI_ASYNC)
#include "client_async.h"
#endif
#include "client_sync.h"
#include "enum.h"
#include "types.h"

#include <memory>

namespace nemoapi {
namespace rest {

class base {
    public:
        base(client* c): _client(c) {};
        ~base(){delete _client;};

    public:
        template<typename T>
        bool make_request(const std::string&& resource_path, result_cb<T>&& cb, const std::string&& data, const std::time_t timeout = 300L);
    
    protected:
        client* _client;
};

template<typename T>
bool base::make_request(const std::string&& resource_path, result_cb<T>&& cb, const std::string&& data, const std::time_t timeout){
    return _client->make_request(std::move(resource_path), [=](const char* res, const char* err){
        if(err != nullptr){
            return _client->make_response(err, result_cb<T>{std::move(cb)});
        }
        return _client->make_response(res, result_cb<T>{std::move(cb)});
    }, std::move(data), timeout);
}

class hotwallet : public base {
    public:
        hotwallet(client* c): base(c) {};
        ~hotwallet(){};
    
    public:
        bool balance(
            const std::string account,
            result_cb<hotwallet_balance_t>&& cb,
            const std::time_t timeout = 300L
        );
        bool charge(
            const std::string account,
            const std::string amount,
            result_cb<hotwallet_charge_t>&& cb,
            const std::time_t timeout = 300L
        );
        bool award(
            const std::string account,
            const std::string amount,
            result_cb<hotwallet_award_t>&& cb,
            const std::time_t timeout = 300L
        );
        bool get_allowance(
            const std::string account,
            result_cb<hotwallet_allowance_t>&& cb,
            const std::time_t timeout = 300L
        );
};

using hotwallet_ptr = std::shared_ptr<hotwallet>;

class nemoid : public base {
    public:
        nemoid(client* c): base(c) {};
        ~nemoid(){};
    public:
        bool login(
            const std::string code,
            const std::string code_verifier,
            const std::string redirect_uri,
            result_cb<nemoid_user_info_t>&& cb,
            const std::time_t timeout = 300L
        );
        bool relogin(
            const std::string refresh_token,
            const std::string code_verifier,
            result_cb<nemoid_user_info_t>&& cb,
            const std::time_t timeout = 300L            
        );
        bool user_info(
            const std::string access_token,
            result_cb<nemoid_user_info_t>&& cb,
            const std::time_t timeout = 300L
        );
};

using nemoid_ptr = std::shared_ptr<nemoid>;

class nft : public base {
    public:
        nft(client* c): base(c) {};
        ~nft(){};
    public:
        bool mint(
            const std::string recipient,
            const rapidjson::Document::Object metadata,
            const std::string callback,
            result_cb<nft_mint_t>&& cb,
            const std::time_t timeout = 300L            
        );
        bool request_mint(
            const std::string recipient,
            const rapidjson::Document::Object metadata,
            const std::string callback,
            result_cb<nft_request_mint_t>&& cb,
            const std::time_t timeout = 300L            
        );
};

using nft_ptr = std::shared_ptr<nft>;

} //namespace rest
} //namespace nemoapi

#endif