#ifndef RRC_PS5_CTRL_IGAMEPADAGENT_HPP_
#define RRC_PS5_CTRL_IGAMEPADAGENT_HPP_

#include <ps5_ctrl/GamepadState.hpp>

namespace rrc::ps5_ctrl
{

class IGamepadAgent
{
public:
    virtual ~IGamepadAgent() = default;
    virtual GamepadState readState() const = 0;
};

} // namespace rrc::ps5_ctrl

#endif // RRC_PS5_CTRL_IGAMEPADAGENT_HPP_