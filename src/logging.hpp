#ifndef LOGGING_HPP_
#define LOGGING_HPP_

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <chrono>
#include <filesystem>
#include <string>

std::string getTimeStr(const std::time_t &time)
{
    char timeString[std::size("yyyy_mm_dd_hh_mm_ss")];
    std::strftime(std::data(timeString),
        std::size(timeString),
        "%Y_%m_%e_%H_%M_%S",
        std::gmtime(&time));
    return std::string(timeString);
}

inline void initLog(const std::filesystem::path &directory=std::filesystem::current_path())
{
    namespace logging = boost::log;
    namespace keywords = boost::log::keywords;
    namespace expr = boost::log::expressions;

    auto time_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto filepath = directory.string();
    filepath.append("/device_controller_");
    filepath.append(getTimeStr(time_now));
    filepath.append(".log");

    logging::formatter simpleFormat(
        expr::format("[%1%]/ %2% / %3%") %
        expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S.%f") %
        logging::trivial::severity %
        expr::smessage); 

    logging::add_file_log(
        keywords::file_name = filepath,
        keywords::format = simpleFormat
    );

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );
}

inline void exitLog()
{
    namespace logging = boost::log;
    logging::core::get()->flush();
}

#endif // LOGGING_HPP_