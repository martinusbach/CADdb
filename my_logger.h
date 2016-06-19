#ifndef MY_LOGGER_H
#define MY_LOGGER_H

#include <boost/log/trivial.hpp>

#define LOG(lvl) BOOST_LOG_TRIVIAL(lvl)

void log_init();
void log_example();

#endif // MY_LOGGER_H

