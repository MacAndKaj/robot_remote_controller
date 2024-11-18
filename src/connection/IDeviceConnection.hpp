#ifndef CONNECTION_IDEVICECONNECTION_HPP_
#define CONNECTION_IDEVICECONNECTION_HPP_

#include <connection/Messages.hpp>

namespace connection
{

class IDeviceConnection
{
public:
    virtual ~IDeviceConnection() = default;
    virtual bool send(const Indication & msg) = 0;
};

} // namespace connection

#endif // CONNECTION_IDEVICECONNECTION_HPP_
