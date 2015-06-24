#ifndef LOG_HPP
#define LOG_HPP

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/channel_feature.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/formatting_ostream.hpp>
#include <boost/log/utility/manipulators/to_log.hpp>

#include <cstddef>
#include <iostream>
#include <boost/log/utility/setup/common_attributes.hpp>

#include "config.hpp"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;


#define NUM_SEVERITY_LEVELS 9

enum severity_level
{
    debug,                          //for development team
    debug_temp,                //for development team
    message,
    threads,
    notification,
    warning,
    error,
    critical
};

static const char* severity_level_strings[] =
{
            "debug          ",                      //for development team
            "debug_tempo    ",                //for development team
            "message        ",
            "threads        ",
            "notification   ",
            "warning        ",
            "error          ",
            "critical       "
};

template< typename CharT, typename TraitsT >
std::basic_ostream< CharT, TraitsT >&
operator<< (
  std::basic_ostream< CharT, TraitsT >& strm,
  severity_level lvl
)
{
    const char* str = severity_level_strings[lvl];
    if (lvl < NUM_SEVERITY_LEVELS && lvl >= 0)
        strm << str;
    else
        strm << static_cast< int >(lvl);
    return strm;
}

typedef src::severity_channel_logger_mt<
    severity_level,     // the type of the severity level
    std::string         // the type of the channel name
> startup_severity_channel_logger_mt;

typedef sinks::synchronous_sink< sinks::text_file_backend > sink_t;


BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(startup_logger, startup_severity_channel_logger_mt)
{
    // Specify the channel name on construction, similarly as with the channel_logger
    return startup_severity_channel_logger_mt(keywords::channel = "config\t\t");
}

BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(main_logger, startup_severity_channel_logger_mt)
{
    // Specify the channel name on construction, similarly as with the channel_logger
    return startup_severity_channel_logger_mt(keywords::channel = "main\t\t");
}

BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(server_comm_logger, startup_severity_channel_logger_mt)
{
    // Specify the channel name on construction, similarly as with the channel_logger
    return startup_severity_channel_logger_mt(keywords::channel = "servercomm\t");
}

BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(client_comm_logger, startup_severity_channel_logger_mt)
{
    // Specify the channel name on construction, similarly as with the channel_logger
    return startup_severity_channel_logger_mt(keywords::channel = "clientcomm\t");
}


BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(trainGUI_logger, startup_severity_channel_logger_mt)
{
    // Specify the channel name on construction, similarly as with the channel_logger
    return startup_severity_channel_logger_mt(keywords::channel = "GUI\t\t");
}

BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(movement_logger, startup_severity_channel_logger_mt)
{
    // Specify the channel name on construction, similarly as with the channel_logger
    return startup_severity_channel_logger_mt(keywords::channel = "movement\t");
}

class log
{
    public:
        log();
        log(config const *);
        virtual ~log();
        boost::shared_ptr< sink_t > startupSink;
        boost::shared_ptr< sink_t > afterConfigSink;
        void RemoveStartupSink(void);
    protected:
    private:
};

#endif // LOG_HPP
