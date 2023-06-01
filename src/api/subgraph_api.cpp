#include "nemoapi.h"

namespace Nemo
{
SubgraphApi::~SubgraphApi() {
    delete client_;
    client_ = nullptr;
}

rapidjson::Document::Object SubgraphApi::call(
    rapidjson::Document::Object params,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document doc(rapidjson::kObjectType);
    auto& allocator = doc.GetAllocator();
    rapidjson::Value val(params);
    doc.CopyFrom(val, allocator);

    size_t data_size;
    uint8_t* data = json_decode(doc, &data_size);

    uint8_t resource_path[] = "/subgraph/call";
    size_t path_size = 14;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data,
                data_size,
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data,
            data_size,
            signature.get(),
            timeout,
            argv,
            argc
        );
        delete[] data;
        return res.GetObject();
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
}

rapidjson::Document::Object SubgraphApi::get_total_volume(
    rapidjson::Document::Object params,
    void* argv[],
    size_t argc,
    long timeout
) {
    rapidjson::Document doc(rapidjson::kObjectType);
    auto& allocator = doc.GetAllocator();
    rapidjson::Value val(params);
    doc.CopyFrom(val, allocator);
    
    size_t data_size;
    uint8_t* data = json_decode(doc, &data_size);

    uint8_t resource_path[] = "/subgraph/getTotalVolume";
    size_t path_size = 24;
    
    try {
        unique_ptr<APIV2Signed> signature(
            client_->sign(
                resource_path,
                path_size,
                data,
                data_size,
                timestamp()
            )
        );

        auto res = client_->call_api(
            resource_path,
            path_size,
            NEMOAPI_POST,
            NemoApiV2Auth,
            data,
            data_size,
            signature.get(),
            timeout,
            argv,
            argc
        );
        delete[] data;
        return res.GetObject();
    } catch (const std::exception& e) {
        delete[] data;
        throw;
    }
}
} // namespace Nemo
