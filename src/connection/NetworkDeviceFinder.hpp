#ifndef CONNECTION_NETWORKDEVICEFINDER_HPP_
#define CONNECTION_NETWORKDEVICEFINDER_HPP_

#include <connection/DeviceConnection.hpp>

#include <boost/asio.hpp>

#include <future>
#include <memory>
#include <string>

namespace connection
{
using EndpointInfo = std::pair<std::string, unsigned short>;

class NetworkDeviceFinder
{
public:
    NetworkDeviceFinder(unsigned short port);
    virtual ~NetworkDeviceFinder();

    EndpointInfo getConnectionInfo();

private:
    void work(std::promise<EndpointInfo> p);

    boost::asio::io_context m_io_context;
    boost::asio::ip::udp::socket m_socket;
    boost::asio::ip::udp::endpoint m_endpoint;
};

} // namespace connection

#endif // CONNECTION_NETWORKDEVICEFINDER_HPP_