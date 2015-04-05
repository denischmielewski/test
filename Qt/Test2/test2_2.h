#ifndef TEST2_2_H
#define TEST2_2_H

#include <QMainWindow>

namespace Ui {
class Test2_2;
}

class Test2_2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Test2_2(QWidget *parent = 0);
    ~Test2_2();

private:
    Ui::Test2_2 *ui;
};

#endif // TEST2_2_H
