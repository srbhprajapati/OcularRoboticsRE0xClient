#ifndef CLIENTGLTHREAD_H
#define CLIENTGLTHREAD_H

#include <QtOpenGL/qgl.h>
#include <qthread.h>
#include <qsize.h>
#include "udphost.h"

#include <QtNetwork/QUdpSocket>
class ClientGLWidget;

/** CPU time saving OpenGL rendering thread.
 * This thread only renders when its corresponding QGLWidget subclass
 * render() method is called (might be called from a different thread).
 */
class ClientGLThread : public QThread
{
public:

    /** Init an OpenGl render thread for the _glwidget QGL */
    ClientGLThread(ClientGLWidget& _glw);

    /** main() function of the thread. */
    void run();

    /** Signal the thread to exit the next time it is woken up. */
    void stop();

    /** Request a resize of the GL viewport.
     * This is usually called from the QGLWidgets resizeEvent() method.
     */
    void resizeViewport(const QSize& _size);

    /** Change settings for rendering. */
    void setRotation( GLfloat _x, GLfloat _y, GLfloat _z);

    /** Returns the color of a cube face.
     * This function can be called from different threads!
     */
    int faceAtPosition(const QPoint &pos);

    /** The six face colors of the cube
     * This should not be public!
     */
    QColor faceColors[6];


    /// \brief  Sends the Data through UDP Socket
    void sendData();





    //RESFFS
    bool setFullFieldScan(int Azimuthal_value, int Scanline_value);

    //RESBES
    bool setBoundedElevationScan(float upper_bound, float lower_bound);

    //RESRES
    bool setRegionScan(float upper_bound, float lower_bound, float lAngular, float rAngular);

    //RERNLS
    bool start_laser(int Azimuthal_value, int Scanline_value);

    //RESTLS
    bool stopLaserSensor();


    void updateScene(QByteArray pointData);


protected:
    /** Init the GL environment. */
    void initializeGL();
    /** Handles resizes of the GL viewport. */
    void resizeGL(int width, int height);
    /** Does all the painting. */
    void paintGL();

private:
    /** Actually draws the example scene (cube). */
    void draw();
    /** The QGLWidget of the render thread.
     * This widget provides the GL rendering context.
     */
    ClientGLWidget& glw;
    /** Keep the thread running as long this flag is true. */
    volatile bool render_flag;
    /** Perform a resize when this flag is true. */
    volatile bool resize_flag;
    /** Current size of the viewport. */
    QSize viewport_size;

    // example implmentation members
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;

    UdpHost *_udp;

    float *points;
    bool doRendering;
    int pointCounter;
};



#endif // CLIENTGLTHREAD_H
