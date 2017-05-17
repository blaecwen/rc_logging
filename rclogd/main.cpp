#include <iostream>

#include "localserver.h"
#include "mongomanager.h"

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: localserver <socket_path>\n";
            return 1;
        }

        rclog::MongoManager dbManager;

        ::unlink(argv[1]);     // NOTE: is it correct behavior?
        boost::asio::io_service io_service;
        rclog::LocalServer s(io_service, argv[1], dbManager);
        io_service.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
