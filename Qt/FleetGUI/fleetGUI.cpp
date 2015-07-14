#include "fleetGUI.h"
#include <QtDebug>
#include <iostream>
#include <string>
#include <math.h>

MainWindow::MainWindow(QWidget *parent, const config * fleetGUI_configuration,
                                        std::unordered_map<std::string, TrainSession> * trainsSessions,
                                        FleetGUICommunicationsServer const * t, const FleetGUICommunicationClient *c) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    bool connection;
    softwareConfig_ = fleetGUI_configuration;
    trainsSessions_ = trainsSessions;
    startup_severity_channel_logger_mt& lg = fleetGUI_logger::get();
    fleetGUI_logger_ = &lg;
    QPainterPath linePath, stopsPath;

    BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "==================== CONSTRUCTOR !!! ";

    ui->setupUi(this);


    connection = connect(this, &MainWindow::myclose, t, &FleetGUICommunicationsServer::onCloseFleetGUI);
    connection = connect(this, &MainWindow::myclose, c, &FleetGUICommunicationClient::onCloseFleetGUI);
    myScene = new QGraphicsScene(0,0,1600,900,this);
    ui->graphicsView->setScene(myScene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);
    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    QPen redPen(Qt::red);
    redPen.setWidth(2);
    QPen bluePen(Qt::blue);
    bluePen.setWidth(3);
    linePath.moveTo(50, 800);
    linePath.cubicTo(80,600, 200,600, 200,600);
    linePath.cubicTo(300,600, 300,400, 300,400);
    linePath.cubicTo(300,300, 200,300, 200,300);
    linePath.cubicTo(80,300, 40,200, 40,200);
    linePath.cubicTo(20,100, 80,80, 100,80);
    linePath.cubicTo(200,80, 300,50, 300,50);
    linePath.cubicTo(400,20, 500,20, 500,20);
    linePath.cubicTo(575,15, 630,20, 630,20);
    linePath.cubicTo(735,25, 835,20, 835,20);
    linePath.cubicTo(910,15, 1030,20, 1030,20);
    linePath.cubicTo(1087,22, 1190,15, 1190,15);
    linePath.cubicTo(1250,11, 1319,1, 1365,45);
    linePath.cubicTo(1400,75, 1400,125, 1400,125);
    linePath.cubicTo(1400,195, 1320,205, 1320,205);
    linePath.cubicTo(1247,218, 1067,334, 1125,435);
    linePath.cubicTo(1140,465, 1170,480, 1200,485);
    linePath.cubicTo(1279,493, 1274,580, 1195,565);
    linePath.cubicTo(1145,559, 1070,542, 1040,595);
    linePath.cubicTo(872,737, 609,755, 400,790);

    linePathLength_ = linePath.length(); //Memorized after Line was built and before adding any item (because adding items length is modified)
    std::string s = std::to_string(linePathLength_);
    //qDebug("\n\n ==================== TEST111 !!!!! ========================");
    BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "\n******* length " << " is " <<  s.c_str();
    //add stations
    QPointF qp1;
    // #1
    float length = 0.0/38500*linePathLength_;
    float percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #2
    length = 100.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #3
    length = 400.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #4
    length = 500.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #5
    length = 3500.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #6
    length = 6500.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #7
    length = 9500.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #8
    length = 12500.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #9
    length = 14500.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #10
    length = 16000.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #11
    length = 17000.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #12
    length = 18500.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #13
    length = 20000.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #14
    length = 21500.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #15
    length = 23000.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #16
    length = 24000.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #17
    length = 25500.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #18
    length = 26500.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #19
    length = 28000.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #20
    length = 30000.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #21
    length = 32000.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #22
    length = 34500.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #23
    length = 38000.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);
    // #24
    length = 38500.0/38500.0*linePathLength_;
    percent = linePath.percentAtLength(length);
    qp1 = linePath.pointAtPercent(percent);
    stopsPath.addEllipse(qp1, 5, 5);

    QGraphicsPathItem * myPath = ui->graphicsView->scene()->addPath(linePath, bluePen);
    ui->graphicsView->scene()->addPath(stopsPath, blackPen);
    timerForGUIRefresh_ = new QTimer;
    connect(timerForGUIRefresh_, &QTimer::timeout, this, &MainWindow::onTimerForGUIRefreshShot);
    timerForGUIRefresh_->start(1000);
    linePath_ = linePath;
    BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "===================== END CONSTRUCTOR MAIN WINDOW ===================";
}

MainWindow::~MainWindow()
{
    timerForGUIRefresh_->stop();
    delete timerForGUIRefresh_;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
    emit myclose();
    evt->accept();
}

void MainWindow::resizeEvent(QResizeEvent * evt)
{

    int hcw = ui->centralWidget->height();
    BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "central widget height " << hcw;
    int wcw = ui->centralWidget->width();
    BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "central widget width " << wcw;
    ui->graphicsView->resize(wcw, hcw);
    int hgw = ui->graphicsView->height();
    BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "graphicView height " << hgw;
    int wgw = ui->graphicsView->width();
    BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "graphicView width " << wgw;

    float newScalex, newScaley;
    newScalex = (float)wcw/(float)1600;
    newScaley = (float)hcw/(float)1000;
    //invert previous scale
    ui->graphicsView->scale(1/currentScaleW_, 1/currentScaleY_);
    BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "newScalex factor = " << newScalex << "newScaley factor = " << newScaley;
    //apply new scale
    ui->graphicsView->scale(newScalex, newScaley);
    //memorize new scale
    currentScaleW_ = newScalex;
    currentScaleY_ = newScaley;

    QRectF sceneRect = ui->graphicsView->sceneRect();
    ui->graphicsView->scene()->setSceneRect(sceneRect);
    evt->accept();
    BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "===================== END RESIZE ===================";
}

void MainWindow::onTimerForGUIRefreshShot(void)
{
    //display trains
    BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "Start Displaying trains !";
    int i = 0;
    for ( auto it = trainsSessions_->begin(); it != trainsSessions_->end(); ++it )
    {
        //TrainOperationSession trainOperationSession = (it->second).GetTrainOperationSessionRef();
        if((it->second).GetTrainOperationSessionRef().IsThisSessionATrain())
        {
            i++;
        }
    }
    BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "number of Train : " << i;
    i = 0;
    for ( auto it = trainsSessions_->begin(); it != trainsSessions_->end(); ++it )
    {
        if((it->second).GetTrainOperationSessionRef().IsThisSessionATrain())
        {
            i++;
            BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "========================== Train : " << i << " IP address :" << it->first;
            // 1st step: curve length. It was calculated after Line was built and before adding any item (else length is modified)
            BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "length : " << linePathLength_;

            if((it->second).GetTrainOperationSessionRef().TryLockOperationSessionMutexFor(softwareConfig_->commSessionMutexLockTimeoutMilliseconds_))
            {
                static QPointF qp1, qp2, qp3;
                //retrieve information we need from TrainOPperationSession class then release mutex as fast as possible as it is accessed by message processing
                std::string path = ((it->second).GetTrainOperationSessionRef().GetPath());
                BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "path = " << path;
                float pkPos = (it->second).GetTrainOperationSessionRef().GetKpPosition();
                BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "pkPos = " << pkPos;
                int dir = (it->second).GetTrainOperationSessionRef().GetDirection();
                BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "dir = " << dir;
                (it->second).GetTrainOperationSessionRef().UnlockOperationSessionMutex();

                // position in line coordinates
                auto iter = softwareConfig_->linesData_.find(path); //note: we can not use map [] operator as it is not const
                float length = (pkPos - (float)((iter->second).lineFirstKPPosition_))/(float)((iter->second).linePKDistance_) * (float)linePathLength_;
                std::string s = std::to_string(length);
                BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "position in linePath_ coordinates : " << length << " kpPosition : " << pkPos;

                // retrieve percent from position in line coordinates
                //float p = linePath_.percentAtLength(((pkPos - (float)((iter->second).lineFirstKPPosition_)))/(float)((iter->second).linePKDistance_));
                float p = linePath_.percentAtLength(length);
                s = std::to_string(p);
                BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "percent : " << p << " kpPosition : " << pkPos;

                // get angle at percent
                float angle = linePath_.angleAtPercent(p);
                s = std::to_string(angle);
                BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "angle at percent : " << angle;

                // get point at percent
                qp1 = linePath_.pointAtPercent(p);
                s = std::to_string(qp1.rx());
                BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "x at percent "  << " is " <<  s.c_str();
                s = std::to_string(qp1.ry());
                BOOST_LOG_SEV(*fleetGUI_logger_, debug) << "y at percent "  << " is " <<  s.c_str();

                // get slope at percent
                float slope = linePath_.slopeAtPercent(p);
                s = std::to_string(slope);
                BOOST_LOG_SEV(*fleetGUI_logger_, debug) <<  "slope at percent "  << " is " <<  s.c_str();


                QBrush yellowBrush(Qt::yellow);
                QBrush blackBrush(Qt::black);
                QBrush greenBrush(Qt::green);
                //QPen greenPen(Qt::green);
                QPen greenPen(greenBrush, 10);
                int16_t trainSizePixel = softwareConfig_->configForFleetGUISw_.trainDisplaySizeInPixel_;
                int16_t trainDisplayOffsetPixel = softwareConfig_->configForFleetGUISw_.trainDisplayOffsetRelativeToLineInPixels_;

                bool firstTimeTrainIsReceived = false;
                auto search = trainsQGraphicsEllipseItems_.find(it->first);
                if(search == trainsQGraphicsEllipseItems_.end()) firstTimeTrainIsReceived = true;   //for unordered map management
                if(dir == 1)
                {
                    //center of train position point for direction 1
                    qp2.setX((qp1.rx()) - (trainDisplayOffsetPixel * sin(angle/360*2*M_PI)));
                    qp2.setY((qp1.ry()) - (trainDisplayOffsetPixel * cos(angle/360*2*M_PI)));
                    if(firstTimeTrainIsReceived)
                    {
                        //first time, create the item
                        BOOST_LOG_SEV(*fleetGUI_logger_, debug) <<  "first time on graphic scene for " \
                                                                 << it->first << " x = " << qp2.rx()-trainSizePixel/sqrt(2) << " y = " << qp2.ry()-trainSizePixel/sqrt(2);

                        QGraphicsEllipseItem * ei = \
                                ui->graphicsView->scene()->addEllipse(qp2.rx()-trainSizePixel/sqrt(2),qp2.ry()-trainSizePixel/sqrt(2),trainSizePixel*sqrt(2),trainSizePixel*sqrt(2),greenPen, blackBrush);
                        trainsQGraphicsEllipseItems_.emplace(it->first, ei);
                    }
                    else
                    {
                        trainsQGraphicsEllipseItems_[it->first]->setRect(qp2.rx()-trainSizePixel/sqrt(2),qp2.ry()-trainSizePixel/sqrt(2),trainSizePixel*sqrt(2),trainSizePixel*sqrt(2));
                    }
                }
                else if(dir == 2)
                {
                    //center of train position point for direction 2
                    qp3.setX((qp1.rx()) + (trainDisplayOffsetPixel * sin(angle/360*2*M_PI)));
                    qp3.setY((qp1.ry()) + (trainDisplayOffsetPixel * cos(angle/360*2*M_PI)));
                    if(firstTimeTrainIsReceived)
                    {
                        //first time, create the item
                        BOOST_LOG_SEV(*fleetGUI_logger_, debug) <<  "first time on graphic scene for " \
                                                                 << it->first << " x = " << qp3.rx()-trainSizePixel/sqrt(2) << " y = " << qp3.ry()-trainSizePixel/sqrt(2);
                        QGraphicsEllipseItem * ei = \
                                ui->graphicsView->scene()->addEllipse(qp3.rx()-trainSizePixel/sqrt(2),qp3.ry()-trainSizePixel/sqrt(2),trainSizePixel*sqrt(2),trainSizePixel*sqrt(2),greenPen, blackBrush);
                        trainsQGraphicsEllipseItems_.emplace(it->first, ei);
                    }
                    else
                    {
                        trainsQGraphicsEllipseItems_[it->first]->setRect(qp3.rx()-trainSizePixel/sqrt(2),qp3.ry()-trainSizePixel/sqrt(2),trainSizePixel*sqrt(2),trainSizePixel*sqrt(2));
                    }
                }
            }
            else
            {
                BOOST_LOG_SEV(*fleetGUI_logger_, warning) << "Train Operation Session Lock failed !!!";
            }
        }
    }
}

