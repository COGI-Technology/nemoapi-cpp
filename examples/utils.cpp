#include "nemoapi.h"

using namespace Nemo;

CURLClient* mock_client(
    string key_id,
    string host,
    string pub,
    string prv,
    string identification
) {
  unique_ptr<uint8_t[]> _k(to_bytes(key_id.c_str(), key_id.length()));

  unique_ptr<uint8_t[]> _host(to_bytes(host.c_str(), host.length()));

  unique_ptr<uint8_t[]> _id(to_bytes(identification.c_str(), identification.length()));

  size_t pub_size;
  base64_decode(pub.c_str(), pub.length(), nullptr, &pub_size);
  unique_ptr<uint8_t[]> _pub = make_unique<uint8_t[]>(pub_size);
  base64_decode(pub.c_str(), pub.length(), _pub.get(), &pub_size);

  size_t prv_size;
  base64_decode(prv.c_str(), prv.length(), nullptr, &prv_size);
  unique_ptr<uint8_t[]> _prv = make_unique<uint8_t[]>(prv_size);
  base64_decode(prv.c_str(), prv.length(), _prv.get(), &prv_size);

  CURLClient* ret = new CURLClient(
    _host.get(),
    host.length(),
    _k.get(),
    key_id.length(),
    _pub.get(),
    pub_size,
    _prv.get(),
    prv_size,
    nullptr,
    _id.get(),
    identification.length()
  );
  return ret;
}