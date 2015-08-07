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
    //Read Azimuthal and Elevation Value
    int Azimuthal_value = ui->AzimuthalspinBox->value();
    int Scanline_value = ui->scanLineSpinBox->value();

    //Set Azimuthal and Elevation Value to display tag
    ui->AzimuthalFullScanLabel->setText(QString::number(Azimuthal_value));
    ui->ScanLineFullScanLabel->setText(QString::number(Scanline_value));

    //For starting the rendering in ClientGLWidget
    ui->widget->start_laser(Azimuthal_value, Scanline_value);


    //Set Value to Other Text Labels
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
    //For stopping the rendering in ClientGLWidget
    ui->widget->stopLaserSensor();

    //Set Value to Other Text Labels
    ui->LaserStatusLabel->setText(QString("OFF"));
}


void MainWindow::on_fullScanModeButton_clicked()
{
    //Read Azimuthal and Elevation Value
    int Azimuthal_value = ui->AzimuthalspinBox->value();
    int Scanline_value = ui->scanLineSpinBox->value();

    //Set Azimuthal and Elevation Value to display tag
    ui->AzimuthalFullScanLabel->setText(QString::number(Azimuthal_value));
    ui->ScanLineFullScanLabel->setText(QString::number(Scanline_value));

    //call Full Field Scan method in ClientGLWidget
    ui->widget->setFullFieldScan(Azimuthal_value, Scanline_value);

    //Set Value to Other Text Labels
    ui->scanModeValueLabel->setText(QString("FS"));

    ui->AzimuthalFullScanLabel->setText(QString::number(Azimuthal_value));
    ui->AzimuthalFreqLabel->setText(QString::number(Azimuthal_value));

    ui->ScanLineFullScanLabel->setText(QString::number(Scanline_value));
    ui->scanLinesValueLabel->setText(QString::number(Scanline_value));
}


void MainWindow::on_boundedElevationModeButton_clicked()
{

    //Read Azimuthal and Elevation Value
    int Azimuthal_value = ui->AzimuthalspinBox->value();
    int Scanline_value = ui->scanLineSpinBox->value();

    //Read Upper and Lower Bound Values
    float upper_bound = ui->UpperBoundSpinBox->value();
    float lower_bound = ui->LowerBoundSpinBox->value();

    //call Bounded Elevation Scan method in ClientGLWidget
    ui->widget->setBoundedElevationScan(Azimuthal_value, Scanline_value, upper_bound, lower_bound);

    //Set Value to Other Text Labels
    ui->scanModeValueLabel->setText(QString("BES"));
}

void MainWindow::on_regionScanModeButton_clicked()
{
    //Read Azimuthal and Elevation Value
    int Azimuthal_value = ui->AzimuthalspinBox->value();
    int Scanline_value = ui->scanLineSpinBox->value();

    //Read Upper Bound, LowerBound, Left Bound, and Right Bound from the UI
    float upper_bound = ui->UpperBoundRegionSpinBox->value();
    float lower_bound = ui->LowerBoundRegionSpinBox->value();
    float lAngular = ui->AngularLeftSpinBox->value();
    float rAngular = ui->AngularRightSpinBox->value();

    //call Bounded Elevation Scan method in ClientGLWidget
    ui->widget->setRegionScan(Azimuthal_value, Scanline_value, upper_bound, lower_bound, lAngular, rAngular);

    //Set Value to Other Text Labels
    ui->scanModeValueLabel->setText(QString("RS"));
}


//Funtion not used currently
//Reason : this function is intentionally no working because it makes
//          more sense to change the model from server only.
void MainWindow::on_openAction_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/", tr("Data Files( *.obj *.ply)"));
    if(!filename.isEmpty())
    {
        ui->widget->openModel(filename);
    }

}


void MainWindow::readPendingDatagrams()
{
    //while data is available
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender = QHostAddress::LocalHost;
        quint16 senderPort = 1235;

        socket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        //render the data
        ui->widget->updateScene(datagram);
    }

}


void MainWindow::on_actionSave_triggered()
{

    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "C:/", tr("Data Files(*.pcd)"));

    if(!filename.isEmpty())
    {
        ui->widget->saveModel(filename);
    }

}
