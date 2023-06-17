#ifndef __nemoapi__enum_h
#define __nemoapi__enum_h

namespace nemoapi
{

enum transaction_e {
    internal = 207
    , onchain = 203
    , offchain = 202
    , withdraw = 201
    , deposit = 200
};

enum status_e {
    normal = 1
    , queue = 0
    , input_invalid = -1
    , something_wrong = -500
};

} //namespace nemoapi

#endif