#include "nemoapi.h"

using namespace Nemo;


void hotwallet_award(HotwalletApi* client) {
    const string account("0x48b1747b7221c894f1548740435d5d54377e422d");
    const string amount("1.1");

    auto res = client->balance(account.c_str());
    printf("before: %s\n", res.balance.c_str());
    InternalTransaction after = client->award(account.c_str(), amount.c_str());
    printf("tx hash: %s\nbalance: %s\n", after.tx_hash.c_str(), after.balance.c_str());
}