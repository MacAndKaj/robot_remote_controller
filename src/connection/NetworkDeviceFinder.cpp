#include <connection/NetworkDeviceFinder.hpp>

#include <logging.hpp>

#include <iostream>
#include <thread>

namespace connection
{

EndpointInfo parseMessageFromDevice(std::string &&message)
{
    if (message.empty())
    {
        return std::make_pair("", 0);
    }

    auto split_char_index = message.find(':');
    if (split_char_index == message.npos)
    {
        return std::make_pair("", 0);
    }

    auto port_str = message.substr(split_char_index+1);
    return std::make_pair(
        message.substr(0, split_char_index),
        static_cast<unsigned short>(std::strtoul(port_str.c_str(), NULL, 10))
     );
}

NetworkDeviceFinder::NetworkDeviceFinder(unsigned short port)
    : m_socket(m_io_context)
    , m_endpoint(boost::asio::ip::address_v4::broadcast(), port)
{
    m_socket.open(boost::asio::ip::udp::v4());
    m_socket.bind(m_endpoint);
    BOOST_LOG_TRIVIAL(info) << "[NetworkDeviceFinder] Connected to: " << m_endpoint;
}

NetworkDeviceFinder::~NetworkDeviceFinder()
{
    BOOST_LOG_TRIVIAL(info) << "[~NetworkDeviceFinder] Closing connection";
    m_socket.close();
}

EndpointInfo NetworkDeviceFinder::getConnectionInfo()
{
    using namespace std::chrono_literals;
    std::promise<EndpointInfo> device_address;
    std::future<EndpointInfo> device_address_future = device_address.get_future();

    std::thread work_thread(&NetworkDeviceFinder::work, this, std::move(device_address));

    std::future_status status;
    do
    {
        BOOST_LOG_TRIVIAL(info) << "[NetworkDeviceFinder::getDeviceConnection] Waiting for device address...";
        status = device_address_future.wait_for(10s);
    }
    while (status == std::future_status::timeout);
    
    if (work_thread.joinable())
    {
        work_thread.join();
    }

    if (status != std::future_status::ready)
    {
        return {"", 0};
    }

    return device_address_future.get();
}

void NetworkDeviceFinder::work(std::promise<EndpointInfo> p)
{
    static constexpr int buffer_size{1024};
    std::array<char, buffer_size> buffer;
    auto output = std::make_pair<std::string, unsigned short>("", 0);

    size_t bytes_received;
    do
    {
        std::memset(buffer.data(), 0, buffer_size);
        try
        {
            bytes_received = m_socket.receive_from(boost::asio::buffer(buffer), m_endpoint);
        }
        catch (std::exception &e)
        {
            BOOST_LOG_TRIVIAL(error) << "[NetworkDeviceFinder::work] Received std::exception: " << e.what();
        }
        catch (...)
        {
            BOOST_LOG_TRIVIAL(error) << "[NetworkDeviceFinder::work] Unknown exception";
        }
        output = parseMessageFromDevice(std::string(buffer.data(), bytes_received));
    }
    while (output.first.empty());

    BOOST_LOG_TRIVIAL(error) << "[NetworkDeviceFinder::work] Received message: " << std::string(buffer.data(), bytes_received);

    p.set_value(std::move(output));
}


} // namespace connection
