#ifndef RRC_INIT_APPLICATIONCONTEXT_HPP_
#define RRC_INIT_APPLICATIONCONTEXT_HPP_

#include <init/IApplicationContext.hpp>

namespace rrc::init
{

class ApplicationContext : public IApplicationContext
{
public:
    ApplicationContext(int argc, char** argv);
    const ProgramConfiguration & getConfig() const override;
    std::unique_ptr<ps5_ctrl::IGamepadAgent> buildGamepadAgent(const std::filesystem::path & device_path);
    std::unique_ptr<connection::IDeviceConnection> buildDeviceConnection(const std::string & address, unsigned short port);

private:
    ::ProgramConfiguration m_application_config;
};

} // namespace rrc::init

#endif // RRC_INIT_APPLICATIONCONTEXT_HPP_