#include <nemoapi/dsa.h>
#include <nemoapi/logging.h>

namespace nemoapi
{
eddsa::eddsa(const uint8_t* prv, const uint8_t* pub):
    _prv(new uint8_t[eddsa_PRV_SIZE])
    ,_pub(new uint8_t[eddsa_PUB_SIZE])
    ,_sk(new uint8_t[eddsa_SK_SIZE])
{
    if(prv != nullptr){
        memmove(_prv, prv, eddsa_PRV_SIZE);
        crypto_sign_seed_keypair(_pub, _sk, _prv);
        return;
    }

    if(pub != nullptr){
        memmove(_pub, pub, eddsa_PUB_SIZE);
    }

    if(prv != nullptr || pub != nullptr){
        LOG_FATAL << "Invalid prv, pub";
    }
}

eddsa::~eddsa() {
    delete[] _prv;
    delete[] _pub;
    delete[] _sk;
}

eddsa* eddsa::from_prv(const uint8_t* prv){
    return new eddsa(prv);
}

eddsa* eddsa::from_pub(const uint8_t* pub){
    return new eddsa(nullptr, pub);
}

std::string eddsa::prv_as_base64(){
    return base64_encode(_prv, eddsa_PRV_SIZE);
}

std::string eddsa::pub_as_base64(){
    return base64_encode(_pub, eddsa_PUB_SIZE);
}

bool eddsa::sign(const uint8_t* m, size_t m_len, uint8_t* s, size_t s_len){
    if (s_len != eddsa_S_SIZE){
        LOG_ERROR << "Failed s_len:" << s_len;
        return false;
    }
    uint8_t* mh = new uint8_t[eddsa_MH_SIZE];

    if (crypto_hash_sha256(mh, (const unsigned char *) m, m_len) < 0) {
        delete[] mh;
        LOG_ERROR << "Failed crypto_hash_sha256";
        return false;
    }

    auto s_len_p = (unsigned long long)s_len;
    if (crypto_sign_detached(s, &s_len_p, mh, (unsigned long long)eddsa_MH_SIZE, _sk) < 0) {
        delete[] mh;
        LOG_FATAL << "Failed crypto_sign";
        return false;
    }
    delete[] mh;
    return true;
}

bool eddsa::verify(const uint8_t* m, size_t m_len, const uint8_t* s, size_t s_len){
    if (s_len != eddsa_S_SIZE){
        LOG_ERROR << "Failed s_len:" << s_len;
        return false;
    }
    uint8_t* mh = new uint8_t[eddsa_MH_SIZE];
    if (crypto_hash_sha256(mh, m, (unsigned long long)m_len) < 0) {
        delete[] mh;
        LOG_ERROR << "Failed crypto_hash_sha256";
        return false;
    }

    bool ret = crypto_sign_verify_detached(s, mh, m_len, _pub) != 0;
    delete[] mh;
    return ret;
}

eddsa* eddsa::generate(){
    uint8_t* buf = new uint8_t[eddsa_PRV_SIZE];
    randombytes_buf(buf, eddsa_PRV_SIZE);
    eddsa* ret = eddsa::from_prv(buf);
    delete[] buf;
    return ret;
}
}
