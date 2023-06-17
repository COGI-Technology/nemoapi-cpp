#include <nemoapi/nemoapi.h>

using namespace nemoapi;

bool mint_nft(rest::nft_ptr client) {
    const std::string account("0x48b1747b7221c894f1548740435d5d54377e422d");
    const std::string metadata_in_str("{\"description\":\"hero_501_name\",\"image\":\"https://n69.gsscdn.com/icons/hero/501.png\",\"name\":\"Maral\",\"attributes\":[{\"trait_type\":\"ID\",\"value\":\"02e896e7-7bed-4c3b-8000-230fc7777892\"},{\"trait_type\":\"NftType\",\"value\":\"Hero\"},{\"display_type\":\"number\",\"trait_type\":\"HeroId\",\"value\":501},{\"display_type\":\"number\",\"trait_type\":\"MintId\",\"value\":50010000},{\"trait_type\":\"Avatar\",\"value\":\"https://n69.gsscdn.com/icons/hero/501.png\"},{\"trait_type\":\"Tittle\",\"value\":\"Blitzkriegexpert.Heprovidesbonusfor[d80c0cFF]Tanks[-]inbattle.\"},{\"trait_type\":\"Des\",\"value\":\"Bóngđêmkinhhoàng\"},{\"trait_type\":\"Rarity\",\"value\":5},{\"display_type\":\"number\",\"trait_type\":\"Level\",\"value\":1},{\"display_type\":\"number\",\"trait_type\":\"Star\",\"value\":1},{\"display_type\":\"number\",\"trait_type\":\"Grade\",\"value\":1},{\"display_type\":\"number\",\"trait_type\":\"Power\",\"value\":1204.165092468262},{\"display_type\":\"number\",\"trait_type\":\"Growth\",\"value\":90},{\"display_type\":\"number\",\"trait_type\":\"Morale\",\"value\":109.0899963378906},{\"display_type\":\"number\",\"trait_type\":\"Physique\",\"value\":109.0899963378906},{\"display_type\":\"number\",\"trait_type\":\"Support\",\"value\":361.9688110351562},{\"display_type\":\"number\",\"trait_type\":\"Develop\",\"value\":120.6563034057617},{\"trait_type\":\"Type\",\"value\":\"Cyborg\"},{\"display_type\":\"number\",\"trait_type\":\"BattlePoint\",\"value\":50.0},{\"display_type\":\"number\",\"trait_type\":\"ATK\",\"value\":450.0},{\"display_type\":\"number\",\"trait_type\":\"Range\",\"value\":3.50},{\"display_type\":\"number\",\"trait_type\":\"Units\",\"value\":1},{\"display_type\":\"number\",\"trait_type\":\"HP\",\"value\":9600.0},{\"display_type\":\"number\",\"trait_type\":\"Speed\",\"value\":2.50},{\"trait_type\":\"Ability\",\"value\":\"Thisunitdoesn'thaveanyspecialability.\"},{\"trait_type\":\"PassiveSkill\",\"value\":\"[\\n{\\n\\\"id\\\":100495,\\n\\\"level\\\":1,\\n\\\"name\\\":\\\"TankDEF\\\"\\n}\\n]\\n\"},{\"trait_type\":\"PVESkill\",\"value\":\"[\\n{\\n\\\"level\\\":1,\\n\\\"name\\\":\\\"HealIV\\\"\\n}\\n]\\n\"},{\"trait_type\":\"PVPSkill\",\"value\":\"[\\n{\\n\\\"level\\\":1,\\n\\\"name\\\":\\\"HealingV\\\"\\n}\\n]\\n\"},{\"display_type\":\"number\",\"trait_type\":\"CardLevel\",\"value\":1}]}");
    const std::string callback("http://127.0.0.1:7888/nft/callback/v1?item_id=itemidtest&sign=testsign");

    rapidjson::Document metadata;
    metadata.Parse(metadata_in_str.data());
    return client->mint(std::move(account), metadata.GetObject(), std::move(callback), [](status_e status, rest::err_t&& err, rest::nft_mint_t&& res){
        if (status != normal) {
            LOG_ERROR << "mint error: status=" << status << ", err=" << err;
            return false;
        }
        LOG_DEBUG << res;
        return true;
    });
}