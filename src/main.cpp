#include <init/RrcApp.hpp>
#include <logging.hpp>
#include <program_options.hpp>

#include <cstring>
#include <csignal>
#include <memory>
#include <unistd.h>

std::shared_ptr<rrc::init::RrcApp> g_app;

void sigquit_handler(int signal)
{
    if (g_app)
    {
        g_app->stop();
        g_app.reset();
    }
    exitLog();
    std::exit(0);
}

int main(int argc, char** argv)
{
    std::signal(SIGQUIT, sigquit_handler);

    initLog();

    g_app = std::make_shared<rrc::init::RrcApp>(argc, argv);

    if (g_app->logToStdout())
    {
        enableStdoutLogging();
    }

    int status = g_app->run();

    if (g_app)
    {
        g_app.reset();
    }

    // Tutaj jest akcja ze aplikacja jest nie do konca zniszczona i wylacza sie logger i juz chce wychodzic ale czeka na wszystko

    exitLog();
    return status;
}