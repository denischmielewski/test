#include "test2.h"
#include "test2_2.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Test2 w;
    w.show();
    a.exec();

    QApplication b(argc, argv);
    Test2_2 w2;
    w2.show();

    return b.exec();
}
