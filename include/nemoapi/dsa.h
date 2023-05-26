#ifndef DSA_H
#define DSA_H

#include "utils.h"
#define PRV_SIZE    crypto_sign_ed25519_SEEDBYTES
#define PUB_SIZE    crypto_sign_ed25519_PUBLICKEYBYTES
#define SK_SIZE     crypto_sign_ed25519_SECRETKEYBYTES
#define S_SIZE      crypto_sign_ed25519_BYTES
#define MH_SIZE     crypto_hash_sha256_BYTES

using namespace std;
class EDDSA{
    public:
        EDDSA(struct nemoapi_memory* prv, struct nemoapi_memory* pub);
        ~EDDSA();
    
    public:
        static EDDSA* from_prv(struct nemoapi_memory* prv);
        static EDDSA* from_pub(struct nemoapi_memory* pub);
        string prv_as_base64();
        string pub_as_base64();
        struct nemoapi_memory* sign(const struct nemoapi_memory* m);
        bool verify(const struct nemoapi_memory* m, const struct nemoapi_memory* s);
        static EDDSA* generate();
    
    private:
        struct nemoapi_memory* _prv;
        struct nemoapi_memory* _pub;
        struct nemoapi_memory* _sk;
};

#endif