#include "test2.h"
#include "ui_test2.h"

Test2::Test2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Test2)
{
    ui->setupUi(this);
}

Test2::~Test2()
{
    delete ui;
}
