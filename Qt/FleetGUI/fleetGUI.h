#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QProcess>
#include <string>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QPainterPath>

#include "fleetGUIcommunicationsserver.h"
#include "fleetGUIcommunicationclient.hpp"
#include "config.hpp"
#include "ui_fleetGUI.h"
#include "utils.hpp"
#include "errors.hpp"
#include "log.hpp"
#include <unordered_map>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0,
                        const config * const fleetGUI_configuration = 0,
                        std::unordered_map<std::string, TrainSession> * trainsSessions = 0,
                        FleetGUICommunicationsServer const * t = 0,
                        FleetGUICommunicationClient const * c = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);
    startup_severity_channel_logger_mt * fleetGUI_logger_;

private:
    Ui::MainWindow *ui;
    const config * softwareConfig_ = 0;
    std::unordered_map<std::string, TrainSession> * trainsSessions_ = 0;
    QGraphicsScene * myScene;
    QPainterPath linePath_;
    float linePathLength_;
    float currentScaleW_ = 1;
    float currentScaleY_ = 1;
    QTimer * timerForGUIRefresh_ = nullptr;
    std::unordered_map<std::string, QGraphicsEllipseItem *>    trainsQGraphicsEllipseItems_;

    QGraphicsEllipseItem * testEllipseItem_;

signals:
    void myclose(void);
public slots:
    void onTimerForGUIRefreshShot(void);
};

#endif // MAINWINDOW_H
