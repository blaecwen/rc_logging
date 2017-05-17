#include <iostream>

#include "localserver.h"
#include "mongomanager.h"

#include <memory>


int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: localserver <socket_path> <database>\n";
            return 1;
        }

        std::shared_ptr<rclog::DatabaseManager> dbManager;
        if (argv[2] == std::string("mongodb")) {
            dbManager = std::make_shared<rclog::MongoManager>();
        }
        else {
            std::cerr << "Unknown database, valid values: mongodb\n";
            return 1;
        }

        ::unlink(argv[1]);     // NOTE: is it correct behavior?
        boost::asio::io_service io_service;
        rclog::LocalServer s(io_service, argv[1], *dbManager);
        io_service.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
