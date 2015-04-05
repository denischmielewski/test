#ifndef PUSHME_H
#define PUSHME_H

#include <QPushButton>
#include <QMessageBox>

class PushMe : public QPushButton
{
    Q_OBJECT

public:
    PushMe();

private slots:
    void onClicked();

};

#endif // PUSHME_H
