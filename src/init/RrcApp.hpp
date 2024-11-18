#ifndef RRC_INIT_RRCAPP_HPP_
#define RRC_INIT_RRCAPP_HPP_

#include <init/ApplicationContext.hpp>

#include <memory>

namespace rrc::init
{

class Components;

class RrcApp
{
public:
    RrcApp(int argc, char** argv);

    int run();
    void stop();
    bool logToStdout() const;

private:
    ApplicationContext m_app_context;

    std::shared_ptr<Components> m_components;
};

}

#endif // RRC_INIT_RRCAPP_HPP_