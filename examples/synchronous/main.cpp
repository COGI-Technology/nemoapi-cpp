#include <nemoapi/nemoapi.h>
#include "../mint_nft.cpp"
#include "../hotwallet_award.cpp"

using namespace nemoapi;

rest::client_sync* mock_client(std::string&& endpoint, std::string&& prv, std::string&& access_key_id) {
    std::string _prv = base64_decode(prv);
    eddsa* dsa = eddsa::from_prv(reinterpret_cast<const uint8_t*>(_prv.c_str()));

    return new rest::client_sync(std::move(endpoint), dsa, std::move(access_key_id));
}

int main(int argc, char* argv[]){
    int ret = static_cast<int>(curl_global_init(CURL_GLOBAL_ALL));
	if (ret != CURLE_OK) {
        LOG_FATAL << "failed curl_global_init";
		return ret;
	}

    rest::nft_ptr nft = rest::nft_ptr(new rest::nft(mock_client(
        "https://api.nemoverse.io/richwork-testnet",
        "4FZi4TRkEqA4RQcboz/GbVH4z0mMNvgZ4ngYlSVj/8k=",
        "0xdf8b6b64e4f28ab"
    )));
    mint_nft(nft);

    rest::hotwallet_ptr hotwallet = rest::hotwallet_ptr(new rest::hotwallet(mock_client(
        "https://api.nemoverse.io/nemo-wallet-testnet",
        "m3ATI2w/aSgQr8id3E8eypVsBNDlwQ8PUg2uyANMElw=",
        "0x15f2ab2fa819a297"
    )));
    
    hotwallet_award(hotwallet);

    curl_global_cleanup();
    return 0;
}