#ifndef UDPHOST_H
#define UDPHOST_H

#include <QtNetwork/QUdpSocket>

class UdpHost : public QObject
{    
    public:
        UdpHost();

        void sendData();


        //RECQRE - do it later

        //RESIPC 10Char Serial[] IP-4

        //REHOME - do it later

        //RESTOP

        //RESEAC    Accelaration - 4

        //REGEAA

        //RESEAA    Az-2 Elev-2 Spd-2

        //RETRAA


        //RESFFS
        bool setFullFieldScan(int Azimuthal_value, int Scanline_value);

        //RESBES
        bool setBoundedElevationScan(float upper_bound, float lower_bound);

        //RESRES
        bool setRegionScan(float upper_bound, float lower_bound, float lAngular, float rAngular);

        //RERNLS
        bool runLaserSensor(int Azimuthal_value, int Scanline_value);

        //RESTLS
        bool stopLaserSensor();




    private:
        QUdpSocket *socket;
};

#endif // UDPHOST_H
