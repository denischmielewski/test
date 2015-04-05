#include "test2_2.h"
#include "ui_test2_2.h"

Test2_2::Test2_2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Test2_2)
{
    ui->setupUi(this);
}

Test2_2::~Test2_2()
{
    delete ui;
}
