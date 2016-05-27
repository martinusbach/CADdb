#include <iostream>
#include <iomanip>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

namespace blog = boost::log;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

int main(int argc, char* argv[])
{
    std::cout << "Hello World!" << std::endl;

    blog::add_file_log
    (
        keywords::file_name = "boostexp_%Y-%m-%d_%H-%M-%S.log",
        keywords::format =
        (
            expr::stream
                << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
                << " [" << std::setw(7) << blog::trivial::severity << "] "
                << expr::smessage
        )
    );

    blog::add_common_attributes();

    blog::core::get()->set_filter(blog::trivial::severity >= blog::trivial::trace);


    BOOST_LOG_TRIVIAL(trace)   << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug)   << "A debug severity message";
    BOOST_LOG_TRIVIAL(info)    << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message: " << 1 << ", " << 1.0/3.0;
    BOOST_LOG_TRIVIAL(error)   << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal)   << "A fatal severity message";

    return 0;
}

