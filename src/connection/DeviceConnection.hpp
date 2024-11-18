#ifndef CONNECTION_DEVICECONNECTION_HPP_
#define CONNECTION_DEVICECONNECTION_HPP_

#include <connection/IDeviceConnection.hpp>

#include <boost/asio.hpp>

namespace connection
{

class DeviceConnection : public IDeviceConnection
{
public:
    DeviceConnection(const std::string &address, unsigned short port);
    virtual ~DeviceConnection();
    bool send(const Indication & msg) override;

private:
    boost::asio::io_context m_io_context;
    boost::asio::ip::udp::socket m_socket;
    boost::asio::ip::udp::endpoint m_endpoint;
};

} // namespace connection

#endif // CONNECTION_DEVICECONNECTION_HPP_
