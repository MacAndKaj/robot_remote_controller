#ifndef PS5DUALSENSEHIDAGENT_HPP_
#define PS5DUALSENSEHIDAGENT_HPP_

#include <ps5_ctrl/IGamepadAgent.hpp>
#include <ps5_ctrl/GamepadState.hpp>

#include <filesystem>

namespace rrc::ps5_ctrl
{

class Ps5DualsenseHidAgent : public IGamepadAgent
{
public:
    Ps5DualsenseHidAgent(const std::filesystem::path &device_path);
    virtual ~Ps5DualsenseHidAgent();

    GamepadState readState() const;

private:
    void verifyDevice();

    int m_fd;
    static constexpr int m_s_report_size{64};
};

} // namespace rrc::ps5_ctrl

#endif // PS5DUALSENSEHIDAGENT_HPP_