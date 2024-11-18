#ifndef CONNECTION_MESSAGES_HPP_
#define CONNECTION_MESSAGES_HPP_

#include <variant>

namespace connection
{

struct ControllerStateInd
{
    int l_x;
    int l_y;
};

struct ControllerEnabledInd
{
    bool enabled;
};

// struct GetDeviceStateReq
// {

// };

typedef std::variant<
    ControllerStateInd,
    ControllerEnabledInd
> Indication;

} // namespace connection

#endif // CONNECTION_MESSAGES_HPP_
