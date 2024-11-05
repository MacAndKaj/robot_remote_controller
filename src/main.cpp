#include "logging.hpp"

#include <ps5_ctrl/Ps5DualsenseHidAgent.hpp>
#include <connection/NetworkDeviceFinder.hpp>

#include <cstring>
#include <iostream>
#include <unistd.h>

void sigquit_handler(int signal)
{
    exitLog();
    std::exit(0);
}


int main(int argc, char** argv)
{
    std::signal(SIGQUIT, sigquit_handler);

    initLog();

    BOOST_LOG_TRIVIAL(info) << "HEllo from reader, argc: " << argc;

    constexpr unsigned short default_broadcast_port{5162};

    BOOST_LOG_TRIVIAL(info) << "Netowrk device finder start";
    connection::NetworkDeviceFinder finder(default_broadcast_port);

    BOOST_LOG_TRIVIAL(info) << "Starting device detection";
    auto device_connection = finder.getDeviceConnection();

    BOOST_LOG_TRIVIAL(info) << "Starting controller";
    ps5_ctrl::Ps5DualsenseHidAgent controller_if{std::filesystem::path{argv[1]}};

    int i = 10000;
    auto previous_state = controller_if.readState();
    while (i-->0)
    {
        auto state = controller_if.readState();

        if (std::memcmp(&state, &previous_state,sizeof(previous_state)) != 0)
        {
            std::cout << "L_X: [" << state.l_x;
            std::cout << "] L_Y: [" << state.l_y;
            std::cout << "] R_X: [" << state.r_x;
            std::cout << "] R_Y: [" << state.r_y;
            std::cout << "]" << std::endl;
        }
        // sleep(1);
        previous_state = state;
    }

    return EXIT_SUCCESS;
}