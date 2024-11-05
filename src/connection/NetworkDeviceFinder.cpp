#include <connection/NetworkDeviceFinder.hpp>

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
}

NetworkDeviceFinder::~NetworkDeviceFinder()
{
    std::cout << __func__ << ": Closing connection" << std::endl;
    m_socket.close();
}

std::unique_ptr<DeviceConnection> NetworkDeviceFinder::getDeviceConnection()
{
    using namespace std::chrono_literals;
    std::promise<EndpointInfo> device_address;
    std::future<EndpointInfo> device_address_future = device_address.get_future();

    std::thread work_thread(&NetworkDeviceFinder::work, this, std::move(device_address));

    std::future_status status;
    do
    {
        std::cout << "Waiting for device address..." << std::endl;
        status = device_address_future.wait_for(10s);
    }
    while (status == std::future_status::timeout);
    
    if (work_thread.joinable())
    {
        work_thread.join();
    }

    if (status != std::future_status::ready)
    {
        return nullptr;
    }

    auto [address, port] = device_address_future.get();

    return std::make_unique<DeviceConnection>(address, port);
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
            std::cerr << __func__ << "(): Received std::exception: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << __func__ << ": Unknown exception" << std::endl;   
        }
        output = parseMessageFromDevice(std::string(buffer.data(), bytes_received));
    }
    while (output.first.empty());

    std::cout << __func__ << ": Received message: " << std::string(buffer.data(), bytes_received) << std::endl;

    p.set_value(std::move(output));
}


} // namespace connection
