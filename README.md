# nemoapi
NEMO API implemented in C++ for both synchronous and asynchronous way.

# REST API
- [NEMO ID login](//api/nemoid.md#login) -> `rest::nemoid:login()`
- [NEMO ID relogin](//api/nemoid.md#relogin) -> `rest::nemoid:relogin()`
- [NEMO ID user_info](//api/nemoid.md#user_info) -> `rest::nemoid:user_info()`
- [Hotwallet balance](//api/nemoid.md#relogin) -> `rest::nemoid:balance()`
- [Hotwallet charge](//api/nemoid.md#login) -> `rest::hotwallet:charge()`
- [Hotwallet award](//api/nemoid.md#relogin) -> `rest::hotwallet:award()`
- [NFT mint](//api/nft.md#mint) -> `rest::nft:mint()`
- [NFT request_mint](//api/nft.md#request_mint) -> `rest::nft:request_mint()`

# Examples
### Synchronous example
```cpp
#include <nemoapi/nemoapi.h>

using namespace nemoapi;

int main(int argc, char* argv[]){
    std::string prv = '...';
    std::string account = '...';

    curl_global_init(CURL_GLOBAL_ALL);
    eddsa dsa(prv);
    rest::client_sync client(endpoint, dsa, access_key_id);
    rest::nft nft(client);
    client.balance(account, [=](status_e status, rest::err_t&& err, rest::hotwallet_balance_t&& res){
        //
    });
    curl_global_cleanup();
    return 0;
}

```

### Asynchronous example
```cpp
#include <nemoapi/nemoapi.h>

using namespace nemoapi;

int main(int argc, char* argv[]){
    std::string prv = '...';
    std::string account = '...';

    curl_global_init(CURL_GLOBAL_ALL);
    uv_loop_t* loop = uv_default_loop();

    eddsa dsa(prv);
    uv::http_client_pool_ptr pool(new uv::http_client(256, loop));
    rest::client_async client(endpoint, dsa, access_key_id, pool);
    rest::nft nft(client);
    client.balance(account, [=](status_e status, rest::err_t&& err, rest::hotwallet_balance_t&& res){
        //
    });
    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);
    curl_global_cleanup();
    return 0;
}
```

### More examples
Please see the source code in [`examples`](examples).