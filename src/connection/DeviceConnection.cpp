#include <connection/DeviceConnection.hpp>

#include <iostream>

namespace connection
{

DeviceConnection::DeviceConnection(const std::string &address, unsigned short port)
    : m_socket(m_io_context)
    , m_endpoint(boost::asio::ip::address_v4::from_string(address), port)
{
    std::cout << "Created device connection to endpoint:" << m_endpoint << std::endl;
}

} // namespace connection
