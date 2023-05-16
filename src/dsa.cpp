#include "nemoapi.h"

EDDSA::EDDSA(struct nemoapi_memory* prv = nullptr, struct nemoapi_memory* pub = nullptr):
    _prv(new nemoapi_memory)
    ,_pub(new nemoapi_memory)
    ,_sk(new nemoapi_memory)
{
    _prv->data = new uint8_t[PRV_SIZE];
    _prv->length = PRV_SIZE;
    
    _pub->data = new uint8_t[PUB_SIZE];
    _pub->length = PUB_SIZE;

    _sk->data = new uint8_t[SK_SIZE];
    _sk->length = SK_SIZE;

    if(prv != nullptr){
        _prv = prv;
        crypto_sign_seed_keypair(_pub->data, _sk->data, _prv->data);
        return;
    }

    if(pub != nullptr){
        _pub = pub;
    }

    _assert(prv != nullptr || pub != nullptr, "Invalid prv, pub");
}

EDDSA::~EDDSA(){
    free(_prv);
    free(_pub);
    free(_sk);
    _prv = nullptr;
    _pub = nullptr;
    _sk = nullptr;
};

EDDSA* EDDSA::from_prv(struct nemoapi_memory* prv){
    return new EDDSA(prv);
}

EDDSA* EDDSA::from_pub(struct nemoapi_memory* pub){
    return new EDDSA(nullptr, pub);
}

string EDDSA::prv_as_base64(){
    return base64_encode(_prv);
}

string EDDSA::pub_as_base64(){
    return base64_encode(_pub);
}

struct nemoapi_memory* EDDSA::sign(const struct nemoapi_memory* m){
    uint8_t* mh = new uint8_t[MH_SIZE];
    unsigned char out[crypto_hash_sha256_BYTES];
    _assert(crypto_hash_sha256(
        mh,
        (const unsigned char *) m->data,
        m->length) > 0,
        "Failed crypto_hash_sha256"
    );
        
    unsigned long long sm_size = S_SIZE + MH_SIZE;
    uint8_t* sm = new uint8_t[sm_size];
    _assert(crypto_sign(sm, &sm_size, mh, (unsigned long long)MH_SIZE, _sk->data) > 0, "Failed crypto_sign");
    struct nemoapi_memory* ret = new nemoapi_memory;
    ret->data = new uint8_t[S_SIZE];
    ret->length = S_SIZE;
    memcpy(ret->data, sm, S_SIZE);
    delete[] sm;
    delete[] mh;
    return ret;
}

bool EDDSA::verify(const struct nemoapi_memory* m, const struct nemoapi_memory* s){
    if (s->length != S_SIZE)
        return false;
    unsigned long long mh_size = MH_SIZE;
    uint8_t* mh = new uint8_t[mh_size];
    _assert(crypto_hash_sha256(mh, reinterpret_cast<const uint8_t *>(m->data), (unsigned long long)m->length) > 0, "Failed crypto_hash_sha256");
    uint8_t* sm = new uint8_t[S_SIZE + mh_size];
    memcpy(sm, s, S_SIZE);
    memcpy(sm+S_SIZE, mh, m->length);
    bool ret = crypto_sign_open(mh, &mh_size, sm, S_SIZE + MH_SIZE, _pub->data) > 0;
    delete[] sm;
    delete[] mh;
    return ret;
}

EDDSA* EDDSA::generate(){
    uint8_t* buf = new uint8_t[PRV_SIZE];
    randombytes_buf(buf, PRV_SIZE);
    struct nemoapi_memory* prv = new nemoapi_memory;
    prv->data = buf;
    prv->length = PRV_SIZE;
    return new EDDSA(prv);
}