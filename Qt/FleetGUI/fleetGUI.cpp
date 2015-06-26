#include "fleetGUI.h"
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QtDebug>
#include <iostream>
#include <string>
#include <math.h>


MainWindow::MainWindow(QWidget *parent, config const * fleetGUI_configuration, FleetGUICommunicationsServer const * t, const FleetGUICommunicationClient *c) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    bool connection;

    ui->setupUi(this);


    connection = connect(this, &MainWindow::myclose, t, &FleetGUICommunicationsServer::onCloseFleetGUI);
    connection = connect(this, &MainWindow::myclose, c, &FleetGUICommunicationClient::onCloseFleetGUI);
    myScene = new QGraphicsScene(0,0,1600,900,this);
    //myScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(myScene);
    ui->graphicsView->scale(0.25,0.25);
    //ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);
    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    QPen redPen(Qt::red);
    redPen.setWidth(2);
    QLine Line(0,0,1000,250);

    QPainterPath path;
    //path.moveTo(400, 400);
    //path.lineTo(100, 100);
    path.moveTo(50, 800);
    path.cubicTo(80,600, 200,600, 200,600);
    path.cubicTo(300,600, 300,400, 300,400);
    path.cubicTo(300,300, 200,300, 200,300);
    path.cubicTo(80,300, 40,200, 40,200);
    path.cubicTo(20,100, 80,80, 100,80);
    path.cubicTo(200,80, 300,50, 300,50);
    path.cubicTo(400,20, 500,20, 500,20);
    path.cubicTo(575,15, 630,20, 630,20);
    path.cubicTo(735,25, 835,20, 835,20);
    path.cubicTo(910,15, 1030,20, 1030,20);
    path.cubicTo(1087,22, 1190,15, 1190,15);
    path.cubicTo(1250,11, 1319,1, 1365,45);
    path.cubicTo(1400,75, 1400,125, 1400,125);
    path.cubicTo(1400,195, 1320,205, 1320,205);
    path.cubicTo(1247,218, 1067,334, 1125,435);
    path.cubicTo(1140,465, 1170,480, 1200,485);
    path.cubicTo(1279,493, 1274,580, 1195,565);
    path.cubicTo(1145,559, 1070,542, 1040,595);
    path.cubicTo(872,737, 609,755, 400,790);

    //QGraphicsEllipseItem * myEllipse = myScene->addEllipse(500,250,600,300,blackPen, redBrush);
    //QGraphicsEllipseItem * myEllipse = ui->graphicsView->scene()->addEllipse(0,0,50,25,blackPen, redBrush);
    //QGraphicsLineItem * myLine = ui->graphicsView->scene()->addLine(Line,redPen);
    //ui->graphicsView->scene()->setBackgroundBrush(blueBrush);
    //myEllipse->setFlag(QGraphicsItem::ItemIsMovable);
    //QGraphicsEllipseItem * myEllipse = ui->graphicsView->scene()->addEllipse(0,0,50,25,blackPen, redBrush);
    //QGraphicsEllipseItem * pos1Dir1Segment1Ellipse = ui->graphicsView->scene()->addEllipse(38,794,10,10,blackPen, redBrush);
    //QGraphicsEllipseItem * pos2Dir1Segment1Ellipse = ui->graphicsView->scene()->addEllipse(58,796,10,10,blackPen, redBrush);
    //QGraphicsEllipseItem * pos2Dir1Segment1Ellipse = ui->graphicsView->scene()->addEllipse(0,0,100,100,blackPen, redBrush);

    qDebug("\n\n ==================== TEST !!!!! ========================\n\n");

    // 1st step: curve length
    float l = path.length();
    std::string s = std::to_string(l);
    qDebug() << "\n******* length " << " is " <<  s.c_str();

    QPointF qp2;
    QPointF qp3;


    for(int i = 0; i <= 200; i++)
    {
        qDebug() << "\n******* iteration" << " # " <<  i;
        qDebug() << "sublength" << " is " << l*i*0.005;

        // retrieve percent from length
        float p = path.percentAtLength(l*i*0.005);
        s = std::to_string(p);
        qDebug() << "percent at length" << " is " <<  s.c_str();

        // get angle at percent
        float angle = path.angleAtPercent(p);
        s = std::to_string(angle);
        qDebug() << "angle at percent "  << " is " <<  s.c_str();

        // get point at percent
        QPointF qp1 = path.pointAtPercent(p);
        s = std::to_string(qp1.rx());
        qDebug() << "x at percent "  << " is " <<  s.c_str();
        s = std::to_string(qp1.ry());
        qDebug() << "y at percent 1"  << " is " <<  s.c_str();

        // get slope at percent
        float slope = path.slopeAtPercent(p);
        s = std::to_string(slope);
        qDebug() << "slope at percent "  << " is " <<  s.c_str();

        // add point on curve
        path.addEllipse(qp1, 10, 10);

        //add train position for direction 1
        qp2.setX((qp1.rx()) - (20 * sin(angle/360*2*M_PI)));
        qp2.setY((qp1.ry()) - (20 * cos(angle/360*2*M_PI)));
        path.addEllipse(qp2, 10, 10);

        //add train position for direction 2
        qp3.setX((qp1.rx()) + (20 * sin(angle/360*2*M_PI)));
        qp3.setY((qp1.ry()) + (20 * cos(angle/360*2*M_PI)));
        path.addEllipse(qp3, 10, 10);
    }

    QGraphicsPathItem * myPath = ui->graphicsView->scene()->addPath(path, redPen);

    QGraphicsEllipseItem * e1 = ui->graphicsView->scene()->addEllipse(qp2.rx()-10/sqrt(2),qp2.ry()-10/sqrt(2),10*sqrt(2),10*sqrt(2),blackPen, blueBrush);
    QGraphicsEllipseItem * e2 = ui->graphicsView->scene()->addEllipse(qp3.rx()-10/sqrt(2),qp3.ry()-10/sqrt(2),10*sqrt(2),10*sqrt(2),blackPen, blueBrush);
    e1->setFlags(QGraphicsItem::ItemIsMovable);
    e2->setFlags(QGraphicsItem::ItemIsMovable);
    e1->setBrush(redBrush);
    e1->update();
    //e1->moveBy(20,20);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
    emit myclose();
    evt->accept();
}

void MainWindow::resizeEvent(QResizeEvent * evt)
{
    ui->graphicsView->adjustSize();
    ui->graphicsView->scale(1.01,1.01);
    evt->accept();
}
