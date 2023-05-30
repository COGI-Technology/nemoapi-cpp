#include "nemoapi.h"

namespace Nemo
{
MysteryboxApi::~MysteryboxApi() {
    delete client_;
    client_ = nullptr;
}

string MysteryboxApi::mint(
    const char* box_id,
    const char* recipient,
    rapidjson::Document::Object metadata,
    const char* callback,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    rapidjson::Value _recipient(recipient, allocator);
    params.AddMember("recipient", _recipient, allocator);
    rapidjson::Value _box_id(box_id, allocator);
    params.AddMember("boxid", _box_id, allocator);
    rapidjson::Value _callback(callback, allocator);
    params.AddMember("callback", _callback, allocator);
    params.AddMember("data", metadata, allocator);
    
    size_t data_size;
    unique_ptr<uint8_t[]> data(json_decode(params, &data_size));

    uint8_t resource_path[] = "/mysterybox/mint";
    size_t path_size = 16;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data.get(),
                data_size,
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data.get(),
            data_size,
            signature.get(),
            timeout,
            argv,
            argc
        );
        return res["uuid"].GetString();
    } catch (const std::exception& e) {
        throw;
    }
}

rapidjson::Document MysteryboxApi::build_batch_mint(
    const vector<const char*> box_ids,
    const vector<const char*> recipients,
    const vector<rapidjson::Document::Object> metadatas,
    const vector<const char*> callbacks
) {
    rapidjson::Document ret(rapidjson::kArrayType);
    auto& allocator = ret.GetAllocator();

    for(auto i = 0; i < recipients.size(); i++) {
        rapidjson::Value e(rapidjson::kObjectType);

        rapidjson::Value recipient(recipients[i], allocator);
        e.AddMember("recipient", recipient, allocator);
        rapidjson::Value boxid(box_ids[i], allocator);
        e.AddMember("boxid", boxid, allocator);
        rapidjson::Value callback(callbacks[i], allocator);
        e.AddMember("callback", callback, allocator);
        e.AddMember("data", metadatas[i], allocator);

        ret.PushBack(e, allocator);
    }

    return ret;
}

rapidjson::Document::Array MysteryboxApi::mints(
    rapidjson::Document::Array boxes,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document params(rapidjson::kObjectType);
    auto& allocator = params.GetAllocator();

    params.AddMember("boxs", boxes, allocator);

    size_t data_size;
    unique_ptr<uint8_t[]> data(json_decode(params, &data_size));

    uint8_t resource_path[] = "/mysterybox/mints";
    size_t path_size = 17;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data.get(),
                data_size,
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data.get(),
            data_size,
            signature.get(),
            timeout,
            argv,
            argc
        );
        return res["uuid"].GetArray();
    } catch (const std::exception& e) {
        throw;
    }
}
} // namespace Nemo
