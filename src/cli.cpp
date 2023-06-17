#include <nemoapi/nemoapi.h>
#include <iostream>
#include <string>

using namespace nemoapi;

void generate() {
    eddsa* dsa = eddsa::generate();
    std::cout << "Private Key: " << dsa->prv_as_base64().c_str() << std::endl;
    std::cout << "Public key: " << dsa->pub_as_base64().c_str() << std::endl;
    delete dsa;
}

void sign(std::string prv, std::string data) {
    eddsa* dsa = eddsa::from_prv(reinterpret_cast<const uint8_t*>(base64_decode(prv).c_str()));
    uint8_t* ret = new uint8_t[eddsa_S_SIZE];
    bool done = dsa->sign(
        reinterpret_cast<const uint8_t*>(data.c_str()),
        data.length(),
        ret,
        eddsa_S_SIZE
    );
    if(done){
        std::cout << "Signature: " << base64_encode(ret, eddsa_S_SIZE) << std::endl;
    }
    delete[] ret;
    delete dsa;
}

void verify(std::string prv, std::string data, std::string enc) {
    eddsa* dsa = eddsa::from_prv(reinterpret_cast<const uint8_t*>(base64_decode(prv).c_str()));
    std::string s = base64_decode(enc);
    bool ret = dsa->verify(
        reinterpret_cast<const uint8_t*>(data.c_str()),
        data.length(),
        reinterpret_cast<const uint8_t*>(s.c_str()),
        s.length()
    );
    if(ret){
        std::cout << "Signature valid" << std::endl;
    }else{
        std::cout << "Signature invalid" << std::endl;
    }
    delete dsa;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <command>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "help") {
        std::cout << "Usage" << std::endl;
        std::cout << "- generate" << std::endl;
        std::cout << "- verify" << std::endl;
        std::cout << "- version" << std::endl;
        std::cout << "- help" << std::endl;
    } else if (command == "version") {
        std::cout << argv[0] << " Version " << CLI_VERSION_MAJOR << "."
            << CLI_VERSION_MINOR << "." << CLI_VERSION_PATCH << std::endl;
    } else if (command == "generate") {
        generate();
    } else if (command == "sign") {
        if(argc < 4){
            std::cout << "sign <private key as base64> <message>" << std::endl;
            return 1;
        }
        sign(argv[2], argv[3]);
    } else if (command == "verify") {
        if(argc < 5){
            std::cout << "verify <private key as base64> <message> <signature as base64>" << std::endl;
            return 1;
        }
        verify(argv[2], argv[3], argv[4]);
    } else {
        std::cout << "Unknown command. Type 'help' for available commands." << std::endl;
    }

    return 0;
}