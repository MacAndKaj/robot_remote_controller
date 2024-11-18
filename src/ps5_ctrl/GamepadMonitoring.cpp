#include <ps5_ctrl/GamepadMonitoring.hpp>

#include <logging.hpp>

namespace rrc::ps5_ctrl
{

connection::Indication parseState(const GamepadState & gamepad_state)
{
    return connection::ControllerStateInd{
        .l_x = gamepad_state.l_x,
        .l_y = gamepad_state.l_y,
    };
}

bool gamepadStateChanged(const GamepadState & p1, const GamepadState & p2)
{
    return std::memcmp(&p1, &p2, sizeof(GamepadState)) != 0;
}

GamepadMonitoring::GamepadMonitoring(init::IApplicationContext & app_context, const std::string & address, unsigned short port)
    : m_agent(app_context.buildGamepadAgent(app_context.getConfig().devico_io_file_path))
    , m_connection(app_context.buildDeviceConnection(address, port))
    , m_monitoring_thread_working(true)
    , m_monitoring_thread([this](){work();})
{
    BOOST_LOG_TRIVIAL(info) << "[GamepadMonitoring] Started";
}

GamepadMonitoring::~GamepadMonitoring()
{
    if (m_monitoring_thread.joinable())
    {
        m_monitoring_thread.join();
    }
    BOOST_LOG_TRIVIAL(info) << "[GamepadMonitoring] Finished";
}

void GamepadMonitoring::work()
{
    using namespace std::chrono_literals;

    auto previous_state = m_agent->readState();
    if (not m_connection->send(parseState(previous_state)))
    {
        BOOST_LOG_TRIVIAL(error) << "[GamepadMonitoring::work] Error when sending first state";
    }

    while (m_monitoring_thread_working)
    {
        std::this_thread::sleep_for(100us);
        auto new_state = m_agent->readState();
        if (gamepadStateChanged(previous_state, new_state))
        {
            previous_state = new_state;
            BOOST_LOG_TRIVIAL(trace) << "[GamepadMonitoring::work] State changed: Lx:" << previous_state.l_x << " Ly:" << previous_state.l_y;
            if (not m_connection->send(parseState(previous_state)))
            {
                BOOST_LOG_TRIVIAL(error) << "[GamepadMonitoring::work] Error when sending changed state";
            }
        }
    }
}

void GamepadMonitoring::stop()
{
    m_monitoring_thread_working = false;
}

bool GamepadMonitoring::isRunning() const
{
    return m_monitoring_thread_working;
}

} // namespace rrc::ps5_ctrl
