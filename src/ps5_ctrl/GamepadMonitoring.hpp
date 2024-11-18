#ifndef RCC_PS5_CTRL_GAMEPADMONITORING_HPP_
#define RCC_PS5_CTRL_GAMEPADMONITORING_HPP_

#include <connection/IDeviceConnection.hpp>
#include <init/IApplicationContext.hpp>
#include <ps5_ctrl/IGamepadAgent.hpp>

#include <program_options.hpp>

#include <atomic>
#include <thread>

namespace rrc::ps5_ctrl
{

class GamepadMonitoring
{
public:
    GamepadMonitoring(init::IApplicationContext & app_context, const std::string & address, unsigned short port);
    virtual ~GamepadMonitoring();

    void stop();
    bool isRunning() const;

private:
    void work();

    std::unique_ptr<IGamepadAgent> m_agent;
    std::unique_ptr<connection::IDeviceConnection> m_connection;

    std::atomic<bool> m_monitoring_thread_working;
    std::thread m_monitoring_thread;
};

} // namespace rrc::ps5_ctrl

#endif // RCC_PS5_CTRL_GAMEPADMONITORING_HPP_