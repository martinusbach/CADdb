#include <iomanip>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

namespace blog = boost::log;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

#define LOG(lvl) BOOST_LOG_TRIVIAL(lvl)

void log_init ()
{
    blog::add_file_log
    (
        keywords::file_name = "boostexp_%Y-%m-%d_%H-%M-%S.log",
        keywords::format =
        (
            expr::stream
                << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
                << " [" << std::setw(7) << blog::trivial::severity << "] "
                << expr::smessage
        ),
#ifdef DEBUG
        keywords::filter = ( blog::trivial::severity >= blog::trivial::trace )
#else
        keywords::filter = ( blog::trivial::severity >= blog::trivial::info )
#endif

    );

    blog::add_console_log
    (
        std::cout,
        keywords::format =
        (
            expr::stream
                << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
                << " [" << std::setw(7) << blog::trivial::severity << "] "
                << expr::smessage
        ),
        keywords::filter = ( blog::trivial::severity >= blog::trivial::info )
    );

    blog::add_common_attributes();
}

void log_example()
{
    LOG(trace)   << "A trace severity message";
    LOG(debug)   << "A debug severity message";
    LOG(info)    << "An informational severity message";
    LOG(warning) << "A warning severity message: " << 1 << ", " << 1.0/3.0;
    LOG(error)   << "An error severity message";
    LOG(fatal)   << "A fatal severity message";
}
