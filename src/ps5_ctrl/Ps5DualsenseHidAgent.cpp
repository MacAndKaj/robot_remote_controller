#include "Ps5DualsenseHidAgent.hpp"
#include "defs.hpp"

#include <cstring>
#include <cerrno>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/hidraw.h>

#define MASK_BIT(x) (1 << x)

namespace ps5_ctrl
{

Ps5DualsenseHidAgent::Ps5DualsenseHidAgent(const std::filesystem::path &device_path)
{
    m_fd = ::open(device_path.c_str(), O_RDWR);
    if (m_fd < 0)
    {
        int e = errno;
        std::string err = "Error when opening file: ";
        err.append(std::strerror(e));
        throw std::runtime_error(err.c_str());
    }

    constexpr int max_raw_name_size{256};
    char raw_name_buf[max_raw_name_size];
    if (ioctl(m_fd, HIDIOCGRAWNAME(max_raw_name_size), &raw_name_buf)< 0)
    {
        throw std::runtime_error("Error while reading raw name");
    }

    std::cout << "Raw name: " << raw_name_buf << std::endl;

    verifyDevice();
}

Ps5DualsenseHidAgent::~Ps5DualsenseHidAgent()
{
    ::close(m_fd);
    std::cout << "Closed PS5 dualsense descriptor" << std::endl;    
}

Ps5DualsenseState Ps5DualsenseHidAgent::readState() const
{ 
    // struct hidraw_report_descriptor rd;
    // rpt_desc.size = desc_size;
    // if (ioctl(m_fd, HIDIOCGRDESC, &rd) < 0)
    // {
    //     throw std::runtime_error("Error while reading descriptor report");
    // }
    std::vector<unsigned char> buffer(m_s_report_size);
    
    int bytesRead = read(m_fd, buffer.data(), buffer.size());
    if (bytesRead == 0)
    {
        throw std::runtime_error("Incorrect size of bytes read");
    }

    Ps5DualsenseState tmp;
    std::memset(&tmp, 0, sizeof(tmp));
 
    auto state_base = buffer.cbegin();
    tmp.l_x         = *(state_base + LX_OFFSET) - 128;
    tmp.l_y         = *(state_base + LY_OFFSET) - 128;
    tmp.r_x         = *(state_base + RX_OFFSET) - 128;
    tmp.r_y         = *(state_base + RY_OFFSET) - 128;
    tmp.l_2_trig    = *(state_base + L2_OFFSET);
    tmp.r_2_trig    = *(state_base + R2_OFFSET);

    auto buttons_val = *(state_base + HAT_SWITCH_BUTTON1_OFFSET);
    tmp.triangle    = (buttons_val & MASK_BIT(7));
    tmp.circle      = (buttons_val & MASK_BIT(6));
    tmp.cross       = (buttons_val & MASK_BIT(5));
    tmp.square      = (buttons_val & MASK_BIT(4));

    auto button2_val = *(state_base + BUTTON2_OFFSET);
    tmp.l_1         = static_cast<bool>(button2_val & MASK_BIT(0));
    tmp.r_1         = static_cast<bool>(button2_val & MASK_BIT(1));
    tmp.l_2         = static_cast<bool>(button2_val & MASK_BIT(2));
    tmp.r_2         = static_cast<bool>(button2_val & MASK_BIT(3));
    tmp.share       = static_cast<bool>(button2_val & MASK_BIT(4));
    tmp.options     = static_cast<bool>(button2_val & MASK_BIT(5));
    tmp.l_3         = static_cast<bool>(button2_val & MASK_BIT(6));
    tmp.r_3         = static_cast<bool>(button2_val & MASK_BIT(7));
    
    return tmp;
}

void Ps5DualsenseHidAgent::verifyDevice()
{
    struct hidraw_devinfo info;
    if (ioctl(m_fd, HIDIOCGRAWINFO, &info) < 0)
    {
        throw std::runtime_error("Error while reading raw info");
    }

    // todo: verification
    std::cout << "Bus type: " << info.bustype << std::endl;
    std::cout << "Vendor ID: 0x" << std::hex << info.vendor << std::endl;
    std::cout << "Product ID: 0x" << std::hex << info.product << std::endl;
    std::cout << std::dec;
}

} // namespace ps5_ctrl
