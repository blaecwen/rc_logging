#include <iostream>
#include <fstream>

#include "discoveryserver.h"
#include "localserver.h"
#include "mongomanager.h"

#include <memory>
#include <boost/program_options.hpp>

#include <systemd/sd-daemon.h>

namespace po = boost::program_options;

const char help_message[] = "rclogd -- service managing logs on robotic node system. "
                            "It is a part of distributed robotic log delivery system\n\n"
                            "Usage:\n"
                            "    rclogd [OPTIONS...]\n\n"
                            "All options in \"Configuration\" section can also be specified "
                            "in config file defined by --config option.\n"
                            "Option specified both in config file and command line arguments "
                            "will have value specified in command line.\n"
                            "Also, it should be noted, that options without arguments (e.g. --socket-takeover) "
                            "should be specified with True/False argument when defined in config file.";

const char version_message[] = "Distributed robotic log delivery system, Dmitry Liman, 2017, version 0.1";


int readOptions(int argc, char* argv[], po::variables_map &vm) {
    std::string config_file;

    // options allowed only on command line
    po::options_description generic("Generic options");
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "produce help message")
        ("config,c", po::value<std::string>(&config_file)->value_name("filename"),
              "name of a configuration file");

    // options allowed both on command line and in config file
    po::options_description config("Configuration");
    config.add_options()
        ("socket,s", po::value<std::string>()->required(),
              "Path where unix socket will be created\n")
        ("socket-takeover",
              po::value<bool>()->implicit_value(true, "")->default_value(false, "")->value_name(" "),
              "Takes over socket file if it is already exists. The default is false\n")
        ("dbms,D", po::value<std::string>()->required(),
              "DBMS to use as local storage system for logs\n")
        ("id", po::value<std::string>()->required(),
              "ID of robotic node\n")
        ("priority,p", po::value<int>()->required(),
              "Priority of robotic node.\nUsed by nodes to deside who should be master in PtP connection\n");

    po::options_description discovery("Nodes discovery configuration");
    discovery.add_options()
        ("multicast-address", po::value<std::string>()->required(),
              "Destination address used for sending multicast hello messages\n")
        ("multicast-port", po::value<int>()->required(),
              "Port used for sending multicast hello messages\n")
        ("enable-loopback",
              po::value<bool>()->implicit_value(true, "")->default_value(false, "")->value_name(" "),
              "Receive hello messages sent by local processes. Can be used for debugging\n")
        ("hello-interval", po::value<int>()->required()->value_name("seconds"),
              "Defines how often hello messages should be sent\n");


    // options that can be specified in command line args
    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(discovery);

    // options that can be specified in file
    po::options_description file_options;
    file_options.add(config).add(discovery);


    // first load only generic options
    po::store(po::command_line_parser(argc, argv).options(generic).allow_unregistered().run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << help_message << "\n";
        std::cout << cmdline_options << "\n";
        std::cout << version_message << "\n";
        return -1;
    }

    if (vm.count("version")) {
        std::cout << version_message << "\n";
        return -1;
    }

    if (config_file.empty()) {
        po::store(po::command_line_parser(argc, argv).options(cmdline_options).run(), vm);
        po::notify(vm);
        return 0;
    }

    std::ifstream ifs(config_file.c_str());
    if (!ifs) {
        std::cerr << "Error: Can not open config file: " << config_file << "\n";
        return 1;
    }

    // load all options
    po::store(po::command_line_parser(argc, argv).options(cmdline_options).run(), vm);
    po::store(po::parse_config_file(ifs, file_options), vm);
    po::notify(vm);

    return 0;
}


int main(int argc, char* argv[]) {
    try {
        po::variables_map vm;
        int ec = readOptions(argc, argv, vm);
        if (ec > 0)
            return ec;
        if (ec < 0)
            return 0;

        std::cerr << version_message << std::endl;

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
