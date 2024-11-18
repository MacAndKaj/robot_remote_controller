// TODO: move to shared directory
#ifndef RRC_LOGGING_HPP_
#define RRC_LOGGING_HPP_

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <chrono>
#include <filesystem>
#include <source_location>
#include <string>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

inline std::string here_str(const std::source_location s = std::source_location::current())
{
    std::string tmp("[");
    tmp.append(s.function_name())
        .append(std::to_string(s.line()))
        .append("] ");
    return tmp;
}


inline std::string getTimeStr(const std::time_t &time)
{
    char timeString[std::size("yyyy_mm_dd_hh_mm_ss")];
    std::strftime(std::data(timeString),
        std::size(timeString),
        "%Y_%m_%e_%H_%M_%S",
        std::gmtime(&time));
    return std::string(timeString);
}

static auto simple_format = logging::formatter{
    expr::format("<%1%/%2%>[%3%/%4%]: %5%") %
    expr::attr<logging::attributes::current_thread_id::value_type>("ThreadID") %
    expr::attr<logging::attributes::current_process_id::value_type>("ProcessID") %
    expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S.%f") %
    logging::trivial::severity %
    expr::smessage};

inline void initLog(const std::filesystem::path &directory=std::filesystem::current_path())
{

    auto time_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto filepath = directory.string();
    filepath.append("/device_controller_");
    filepath.append(getTimeStr(time_now));
    filepath.append(".log");


    logging::add_common_attributes();
    logging::add_file_log(
        keywords::file_name = filepath,
        keywords::format = simple_format
    );

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );
}

inline void enableStdoutLogging()
{
    logging::add_console_log()
        ->set_formatter(simple_format);
}

inline void exitLog()
{
    namespace logging = boost::log;
    logging::core::get()->flush();
}

#endif // RRC_LOGGING_HPP_