#include "nemoapi.h"

int main() {
  EDDSA* dsa = EDDSA::generate();
  printf("Private Key: %s\n", dsa->prv_as_base64().c_str());
  printf("Public key: %s\n", dsa->pub_as_base64().c_str());

  struct nemoapi_memory* prv = base64_decode(nemoapi_memory_init("oB13FXaa1BiEiDaUGvuj/blJwj6SRl7JjkE/ApeQf08="));
  EDDSA* dsa2 = EDDSA::from_prv(prv);
  printf("Private Key: %s\n", dsa2->prv_as_base64().c_str());
  printf("Public key: %s\n", dsa2->pub_as_base64().c_str());

  // // HttpBasicAuth* a = new HttpBasicAuth((unsigned char*)"usernaem", (unsigned char*)"password");
  // // struct curl_slist* headers = nullptr;
  // // a->append_to_headers(&headers);
  // // print_headers(headers);
  // // curl_slist_free_all(headers);
  // // return 0;

  // Initialize curl
  curl_global_init(CURL_GLOBAL_ALL);
  
  APIConfiguration* config = new APIConfiguration;
  config->host = nemoapi_memory_init("http://127.0.0.1:5555/api/v2");
  config->key_id = nemoapi_memory_init("0x15f2ab2fa8191238");
  config->private_key = base64_decode(nemoapi_memory_init(dsa2->prv_as_base64().c_str()));
  config->public_key = base64_decode(nemoapi_memory_init(dsa2->pub_as_base64().c_str()));

  printf("Start\n");
  APIClient* client = new APIClient(config);

  struct nemoapi_memory* resource_path = nemoapi_memory_init("/hotwallet/balance");
  struct nemoapi_memory* body = nemoapi_memory_init("{\"account\":\"0x48b1747b7221c894f1548740435d5d54377e422d\"}");

  uint8_t auth_settings = NemoApiV2Auth | NemoApiKeyAuth;
  client->call_api(
    resource_path,
    NEMOAPI_POST,
    auth_settings,
    nullptr,
    nullptr,
    body,
    nullptr
  );
  free(resource_path);
  free(body);
  delete dsa;
  delete dsa2;
  delete client;
  curl_global_cleanup();
}
/*
// g++ -o keypair keypair.cpp dsa.cpp -lsodium -lpthread -static
g++ -I ./include/ -I /opt/vcpkg/installed/x64-linux/include/ \
-o keypair keypair.cpp ./src/dsa.cpp ./src/api/*.cpp \
-lsodium -lpthread -L/opt/vcpkg/installed/x64-linux/lib/ -lcurl -lssl -lcrypto -pthread -lz
*/