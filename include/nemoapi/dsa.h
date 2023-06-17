#ifndef __nemoapi__dsa_h
#define __nemoapi__dsa_h

#include "utils.h"
#include <memory>

#define eddsa_PRV_SIZE    crypto_sign_ed25519_SEEDBYTES
#define eddsa_PUB_SIZE    crypto_sign_ed25519_PUBLICKEYBYTES
#define eddsa_SK_SIZE     crypto_sign_ed25519_SECRETKEYBYTES
#define eddsa_S_SIZE      crypto_sign_ed25519_BYTES
#define eddsa_MH_SIZE     crypto_hash_sha256_BYTES

namespace nemoapi
{
class eddsa{
    public:
        eddsa(const uint8_t* prv = nullptr, const uint8_t* pub = nullptr);
        virtual ~eddsa();
    
    public:
        static eddsa* from_prv(const uint8_t* prv);
        static eddsa* from_pub(const uint8_t* pub);
        std::string prv_as_base64();
        std::string pub_as_base64();
        bool sign(const uint8_t* m, size_t m_len, uint8_t* s, size_t s_len);
        bool verify(const uint8_t* m, size_t m_len, const uint8_t* s, size_t s_len);
        static eddsa* generate();
    
    private:
        uint8_t* _prv;
        uint8_t* _pub;
        uint8_t* _sk;
};

} //namespace nemoapi

#endif