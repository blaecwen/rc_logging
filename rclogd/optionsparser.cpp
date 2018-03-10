#include "optionsparser.h"

#include <iostream>
#include <fstream>

namespace rclog {

OptionsParser::OptionsParser() :
    generic("Generic options"),
    config("General configuration"),
    discovery("Nodes discovery configuration")
{
    initOptions();
}

void OptionsParser::initOptions()
{
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "produce help message")
        ("config,c", po::value<std::string>(&this->config_file)->value_name("filename"),
              "name of a configuration file");

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

    cmdline_options.add(generic).add(config).add(discovery);
    file_options.add(config).add(discovery);
}

// TODO: refactor it.
int OptionsParser::parse(int argc, char *argv[], po::variables_map &vm) {
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

}
