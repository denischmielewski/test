
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
    sink->locked_backend()->set_file_collector(sinks::file::make_collector(
        keywords::target = "/home/train/logs",                      /*< the target directory >*/
        keywords::max_size = 10 * 1024 * 1024,         /*< maximum total size of the stored files, in bytes >*/
        keywords::min_free_space = 1024 * 1024 * 1024   /*< minimum free space on the drive, in bytes >*/
    ));
}


boost::shared_ptr< sink_t > init_logging_startup()
{
    char hostname[1024];
    gethostname(hostname, 1024);


    logging::register_simple_formatter_factory<severity_level, char>("Severity");
    std::string s = std::string(hostname) + "_STARTUP_" + "_%Y%m%d_%H%M%S_%5N.log";

    // Create a text file sink
    boost::shared_ptr< file_sink > sink(new file_sink(
        keywords::file_name = s,
        keywords::rotation_size = 16384
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

//file collecting after xml config files have been read !
void init_file_collecting_with_xml_config(boost::shared_ptr< file_sink > sink, config const * xmlconfig)
{
    sink->locked_backend()->set_file_collector(sinks::file::make_collector(
        keywords::target = "/home/train/logs",                      /*< the target directory >*/
        keywords::max_size = xmlconfig->boostLogCollectorFolderMaxSizeInMbytes_ * 1024 * 1024,         /*< maximum total size of the stored files, in bytes >*/
        keywords::min_free_space = xmlconfig->boostLogDriveMinimumFreeSizeInMegabytes_ * 1024 * 1024   /*< minimum free space on the drive, in bytes >*/
    ));
}


void init_logging_with_xml_config(config const * xmlconfig)
{

    char * hostname = nullptr;
    hostname = new char[xmlconfig->linuxSysCallBufferSize_];   //deleted when hostname is not necessary any more
    gethostname(hostname, xmlconfig->linuxSysCallBufferSize_);


    logging::register_simple_formatter_factory<severity_level, char>("Severity");
    std::string s = std::string(hostname) + "_%Y%m%d_%H%M%S_%5N.log";
    delete [] hostname;

    // Create a text file sink
    boost::shared_ptr< file_sink > sink(new file_sink(
        keywords::file_name = s,
        keywords::rotation_size = xmlconfig->boostLogFileMaxSizeInKBytes_ * 1024
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

// TODO (dev#1#15-03-27): Add filtering after adding several notification level


    if(xmlconfig->boostLogAutoFlush_) sink->locked_backend()->auto_flush(true);
    else    sink->locked_backend()->auto_flush(false);



    // Add the sink to the core
    logging::core::get()->add_sink(sink);


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

log::log(config const * xmlconfig)
{
    try
    {
        // Initialize logging library
        init_logging_with_xml_config(xmlconfig);

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

void log::RemoveStartupSink(void)
{
    boost::shared_ptr< logging::core > core = logging::core::get();

    // Remove the sink from the core, so that no records are passed to it
    core->remove_sink(startupSink);

    // Flush all log records that may have left buffered
    startupSink->flush();

    startupSink.reset();

}

