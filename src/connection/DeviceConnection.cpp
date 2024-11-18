#include <connection/DeviceConnection.hpp>

#include <logging.hpp>

#include <boost/json.hpp>

namespace connection
{
namespace b_asio = boost::asio;

std::string marshallMessage(const ControllerStateInd & msg);

DeviceConnection::DeviceConnection(const std::string &address, unsigned short port)
    : m_socket(m_io_context)
    , m_endpoint(b_asio::ip::address_v4::from_string(address), port)
{
    m_socket.open(b_asio::ip::udp::v4());
    BOOST_LOG_TRIVIAL(info) << "[DeviceConnection] Created device connection to endpoint:" << m_endpoint;
}

DeviceConnection::~DeviceConnection()
{
    BOOST_LOG_TRIVIAL(info) << "[~DeviceConnection] Closing connection:";
    m_socket.close();
}

bool DeviceConnection::send(const Indication & msg)
{
    static constexpr int BufferSize{256};
    static char buffer[BufferSize];
    std::memset(buffer, 0, BufferSize);
    std::string json_msg;
    if (std::holds_alternative<ControllerStateInd>(msg))
    {
        json_msg = marshallMessage(std::get<ControllerStateInd>(msg));
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "[DeviceConnection::send] Unsupported message type: " << msg.index();
        return false;
    }

    if (json_msg.size() > BufferSize)
    {
        BOOST_LOG_TRIVIAL(error) << "[DeviceConnection::send] Marshalled message too long! Type: " << msg.index();
        BOOST_LOG_TRIVIAL(debug) << "[DeviceConnection::send] Message: " << json_msg;
        return false;
    }
    std::memcpy(buffer, json_msg.data(), json_msg.size());
    m_socket.send_to(b_asio::buffer(buffer, BufferSize), m_endpoint);

    return true;
}

std::string marshallMessage(const ControllerStateInd & msg)
{
    namespace b_json = boost::json;

    b_json::value json_msg = {
        {"l_x", msg.l_x},
        {"l_y", msg.l_y},
    };

    return b_json::serialize(json_msg);
}

} // namespace connection
