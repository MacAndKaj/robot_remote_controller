#include <init/RrcApp.hpp>
#include <connection/NetworkDeviceFinder.hpp>
#include <ps5_ctrl/GamepadMonitoring.hpp>

#include <logging.hpp>

#include <thread>

namespace rrc::init
{

struct Components
{
    std::unique_ptr<connection::NetworkDeviceFinder> finder;
    std::unique_ptr<ps5_ctrl::GamepadMonitoring> gamepad_monitoring;
};

RrcApp::RrcApp(int argc, char** argv)
    : m_app_context(argc, argv)
    , m_components(std::make_shared<Components>())
{
    BOOST_LOG_TRIVIAL(info) << "[RrcApp] App initialized successfully";
}

int RrcApp::run()
{
    using namespace std::chrono_literals;
    constexpr unsigned short default_broadcast_port{5162};
    try
    {
        BOOST_LOG_TRIVIAL(info) << "[RrcApp::run] Netowrk device finder start";
        m_components->finder = std::make_unique<connection::NetworkDeviceFinder>(default_broadcast_port);

        BOOST_LOG_TRIVIAL(info) << "Starting device detection";
        auto [addr, port] = m_components->finder->getConnectionInfo();

        m_components->gamepad_monitoring = std::make_unique<ps5_ctrl::GamepadMonitoring>(m_app_context, addr, port);

        while (true)
        {
            std::this_thread::sleep_for(1s);
        }
    }
    catch(const std::exception & e)
    {
        BOOST_LOG_TRIVIAL(error) << "[RrcApp::run] Standard exception catched: " << e.what();
        return EXIT_FAILURE;
    }
    catch(...)
    {
        BOOST_LOG_TRIVIAL(error) << "[RrcApp::run] Unknown exception catched!";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool RrcApp::logToStdout() const
{
    return m_app_context.getConfig().logs_on_stdout;
}

void RrcApp::stop()
{
    BOOST_LOG_TRIVIAL(info) << "[RrcApp::stop] Graceful shutdown";
    if (m_components->gamepad_monitoring)
    {
        m_components->gamepad_monitoring->stop();
    }
}

} // namespace rrc::init
