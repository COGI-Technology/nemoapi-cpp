#ifndef __nemoapi__client_h
#define __nemoapi__client_h

#include "utils.h"
#include "dsa.h"
#include "types.h"

#include <memory>

#define DEFAULT_USER_AGENT "NemoAPI/2.0.0/cpp"
#define ACCESS_TIME "Access-Time"
#define ACCESS_SIGNATURE "Access-Signature"
#define ACCESS_KEY_ID "Access-Key-Id"

namespace nemoapi {
namespace rest {

struct v2_signature_t {
    std::string access_time;
    std::string access_signature;
};

using request_cb = std::function<bool(const char*, const char*)>; //void(res, err)

class client {
    public:
        client(const std::string endpoint, eddsa* dsa, const std::string access_key_id);
        virtual ~client();

    public:
        virtual bool make_request(
            const std::string&& resource_path,
            request_cb&& cb,
            const std::string&& post_data = nullptr,
            const std::time_t timeout = 300L
        ) = 0;
        template<typename T>
        bool make_response(const char* raw, result_cb<T>&& cb);
        v2_signature_t* sign(
            const std::string resource_path,
            const std::string body,
            const std::time_t access_time
        );

    protected:
        std::string m_endpoint;
        eddsa* m_dsa;
        std::string m_access_key_id;
};

template<typename T>
bool client::make_response(const char* raw, result_cb<T>&& cb){
    if(std::strstr(raw, "<HTML>")
        || std::strstr(raw, "<HEAD>")
        || std::strstr(raw, "<BODY>")
    ){
        T res{};
        cb(something_wrong, std::move(err_t{raw}), std::move(res));
        return false;
    }
    rapidjson::Document response;
    response.Parse(raw);

    if(!response.IsObject()){
        T res{};
        cb(something_wrong, std::move(err_t{raw}), std::move(res));
        return false;
    }

    if (!response.HasMember("status")){
        T res{};
        cb(something_wrong, std::move(err_t{raw}), std::move(res));
        response.Clear();
        return false;
    }

    bool missing_params = !response.HasMember("params")
        || (response.HasMember("params") && !response["params"].IsObject());
    if(response["status"].GetInt() < 0){
        T res{};
        if(missing_params){
            cb(static_cast<nemoapi::status_e>(response["status"].GetInt()), std::move(err_t{raw}), std::move(res));
            return true;
        }else{
            auto response_params = response["params"].GetObject();
            if (response_params.HasMember("error")) {
                cb(something_wrong, std::move(err_t{response_params["error"].GetString()}), std::move(res));
                return true;
            }
            if (response_params.HasMember("err")) {
                cb(something_wrong, std::move(err_t{response_params["err"].GetString()}), std::move(res));
                return true;
            }
        }
    }

    if(missing_params){
        cb(normal, std::move(err_t{nullptr}), std::move(T::from_obj(response.GetObject())));
        return true;
    }
    if (response["params"].IsObject()) {
        cb(normal, std::move(err_t{nullptr}), std::move(T::from_obj(response["params"].GetObject())));
    } else {
        cb(normal, std::move(err_t{nullptr}), std::move(T::from_obj(response.GetObject())));
    }
    return true;
}

} //namespace rest
} //namespace nemoapi

#endif