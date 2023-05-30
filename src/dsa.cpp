#include "nemoapi.h"

namespace Nemo
{
EDDSA::EDDSA(const uint8_t* prv, const uint8_t* pub):
    prv_(make_unique<uint8_t[]>(PRV_SIZE))
    ,pub_(make_unique<uint8_t[]>(PUB_SIZE))
    ,sk_(make_unique<uint8_t[]>(SK_SIZE))
{
    if(prv != nullptr){
        memcpy(prv_.get(), prv, PRV_SIZE);
        crypto_sign_seed_keypair(pub_.get(), sk_.get(), prv_.get());
        return;
    }

    if(pub != nullptr){
        memcpy(pub_.get(), pub, PUB_SIZE);
    }

    _assert(prv != nullptr || pub != nullptr, "Invalid prv, pub");
}

EDDSA* EDDSA::from_prv(const uint8_t* prv){
    return new EDDSA(prv);
}

EDDSA* EDDSA::from_pub(const uint8_t* pub){
    return new EDDSA(nullptr, pub);
}

string EDDSA::prv_as_base64(){
    return base64_encode(prv_.get(), PRV_SIZE);
}

string EDDSA::pub_as_base64(){
    return base64_encode(pub_.get(), PUB_SIZE);
}

void EDDSA::sign(const uint8_t* m, size_t m_len, uint8_t* s, size_t* s_len){
    *s_len = S_SIZE;
    if (s == nullptr)
        return;

    unique_ptr<uint8_t[]> mh = make_unique<uint8_t[]>(MH_SIZE);
    
    _assert(crypto_hash_sha256(
        mh.get(),
        (const unsigned char *) m,
        m_len) >= 0,
        "Failed crypto_hash_sha256"
    );
        
    unsigned long long sm_size = S_SIZE + MH_SIZE;
    unique_ptr<uint8_t[]> sm = make_unique<uint8_t[]>(sm_size);

    _assert(crypto_sign(sm.get(), &sm_size, mh.get(), (unsigned long long)MH_SIZE, sk_.get()) >= 0, "Failed crypto_sign");

    s = (uint8_t*)realloc(s, S_SIZE);
    memcpy(s, sm.get(), S_SIZE);
}

bool EDDSA::verify(const uint8_t* m, size_t m_len, const uint8_t* s, size_t s_len){
    if (s_len != S_SIZE)
        return false;
    unsigned long long mh_size = MH_SIZE;
    unique_ptr<uint8_t[]> mh = make_unique<uint8_t[]>(MH_SIZE);
    _assert(crypto_hash_sha256(mh.get(), m, (unsigned long long)m_len) > 0, "Failed crypto_hash_sha256");
    unique_ptr<uint8_t[]> sm = make_unique<uint8_t[]>(S_SIZE + mh_size);
    memcpy(sm.get(), s, S_SIZE);
    memcpy(sm.get()+S_SIZE, mh.get(), m_len);
    bool ret = crypto_sign_open(mh.get(), &mh_size, sm.get(), S_SIZE + MH_SIZE, pub_.get()) > 0;
    return ret;
}

EDDSA* EDDSA::generate(){
    unique_ptr<uint8_t[]> buf = make_unique<uint8_t[]>(PRV_SIZE);
    randombytes_buf(buf.get(), PRV_SIZE);
    return new EDDSA(buf.get());
}
}
