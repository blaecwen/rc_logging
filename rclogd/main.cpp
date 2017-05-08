#include <iostream>

#include "localserver.h"

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: localserver <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        rclog::LocalServer s(io_service, std::atoi(argv[1]));
        std::cout << "Next will be run" << std::endl;
        io_service.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
