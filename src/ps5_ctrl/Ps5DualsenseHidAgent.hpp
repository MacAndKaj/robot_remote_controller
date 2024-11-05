#ifndef PS5DUALSENSEHIDAGENT_HPP_
#define PS5DUALSENSEHIDAGENT_HPP_

#include <filesystem>

#include "Ps5DualsenseState.hpp"

namespace ps5_ctrl
{

class Ps5DualsenseHidAgent
{
public:
    Ps5DualsenseHidAgent(const std::filesystem::path &device_path);
    virtual ~Ps5DualsenseHidAgent();

    Ps5DualsenseState readState() const;
    

private:
    void verifyDevice();

    int m_fd;
    static constexpr int m_s_report_size{64};
};

} // namespace ps5_ctrl

#endif // PS5DUALSENSEHIDAGENT_HPP_