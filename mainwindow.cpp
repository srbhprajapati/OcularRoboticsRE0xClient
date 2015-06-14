#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    socket = new QUdpSocket(this);

    //Bind Socket to an Address
    if(!socket->bind(QHostAddress::Any, 1235))
    {
        qDebug()<<"Unable to connect to Server";
    }

    QObject::connect(socket, SIGNAL(readyRead()),
                     this, SLOT(readPendingDatagrams()));



    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(on_openAction_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startLaserButton_clicked()
{
    int Azimuthal_value = ui->AzimuthalspinBox->value();
    int Scanline_value = ui->scanLineSpinBox->value();

    ui->AzimuthalFullScanLabel->setText(QString::number(Azimuthal_value));
    ui->ScanLineFullScanLabel->setText(QString::number(Scanline_value));

    ui->widget->start_laser(Azimuthal_value, Scanline_value);

    //Set Value to Text Labels
    ui->AzimuthalFullScanLabel->setText(QString::number(Azimuthal_value));
    ui->AzimuthalFreqLabel->setText(QString::number(Azimuthal_value));

    ui->ScanLineFullScanLabel->setText(QString::number(Scanline_value));
    ui->scanLinesValueLabel->setText(QString::number(Scanline_value));

    ui->samplingFreqValueLabel->setText(QString::number(5000));
    ui->TotalPointsValueLabel->setText(QString::number(100000));
    ui->LaserXValueLabel->setText(QString::number(ui->LaserXSpinBox->value()));
    ui->LaserYValueLabel->setText(QString::number(ui->LaserYSpinBox->value()));
    ui->LaserZValueLabel->setText(QString::number(ui->LaserZSpinBox->value()));

    ui->maxRangeValueLabel->setText(QString::number(10.0));

    ui->scanModeValueLabel->setText(QString("FS"));
    ui->LaserStatusLabel->setText(QString("ON"));

}

void MainWindow::on_stopLaserButton_clicked()
{
    ui->widget->stopLaserSensor();
    ui->LaserStatusLabel->setText(QString("OFF"));
}


void MainWindow::on_fullScanModeButton_clicked()
{
    int Azimuthal_value = ui->AzimuthalspinBox->value();
    int Scanline_value = ui->scanLineSpinBox->value();

    ui->AzimuthalFullScanLabel->setText(QString::number(Azimuthal_value));
    ui->ScanLineFullScanLabel->setText(QString::number(Scanline_value));

    ui->widget->setFullFieldScan(Azimuthal_value, Scanline_value);

    //Set Value to Text Labels
    ui->scanModeValueLabel->setText(QString("FS"));

    ui->AzimuthalFullScanLabel->setText(QString::number(Azimuthal_value));
    ui->AzimuthalFreqLabel->setText(QString::number(Azimuthal_value));

    ui->ScanLineFullScanLabel->setText(QString::number(Scanline_value));
    ui->scanLinesValueLabel->setText(QString::number(Scanline_value));
}


void MainWindow::on_boundedElevationModeButton_clicked()
{
    float upper_bound = ui->UpperBoundSpinBox->value();
    float lower_bound = ui->LowerBoundSpinBox->value();

    ui->widget->setBoundedElevationScan(upper_bound, lower_bound);
    ui->scanModeValueLabel->setText(QString("BES"));
}

void MainWindow::on_regionScanModeButton_clicked()
{
    float upper_bound = ui->UpperBoundRegionSpinBox->value();
    float lower_bound = ui->LowerBoundRegionSpinBox->value();
    float lAngular = ui->AngularLeftSpinBox->value();
    float rAngular = ui->AngularRightSpinBox->value();

    ui->widget->setRegionScan(upper_bound, lower_bound, lAngular, rAngular);
    ui->scanModeValueLabel->setText(QString("RS"));
}



void MainWindow::on_openAction_clicked()
{
    qDebug()<<"Open Action button Clicked";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/", tr("Data Files(*.obj, *.ply)"));
}


void MainWindow::readPendingDatagrams()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender = QHostAddress::LocalHost;
        quint16 senderPort = 1235;

        socket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);


        ui->widget->updateScene(datagram);
/*
        QDataStream dStream(&datagram, QIODevice::ReadOnly);
        float x=0, y=0, z=0;
            float x1=0, y1=0, z1=0;

        dStream>>x;
        dStream>>y;
        dStream>>z;

        dStream>>x1;
        dStream>>y1;
        dStream>>z1;

        qDebug()<<"Datagram : "<<x<<" "<<y<<" "<<z<<" "<<x1<<" "<<y1<<" "<<z1;*/
        //i++;
    }

}
