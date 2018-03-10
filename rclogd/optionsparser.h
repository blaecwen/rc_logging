#ifndef OPTIONSPARSER_H
#define OPTIONSPARSER_H

#include <boost/program_options.hpp>

namespace rclog {

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


class OptionsParser
{
public:
    OptionsParser();

    int parse(int argc, char* argv[], po::variables_map &vm);

private:
    void initOptions();

    std::string config_file;

    po::options_description generic;            // allowed only in command line
    po::options_description config;             // general configuration
    po::options_description discovery;          // discovery module configuration

    po::options_description cmdline_options;    // can be specified in command line args
    po::options_description file_options;       // can be specified in file
};

}

#endif // OPTIONSPARSER_H
