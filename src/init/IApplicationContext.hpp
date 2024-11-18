#ifndef RRC_INIT_IAPPLICATIONCONTEXT_HPP_
#define RRC_INIT_IAPPLICATIONCONTEXT_HPP_

#include <connection/IDeviceConnection.hpp>
#include <init/IApplicationContext.hpp>
#include <ps5_ctrl/IGamepadAgent.hpp>

#include <program_options.hpp>

#include <filesystem>
#include <memory>

namespace rrc::init
{

class IApplicationContext
{
public:
    virtual ~IApplicationContext() = default;
    virtual const ::ProgramConfiguration & getConfig() const = 0;
    virtual std::unique_ptr<ps5_ctrl::IGamepadAgent> buildGamepadAgent(const std::filesystem::path &device_path) = 0;
    virtual std::unique_ptr<connection::IDeviceConnection> buildDeviceConnection(const std::string &address, unsigned short port) = 0;
};

} // namespace rrc::shared

#endif // RRC_INIT_IAPPLICATIONCONTEXT_HPP_