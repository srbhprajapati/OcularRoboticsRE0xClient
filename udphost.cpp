#include "udphost.h"

#define LOCALHOST_IP QHostAddress::LocalHost
#define LOCALHHOST_PORT 1236

UdpHost::UdpHost()
{
    socket = new QUdpSocket(this);
    //socket->bind(QHostAddress::Any, 1234);
}


bool UdpHost::setFullFieldScan(int Azimuthal_value, int Scanline_value)
{
    QByteArray data;

    //send UDP packet to start Full Field Scan
    QDataStream dStream(&data, QIODevice::WriteOnly);

    data.clear();
    dStream.device()->reset();

    QString str = "RESFFS";
    dStream<<str<<Azimuthal_value<<Scanline_value;


    int bytesWritten = socket->writeDatagram(data, LOCALHOST_IP, LOCALHHOST_PORT);

    if(bytesWritten==-1) return false;
    else return true;
}

bool UdpHost::setBoundedElevationScan(float upper_bound, float lower_bound)
{
    QByteArray data;

    //send UDP packet to start Bounded Elevation Scan
    QDataStream dStream(&data, QIODevice::WriteOnly);

    data.clear();
    dStream.device()->reset();

    QString str = "RESBES";
    dStream<<str<<upper_bound<<lower_bound;


    int bytesWritten = socket->writeDatagram(data, LOCALHOST_IP, LOCALHHOST_PORT);

    if(bytesWritten==-1) return false;
    else return true;
}

bool UdpHost::setRegionScan(float upper_bound, float lower_bound, float lAngular, float rAngular)
{
    QByteArray data;

    //send UDP packet to start Region Scan
    QDataStream dStream(&data, QIODevice::WriteOnly);

    data.clear();
    dStream.device()->reset();

    QString str = "RESRES";
    dStream<<str<<upper_bound<<lower_bound<<lAngular<<rAngular;


    int bytesWritten = socket->writeDatagram(data, LOCALHOST_IP, LOCALHHOST_PORT);

    if(bytesWritten==-1) return false;
    else return true;
}

bool UdpHost::runLaserSensor(int Azimuthal_value, int Scanline_value)
{
    QByteArray data;
    QDataStream dStream(&data, QIODevice::WriteOnly);

    data.clear();
    dStream.device()->reset();

    //send UDP packet to run Laser Sensor
    QString str = "RERNLS";
    dStream<<str<<Azimuthal_value<<Scanline_value;

    int bytesWritten = socket->writeDatagram(data, LOCALHOST_IP, LOCALHHOST_PORT);

    if(bytesWritten==-1) return false;
    else return true;
}

bool UdpHost::stopLaserSensor()
{
    QByteArray data;

    //send UDP packet to stop laser
    data.append("RESTLS");

    int bytesWritten = socket->writeDatagram(data, LOCALHOST_IP, LOCALHHOST_PORT);

    if(bytesWritten==-1) return false;
    else return true;
}
