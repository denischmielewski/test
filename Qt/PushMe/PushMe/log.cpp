
#include "log.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <unistd.h>     //for gethostname() call


namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;

//file collecting for startup, when config files have not been read We want to log configuration from xml files !
void init_file_collecting_startup(boost::shared_ptr< file_sink > sink)
{
    //config files have not been read so we have no choice to hardcode
    sink->locked_backend()->set_file_collector(sinks::file::make_collector(
        keywords::target = "/home/train/logs",                      /*< the target directory >*/
        keywords::max_size = 10 * 1024 * 1024,         /*< maximum total size of the stored files, in bytes >*/
        keywords::min_free_space = 1024 * 1024 * 1024   /*< minimum free space on the drive, in bytes >*/
    ));
}


boost::shared_ptr< sink_t > init_logging_startup()
{
    //config files have not been read so we have no choice to hardcode
    char hostname[1024];
    gethostname(hostname, 1024);


    logging::register_simple_formatter_factory<severity_level, char>("Severity");
    std::string s = std::string(hostname) + "_%Y%m%d_%H%M%S_%5N.log";

    // Create a text file sink
    boost::shared_ptr< file_sink > sink(new file_sink(
        keywords::file_name = s,
        keywords::rotation_size = 16384 //config files have not been read so we have no choice to hardcode
    ));

    // Set up where the rotated files will be stored
    init_file_collecting_startup(sink);

    // Upon restart, scan the directory for files matching the file_name pattern
    sink->locked_backend()->scan_for_files();

    logging::add_common_attributes();

    sink->set_formatter
    (
        expr::format("%1% <%2%> \t%3% \t%4% ")
            % expr::attr< std::string >("Channel")
            % expr::attr< severity_level >("Severity")
            % expr::attr< boost::posix_time::ptime >("TimeStamp")
            % expr::smessage
    );

    sink->locked_backend()->auto_flush(true);

    // Add the sink to the core
    logging::core::get()->add_sink(sink);

    return sink;
}


log::log()
{
    //ctor

    try
    {
        // Initialize logging library
        startupSink = init_logging_startup();

        // And also add some attributes
        logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());

    }
    catch (std::exception& e)
    {
        std::cout << "FAILURE: " << e.what() << std::endl;
        return;
    }

}



log::~log()
{
    //dtor
}



