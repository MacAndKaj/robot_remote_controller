// TODO: move to shared directory
#ifndef RRC_PROGRAM_OPTIONS_HPP
#define RRC_PROGRAM_OPTIONS_HPP

#include <boost/program_options.hpp>

#include <filesystem>
#include <iostream>

struct ProgramConfiguration
{
    bool debug_logs_enabled{false};
    bool logs_on_stdout{false};
    std::filesystem::path devico_io_file_path;
};

inline ProgramConfiguration parseProgramOptions(int argc, char** argv)
{
    namespace po = boost::program_options;

    po::options_description flags("Allowed flags");
    flags.add_options()
        ("help",        "Show description of the program.")
        ("verbose,V",   "Enables additional(debug) logs.")
        ("stdout,S",    "Enables printing logs to stdout.")
    ;

    po::options_description args_with_value("Allowed arguments");
    args_with_value.add_options()
        ("device,D", po::value<std::string>()->required(),
         "Parameter to pass HID device io file path.")
    ;

    po::options_description desc("Allowed flags");
    desc.add(flags)
        .add(args_with_value);
    po::variables_map var_map;
    po::store(po::parse_command_line(argc, argv, desc), var_map);
    po::notify(var_map);

    if (var_map.count("help"))
    {
        std::cout << desc << std::endl;
        std::exit(EXIT_SUCCESS);
    }

    ProgramConfiguration config;

    if (var_map.count("verbose"))
    {
        config.debug_logs_enabled = true;
    }

    if (var_map.count("stdout"))
    {
        config.logs_on_stdout = true;
    }

    config.devico_io_file_path = var_map.at("device").as<std::string>();

    return config;
}

#endif // RRC_PROGRAM_OPTIONS_HPP