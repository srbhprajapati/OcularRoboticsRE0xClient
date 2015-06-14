/*
 * This class is a UDP sender which sends
 * packets containing control instructions for
 * RE05 laser sensor.
 *
 */

#ifndef UDPHOST_H
#define UDPHOST_H

#include <QtNetwork/QUdpSocket>

class UdpHost : public QObject
{    
    public:
        UdpHost();

        //Sends datagram for Full Field Scan mode
        bool setFullFieldScan(int Azimuthal_value, int Scanline_value);

        //Sends datagram for Bounded Elevation Scan mode
        bool setBoundedElevationScan(float upper_bound, float lower_bound);

        //Sends datagram for Region Scan mode
        bool setRegionScan(float upper_bound, float lower_bound, float lAngular, float rAngular);

        //Sends datagram for starting the laser sensor simulator
        bool runLaserSensor(int Azimuthal_value, int Scanline_value);

        //Sends datagram for stopping the sensor
        bool stopLaserSensor();

    private:

        //Socket for sending data
        QUdpSocket *socket;
};

#endif // UDPHOST_H
