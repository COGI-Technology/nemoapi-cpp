#include "nemoapi.h"
#include "cli_config.h"
#include <iostream>
#include <string>

using namespace std;

void generate() {
    Nemo::EDDSA* dsa = Nemo::EDDSA::generate();
    printf("Private Key: %s\n", dsa->prv_as_base64().c_str());
    printf("Public key: %s\n", dsa->pub_as_base64().c_str());
    delete dsa;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <command>" << endl;
        return 1;
    }

    string command = argv[1];

    if (command == "help") {
        cout << "Usage" << endl;
        cout << "- generate" << endl;
        cout << "- help" << endl;
    } else if (command == "version") {
        cout << argv[0] << " Version " << CLI_VERSION_MAJOR << "."
              << CLI_VERSION_MINOR << "." << CLI_VERSION_PATCH << endl;
    } else if (command == "generate") {
        generate();
    } else {
        cout << "Unknown command. Type 'help' for available commands." << endl;
    }

    return 0;
}