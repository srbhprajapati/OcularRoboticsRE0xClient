/*
 * This thread contains the openGL Context for
 * rendering of sensed point cloud onto the UI
 * screen. CPU time saving OpenGL rendering thread.
 * This thread only renders when its corresponding
 * QGLWidget subclass render() method is called
 * (might be called from a different thread).
 */

#ifndef CLIENTGLTHREAD_H
#define CLIENTGLTHREAD_H

#include <QtOpenGL/qgl.h>
#include <qthread.h>
#include <qsize.h>
#include "udphost.h"

#include <QtNetwork/QUdpSocket>

//forward declaration of ClientGLWidget
class ClientGLWidget;

class ClientGLThread : public QThread
{
public:

    //Init an OpenGl render thread for the _glwidget QGL
    ClientGLThread(ClientGLWidget& _glw);

    // main() function of the thread.
    void run();

    // Signal the thread to exit the next time it is woken up.
    void stop();

    // Request a resize of the GL viewport.
    // This is usually called from the QGLWidgets resizeEvent() method.
    void resizeViewport(const QSize& _size);

    // Change settings for rendering.
    void setRotation( GLfloat _x, GLfloat _y, GLfloat _z);

    //Asks the udphost to send datagram for Full Field Scan Mode
    bool setFullFieldScan(int Azimuthal_value, int Scanline_value);

    //Asks the udphost to send datagram for Bounded Elevation Scan Mode
    bool setBoundedElevationScan(int Azimuthal_value, int Scanline_value, float upper_bound, float lower_bound);

    //Asks the udphost to send datagram for Region Scan Mode
    bool setRegionScan(int Azimuthal_value, int Scanline_value, float upper_bound, float lower_bound, float lAngular, float rAngular);

    //Asks the udphost to send datagram for starting the laser sensor
    bool start_laser(int Azimuthal_value, int Scanline_value);

    //Asks the udphost to send datagram for stopping the laser sensor
    bool stopLaserSensor();

    //Updates the array containing all the points. It replaces the existing
    //points with newly acquired ones once the whole array is filled up.
    void updateScene(QByteArray pointData);

    //Saves the acquired model to the file system
    void saveModel(QString path);

    //Opens a new Model as simulation Model
    void openModel(QString filename);


protected:

    // Init the GL environment.
    void initializeGL();

    // Handles resizes of the GL viewport.
    void resizeGL(int width, int height);

    // Does all the painting.
    void paintGL();

private:

    // Actually draws the Scene
    void draw();

    // The QGLWidget of the render thread.
    // This widget provides the GL rendering context.
    ClientGLWidget& glw;

    // Keep the thread running as long this flag is true.
    volatile bool render_flag;

    // Perform a resize when this flag is true.
    volatile bool resize_flag;

    // Current size of the viewport.
    QSize viewport_size;

    // example implmentation members
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;

    //UdpHost Object for sending UDP packets to Server
    UdpHost *_udp;

    //Array consisting of all the sensed points
    float *points;
    bool doRendering;
    int pointCounter;
};



#endif // CLIENTGLTHREAD_H
