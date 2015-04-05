#include "pushme.h"
#include <QApplication>

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

#include "log.hpp"


int main(int argc, char *argv[])
{
    class log * train_logs_start;

    train_logs_start = new class log;

    startup_severity_channel_logger_mt& lg = startup_logger_c1::get();

    BOOST_LOG_SEV(lg, notification) << "First log from Qt !";

    QApplication a(argc, argv);
    PushMe w;
    w.show();

    int ret = a.exec();
    delete(train_logs_start);
    return ret;
}
