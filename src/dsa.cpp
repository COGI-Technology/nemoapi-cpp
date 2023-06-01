#include "nemoapi.h"

namespace Nemo
{
EDDSA::EDDSA(const uint8_t* prv, const uint8_t* pub):
    prv_(new uint8_t[PRV_SIZE])
    ,pub_(new uint8_t[PUB_SIZE])
    ,sk_(new uint8_t[SK_SIZE])
{
    if(prv != nullptr){
        memcpy(prv_, prv, PRV_SIZE);
        crypto_sign_seed_keypair(pub_, sk_, prv_);
        return;
    }

    if(pub != nullptr){
        memcpy(pub_, pub, PUB_SIZE);
    }

    _assert(prv != nullptr || pub != nullptr, "Invalid prv, pub");
}

EDDSA::~EDDSA() {
    delete[] prv_;
    delete[] pub_;
    delete[] sk_;

    prv_ = nullptr;
    pub_ = nullptr;
    sk_ = nullptr;
}

EDDSA* EDDSA::from_prv(const uint8_t* prv){
    return new EDDSA(prv);
}

EDDSA* EDDSA::from_pub(const uint8_t* pub){
    return new EDDSA(nullptr, pub);
}

string EDDSA::prv_as_base64(){
    return base64_encode(prv_, PRV_SIZE);
}

string EDDSA::pub_as_base64(){
    return base64_encode(pub_, PUB_SIZE);
}

void EDDSA::sign(const uint8_t* m, size_t m_len, uint8_t* s, size_t* s_len){
    *s_len = S_SIZE;
    if (s == nullptr)
        return;

    uint8_t* mh = new uint8_t[MH_SIZE];

    if (crypto_hash_sha256(mh, (const unsigned char *) m, m_len) < 0) {
        delete[] mh;
        _assert(false, "Failed crypto_hash_sha256");
    }
        
    unsigned long long sm_size = S_SIZE + MH_SIZE;
    uint8_t* sm = new uint8_t[sm_size];

    if (crypto_sign(sm, &sm_size, mh, (unsigned long long)MH_SIZE, sk_) < 0) {
        delete[] mh;
        delete[] sm;
        _assert(false, "Failed crypto_sign");
    }

    s = (uint8_t*)realloc(s, S_SIZE);
    memcpy(s, sm, S_SIZE);
    delete[] mh;
    delete[] sm;
}

bool EDDSA::verify(const uint8_t* m, size_t m_len, const uint8_t* s, size_t s_len){
    if (s_len != S_SIZE)
        return false;
    unsigned long long mh_size = MH_SIZE;
    uint8_t* mh = new uint8_t[MH_SIZE];
    if (crypto_hash_sha256(mh, m, (unsigned long long)m_len) < 0) {
        delete[] mh;
        _assert(false, "Failed crypto_hash_sha256");
    }
    uint8_t* sm = new uint8_t[S_SIZE + mh_size];
    memcpy(sm, s, S_SIZE);
    memcpy(sm+S_SIZE, mh, m_len);
    bool ret = crypto_sign_open(mh, &mh_size, sm, S_SIZE + MH_SIZE, pub_) > 0;
    delete[] mh;
    delete[] sm;
    return ret;
}

EDDSA* EDDSA::generate(){
    uint8_t* buf = new uint8_t[PRV_SIZE];
    randombytes_buf(buf, PRV_SIZE);
    EDDSA* ret = EDDSA::from_prv(buf);
    delete[] buf;
    return ret;
}
}
