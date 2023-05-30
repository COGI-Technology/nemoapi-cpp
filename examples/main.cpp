#include "nemoapi.h"
#include "utils.cpp"
#include "mint_nft.cpp"
#include "hotwallet_award.cpp"

using namespace Nemo;

int main() {
    NFTApi* nftc = new NFTApi(mock_client(
        "0x15f2ab2fa819a298",
        "http://127.0.0.1:35555/api/v2",
        "auVgK8gSvFOgF5zWmQ5wWhFKImyl5/ka59dcRZtzcDA=",
        "oB13FXaa1BiEiDaUGvuj/blJwj6SRl7JjkE/ApeQf08=",
        "nemoapi-cpp-demo"
    ));
    try
    {
        mint_nft(nftc);
    }
    catch(const std::exception& e)
    {
        printf("mint nft failed cause: %s\n", e.what());
    }
    
    delete nftc;

    HotwalletApi* hotwalletc = new HotwalletApi(mock_client(
        "0x15f2ab2fa819a291",
        "http://127.0.0.1:5555/api/v2",
        "auVgK8gSvFOgF5zWmQ5wWhFKImyl5/ka59dcRZtzcDA=",
        "oB13FXaa1BiEiDaUGvuj/blJwj6SRl7JjkE/ApeQf08=",
        "nemoapi-cpp-demo"
    ));
    
    try
    {
        hotwallet_award(hotwalletc);
    }
    catch(const std::exception& e)
    {
        printf("hotwallet award failed cause: %s\n", e.what());
    }
    
    delete hotwalletc;

    return 0;
}