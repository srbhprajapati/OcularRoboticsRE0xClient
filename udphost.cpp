#include "udphost.h"
#include <cstdlib>
#include <iostream>

#define LOCALHOST_IP QHostAddress::LocalHost
#define LOCALHHOST_PORT 1236

UdpHost::UdpHost()
{
    socket = new QUdpSocket(this);
    //socket->bind(QHostAddress::Any, 1234);
}


bool UdpHost::setFullFieldScan(int Azimuthal_value, int Scanline_value)
{

    //send UDP packet to start Full Field Scan
    char* dataPacket = new char[10];

    //Header
    memcpy(dataPacket, "RESFFS", 6);

    //Azimuthal Frequency
    unsigned int azimuthalFreq = (unsigned int)Azimuthal_value;
    unsigned char* azimuthalFreqChar = new unsigned char[2];
    _intToChar(azimuthalFreq, azimuthalFreqChar);
    char* azimuthalFreqBytes = reinterpret_cast<char*>(azimuthalFreqChar);
    memcpy(dataPacket+6, azimuthalFreqBytes, 2);

    //Number of Scanlines
    unsigned int scanline = (unsigned int)Scanline_value;
    unsigned char* scanlineChar = new unsigned char[2];
    _intToChar(scanline, scanlineChar);
    char* scanlineBytes = reinterpret_cast<char*>(scanlineChar);
    memcpy(dataPacket+8, scanlineBytes, 2);


    const char* finalDataPacket = &dataPacket[0];


    int bytesWritten = socket->writeDatagram(finalDataPacket, 10, LOCALHOST_IP, LOCALHHOST_PORT);

    if(bytesWritten==-1) return false;
    else return true;
}

bool UdpHost::setBoundedElevationScan(int Azimuthal_value, int Scanline_value, float upper_bound, float lower_bound)
{

    //send UDP packet to start Full Field Scan
    char* dataPacket = new char[14];

    //Header
    memcpy(dataPacket, "RESBES", 6);

    //Azimuthal Frequency
    unsigned int azimuthalFreq = (unsigned int)Azimuthal_value;
    unsigned char* azimuthalFreqChar = new unsigned char[2];
    _intToChar(azimuthalFreq, azimuthalFreqChar);
    char* azimuthalFreqBytes = reinterpret_cast<char*>(azimuthalFreqChar);
    memcpy(dataPacket+6, azimuthalFreqBytes, 2);

    //Minimum Elevation
    short minElevation = (short)(lower_bound*100);
    unsigned char* minElevationChar = new unsigned char[2];
    _shortToChar(minElevation, minElevationChar);
    char* minElevationBytes = reinterpret_cast<char*>(minElevationChar);
    memcpy(dataPacket+8, minElevationBytes, 2);

    //Maximum Elevation
    short maxElevation = (short)(upper_bound*100);
    unsigned char* maxElevationChar = new unsigned char[2];
    _shortToChar(maxElevation, maxElevationChar);
    char* maxElevationBytes = reinterpret_cast<char*>(maxElevationChar);
    memcpy(dataPacket+10, maxElevationBytes, 2);

    //Number of Scanlines
    unsigned int scanline = (unsigned int)Scanline_value;
    unsigned char* scanlineChar = new unsigned char[2];
    _intToChar(scanline, scanlineChar);
    char* scanlineBytes = reinterpret_cast<char*>(scanlineChar);
    memcpy(dataPacket+12, scanlineBytes, 2);


    const char* finalDataPacket = &dataPacket[0];


    int bytesWritten = socket->writeDatagram(finalDataPacket, 14, LOCALHOST_IP, LOCALHHOST_PORT);

    if(bytesWritten==-1) return false;
    else return true;
}

bool UdpHost::setRegionScan(int Azimuthal_value, int Scanline_value, float upper_bound, float lower_bound, float lAngular, float rAngular)
{

    //send UDP packet to start Region Scan
    char* dataPacket = new char[18];

    //Header
    memcpy(dataPacket, "RESRES", 6);

    //Minumum Azimuth Angle
    unsigned int minAzimuth = (unsigned int)(rAngular*100);
    unsigned char* minAzimuthChar = new unsigned char[2];
    _intToChar(minAzimuth, minAzimuthChar);
    char* minAzimuthBytes = reinterpret_cast<char*>(minAzimuthChar);
    memcpy(dataPacket+6, minAzimuthBytes, 2);

    //Maxiumum Azimuth Angle
    unsigned int maxAzimuth = (unsigned int)(lAngular*100);
    unsigned char* maxAzimuthChar = new unsigned char[2];
    _intToChar(maxAzimuth, maxAzimuthChar);
    char* maxAzimuthBytes = reinterpret_cast<char*>(maxAzimuthChar);
    memcpy(dataPacket+8, maxAzimuthBytes, 2);

    //Minimum Elevation
    short minElevation = (short)(lower_bound*100);
    unsigned char* minElevationChar = new unsigned char[2];
    _shortToChar(minElevation, minElevationChar);
    char* minElevationBytes = reinterpret_cast<char*>(minElevationChar);
    memcpy(dataPacket+10, minElevationBytes, 2);

    //Maximum Elevation
    short maxElevation = (short)(upper_bound*100);
    unsigned char* maxElevationChar = new unsigned char[2];
    _shortToChar(maxElevation, maxElevationChar);
    char* maxElevationBytes = reinterpret_cast<char*>(maxElevationChar);
    memcpy(dataPacket+12, maxElevationBytes, 2);


    //Azimuthal Frequency
    unsigned int azimuthalFreq = (unsigned int)Azimuthal_value;
    unsigned char* azimuthalFreqChar = new unsigned char[2];
    _intToChar(azimuthalFreq, azimuthalFreqChar);
    char* azimuthalFreqBytes = reinterpret_cast<char*>(azimuthalFreqChar);
    memcpy(dataPacket+14, azimuthalFreqBytes, 2);


    //Number of Scanlines
    unsigned int scanline = (unsigned int)Scanline_value;
    unsigned char* scanlineChar = new unsigned char[2];
    _intToChar(scanline, scanlineChar);
    char* scanlineBytes = reinterpret_cast<char*>(scanlineChar);
    memcpy(dataPacket+16, scanlineBytes, 2);


    const char* finalDataPacket = &dataPacket[0];


    int bytesWritten = socket->writeDatagram(finalDataPacket, 18, LOCALHOST_IP, LOCALHHOST_PORT);

    if(bytesWritten==-1) return false;
    else return true;
}

bool UdpHost::runLaserSensor(int Azimuthal_value, int Scanline_value)
{

    char* dataPacket = new char[13];


    //Header
    memcpy(dataPacket, "RERNLS", 6);

    //Sampling Frequency
    unsigned int samplingFreq = 10000;
    unsigned char* samplingFreqChar = new unsigned char[2];
    _intToChar(samplingFreq, samplingFreqChar);
    char* samplingFreqBytes = reinterpret_cast<char*>(samplingFreqChar);
    memcpy(dataPacket+6, samplingFreqBytes, 2);

    //Number of Samples to Average in hardware
    memcpy(dataPacket+8, "--", 2);

    //Intensity
    memcpy(dataPacket+10, "0", 1);

    //Port
    unsigned int port = 1236;
    unsigned char* portChar = new unsigned char[2];
    _intToChar(port, portChar);
    char* portBytes = reinterpret_cast<char*>(portChar);
    memcpy(dataPacket+11, portBytes, 2);

    const char* finalDataPacket = &dataPacket[0];

    int bytesWritten = socket->writeDatagram(finalDataPacket, 13, LOCALHOST_IP, LOCALHHOST_PORT);

    if(bytesWritten==-1) return false;
    else return true;
}

bool UdpHost::stopLaserSensor()
{
    //send UDP packet to stop laser
    char* dataPacket = new char[13];

    //Header
    memcpy(dataPacket, "RESTLS", 6);

    const char* finalDataPacket = &dataPacket[0];

    int bytesWritten = socket->writeDatagram(finalDataPacket, 6, LOCALHOST_IP, LOCALHHOST_PORT);

    if(bytesWritten==-1) return false;
    else return true;
}


void UdpHost::_intToChar(unsigned int &num, unsigned char* array)
{
    array[0] = num & 0xFF;
    array[1] = (num>>8) & 0xFF;
    array[2] = (num>>16) & 0xFF;
    array[3] = (num>>24) & 0xFF;
}


void UdpHost::_shortToChar(short &num, unsigned char* array)
{
    array[0] = num & 0xFF;
    array[1] = (num>>8) & 0xFF;
}


bool UdpHost::openModel(QString filename)
{
    //send UDP packet to stop laser
    char* dataPacket = new char[6 + filename.length()];

    //Header
    memcpy(dataPacket, "RECHMD", 6);

//    const char* filenamePointer = filename.toLatin1().constData();

//    memcpy(dataPacket+6, filenamePointer, filename.length());

//    const char* finalDataPacket = &dataPacket[0];

//    std::string s = finalDataPacket;

    for(int i=0; i<filename.length(); i++)
    {
        dataPacket[6+i] = filename.at(i).toLatin1();
    }



    const char* finalDataPacket = &dataPacket[0];
    int bytesWritten = socket->writeDatagram(finalDataPacket, 6+filename.length(), LOCALHOST_IP, LOCALHHOST_PORT);

    if(bytesWritten==-1) return false;
    else return true;
}



bool UdpHost::saveModel(QString filename)
{
    //send UDP packet to stop laser
    char* dataPacket = new char[6 + filename.length()];

    //Header
    memcpy(dataPacket, "RESVMD", 6);
    for(int i=0; i<filename.length(); i++)
    {
        dataPacket[6+i] = filename.at(i).toLatin1();
    }



    const char* finalDataPacket = &dataPacket[0];
    int bytesWritten = socket->writeDatagram(finalDataPacket, 6+filename.length(), LOCALHOST_IP, LOCALHHOST_PORT);

    if(bytesWritten==-1) return false;
    else return true;
}
