#include <nemoapi/client.h>

namespace nemoapi {
namespace rest {

client::client(const std::string endpoint, eddsa* dsa, const std::string access_key_id)
    :m_endpoint(endpoint)
    ,m_dsa(dsa)
    ,m_access_key_id(access_key_id)
{}

client::~client() {
    delete m_dsa;
}

v2_signature_t* client::sign(const std::string resource_path, const std::string body, const std::time_t access_time) {
    std::string prefix = m_endpoint.substr(m_endpoint.find_last_of("/"), m_endpoint.length());
    std::string access_id = std::move(prefix) + resource_path;
    std::string access_time_s = std::to_string(access_time);
    std::string message = std::move(access_id) + ":" + access_time_s + ":" + body;

    size_t s_size = eddsa_S_SIZE;
    uint8_t* s = new uint8_t[s_size];
    bool done = m_dsa->sign(reinterpret_cast<const uint8_t*>(message.c_str()), message.length(), s, s_size);
    if(!done){
        delete[] s;
        return nullptr;
    }

    v2_signature_t* ret = new v2_signature_t{std::move(access_time_s), std::move(base64_encode(s, s_size))};

    delete[] s;
    return ret;
}

} //namesapce rest
} //namespace nemoapi
