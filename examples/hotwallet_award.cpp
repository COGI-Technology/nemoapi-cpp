#include <nemoapi/nemoapi.h>

using namespace nemoapi;

bool hotwallet_award(rest::hotwallet_ptr client) {
    const std::string account("0x48b1747b7221c894f1548740435d5d54377e422d");
    const std::string amount("1.1");

    return client->balance(std::move(account), [=](status_e status, rest::err_t&& err, rest::hotwallet_balance_t&& res){
        if (status != normal) {
            LOG_ERROR << "get balance error: status=" << status << ", err=" << err;
            return false;
        }
        LOG_DEBUG << res;
        return client->award(std::move(account), std::move(amount), [](status_e status, rest::err_t&& err, rest::hotwallet_award_t&& res){
            if(status != normal){
                LOG_ERROR << "award error: status=" << status << ", err=" << err;
            }
            LOG_DEBUG << res;
            return true;
        });
    });
}