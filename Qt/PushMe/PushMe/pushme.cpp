#include "pushme.h"

PushMe::PushMe()
{
    setText("PushMe");
    connect(this, &QPushButton::clicked, this, &PushMe::onClicked);
}

void PushMe::onClicked()
{
    QMessageBox::information(this, "..", "I have been pushed 0002 !");
}
