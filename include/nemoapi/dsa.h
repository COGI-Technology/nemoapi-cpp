#ifndef DSA_H
#define DSA_H

#include "utils.h"
#define PRV_SIZE    crypto_sign_ed25519_SEEDBYTES
#define PUB_SIZE    crypto_sign_ed25519_PUBLICKEYBYTES
#define SK_SIZE     crypto_sign_ed25519_SECRETKEYBYTES
#define S_SIZE      crypto_sign_ed25519_BYTES
#define MH_SIZE     crypto_hash_sha256_BYTES

using namespace std;

namespace Nemo
{
class EDDSA{
    public:
        EDDSA(const uint8_t* prv = nullptr, const uint8_t* pub = nullptr);
        virtual ~EDDSA();
    
    public:
        static EDDSA* from_prv(const uint8_t* prv);
        static EDDSA* from_pub(const uint8_t* pub);
        string prv_as_base64();
        string pub_as_base64();
        void sign(const uint8_t* m, size_t m_len, uint8_t* s, size_t* s_len);
        bool verify(const uint8_t* m, size_t m_len, const uint8_t* s, size_t s_len);
        static EDDSA* generate();
    
    private:
        uint8_t* prv_;
        uint8_t* pub_;
        uint8_t* sk_;
};
} // namespace Nemo

#endif