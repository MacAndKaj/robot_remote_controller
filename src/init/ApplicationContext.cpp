#include <init/ApplicationContext.hpp>

#include <connection/DeviceConnection.hpp>
#include <ps5_ctrl/Ps5DualsenseHidAgent.hpp>

namespace rrc::init
{

ApplicationContext::ApplicationContext(int argc, char** argv)
    : m_application_config(::parseProgramOptions(argc, argv))
{
}

const ProgramConfiguration & ApplicationContext::getConfig() const
{
    return m_application_config;
}

std::unique_ptr<ps5_ctrl::IGamepadAgent> ApplicationContext::buildGamepadAgent(const std::filesystem::path & filepath)
{
    return std::make_unique<ps5_ctrl::Ps5DualsenseHidAgent>(filepath);
}

std::unique_ptr<connection::IDeviceConnection> ApplicationContext::buildDeviceConnection(const std::string & address, unsigned short port)
{
    return std::make_unique<connection::DeviceConnection>(address, port);
}

} // namespace rrc::init 
