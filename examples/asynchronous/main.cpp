#include "uv.h"
#include <nemoapi/nemoapi.h>
#include "../mint_nft.cpp"
#include "../hotwallet_award.cpp"

using namespace nemoapi;

rest::client_async* mock_client(std::string&& endpoint, std::string&& prv, std::string&& access_key_id, uv::http_client_pool_ptr pool) {
    std::string _prv = base64_decode(prv);
    eddsa* dsa = eddsa::from_prv(reinterpret_cast<const uint8_t*>(_prv.c_str()));
    return new rest::client_async(std::move(endpoint), dsa, std::move(access_key_id), pool);
}

int main(int argc, char* argv[]){
    int ret = static_cast<int>(curl_global_init(CURL_GLOBAL_ALL));
	if (ret != CURLE_OK) {
        LOG_FATAL << "failed curl_global_init";
		return ret;
	}

    uv_loop_t* loop = uv_default_loop();

    uv::http_client_pool_ptr pool = uv::http_client_pool_ptr(new uv::http_client_pool(256, loop));
    rest::nft_ptr nft = rest::nft_ptr(new rest::nft(mock_client(
        "https://api.nemoverse.io/richwork-testnet",
        "4FZi4TRkEqA4RQcboz/GbVH4z0mMNvgZ4ngYlSVj/8k=",
        "0xdf8b6b64e4f28ab",
        pool
    )));
    mint_nft(nft);

    rest::hotwallet_ptr hotwallet = rest::hotwallet_ptr(new rest::hotwallet(mock_client(
        "https://api.nemoverse.io/nemo-wallet-testnet",
        "m3ATI2w/aSgQr8id3E8eypVsBNDlwQ8PUg2uyANMElw=",
        "0x15f2ab2fa819a297",
        pool
    )));
    
    hotwallet_award(hotwallet);

    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    curl_global_cleanup();
    
    return 0;
}