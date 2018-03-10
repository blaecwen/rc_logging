#include <iostream>

#include "discoveryserver.h"
#include "localserver.h"
#include "mongomanager.h"
#include <optionsparser.h>

#include <memory>
#include <boost/program_options.hpp>

#include <systemd/sd-daemon.h>

namespace po = boost::program_options;


int main(int argc, char* argv[]) {
    try {
        po::variables_map vm;
        rclog::OptionsParser op;
        int ec = op.parse(argc, argv, vm);
        if (ec > 0)
            return ec;
        if (ec < 0)
            return 0;

        std::cerr << rclog::version_message << std::endl;

        std::shared_ptr<rclog::DatabaseManager> dbManager;
        if (vm["dbms"].as<std::string>() == "mongodb") {
            dbManager = std::make_shared<rclog::MongoManager>(vm["id"].as<std::string>());
        }
        else {
            std::cerr << "Unknown database specified, valid values: mongodb\n";
            return 1;
        }

        std::string socket = vm["socket"].as<std::string>();
        if(vm["socket-takeover"].as<bool>()) {
            std::cerr << "Warning: --socket-takeover option was specified. Taking over socket file '"
                      << socket << "'. This file could be used by other process." << "\n";
            ::unlink(socket.c_str());           // NOTE: is it correct behavior?
        }
        boost::asio::io_service io_service;
        rclog::LocalServer s(io_service, socket, *dbManager);

        rclog::DiscoveryServer::params p;
        p.multicast_address = vm["multicast-address"].as<std::string>();
        p.multicast_port = vm["multicast-port"].as<int>();
        p.node_id = vm["id"].as<std::string>();
        p.priority = vm["priority"].as<int>();
        p.interval = vm["hello-interval"].as<int>();
        p.enable_loopback = vm["enable-loopback"].as<bool>();
        rclog::DiscoveryServer d(io_service, p);

        sd_notifyf(0, "READY=1\n"
                      "STATUS=Processing requests...\n"
                      "MAINPID=%lu",
                      (unsigned long) getpid());

        io_service.run();

    }
    catch (po::error& e) {
        std::cerr << "Error parsing command line or config file: " << e.what() << "\n";
        std::cerr << "try '" << argv[0] << " --help' for more information" << "\n";
        return 1;
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
