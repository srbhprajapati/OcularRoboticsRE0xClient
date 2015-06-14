/*
 * This class is a widget for rendering of sensed
 * data received through UDP from the server. This class
 * ideally can create multiple OpenGL rendering threads
 * such that the current rendering context can be passed
 * to any of the thread in which we want the rendering to
 * happen. However we are using only one thread in which
 * we are rendering the sensed data.
 *
 */

#ifndef CLIENTGLWIDGET_H
#define CLIENTGLWIDGET_H


#include <QtOpenGL/QGLWidget>
#include "clientglthread.h"
#include <qmutex.h>
#include <qwaitcondition.h>


class ClientGLWidget : public QGLWidget
{
public:
    ClientGLWidget(QWidget *parent = 0);

    //Starts the rendering thread.
    //This method is called in the contructor.
    void initRendering();

    //Stops the rendering thread of the widget.
    void finishRendering();

    // Call this method before doing any OpenGL rendering from a thread.
    // This method will aquire the GL rendering context for the calling thread.
    // Rendering will only be possible from this thread until unlockGLContext()
    // is called from the same thread.
    void lockGLContext();

    //Call this method to release the rendering context again after calling lockGLContext().
    void unlockGLContext();

    // Returns a reference to the render wait condition.
    // This is only for internal purpose (render thread communication)
    QWaitCondition& renderCondition();

    // Returns a reference to the render context mutex.
    // This is only for internal purpose (render thread communication)
    QMutex& renderMutex();

    //Updates the array containing all the points. It replaces the existing
    //points with newly acquired ones once the whole array is filled up.
    void updateScene(QByteArray pointData);


    //Asks the udphost to send datagram for Full Field Scan
    bool setFullFieldScan(int Azimuthal_value, int Scanline_value);

    //Asks the udphost to send datagram for Bounded Elevation Scan
    bool setBoundedElevationScan(float upper_bound, float lower_bound);

    //Asks the udphost to send datagram for Region Scan
    bool setRegionScan(float upper_bound, float lower_bound, float lAngular, float rAngular);

    //Asks the udphost to send datagram for starting the laser sensor
    bool start_laser(int Azimuthal_value, int Scanline_value);

    //Asks the udphost to send datagram for stopping the laser sensor
    bool stopLaserSensor();


public slots:

    //Cause the rendering thread to render one frame of the OpenGL scene.
    // This method is thread save.
    // \warning If the rendering thread is currently rendering (not idle) when this method is called
    // NO additional new frame will be rendered afterwards!
    void render();


protected:

    // Handles mouse press events on the QGLWidget.
    void mousePressEvent(QMouseEvent *event);

    // Handles mouse move events on the QGLWidget.
    void mouseMoveEvent(QMouseEvent *event);

    // Handles mouse double click events on the QGLWidget.
    void mouseDoubleClickEvent(QMouseEvent *event);

    // Performs a save shutdown if the widget recieves a close event.
    void closeEvent(QCloseEvent* _e);

    // Calls render() if the widget recieves a paint event.
    void paintEvent(QPaintEvent*);

    // Requests a GL viewport resize from the rendering thread.
    void resizeEvent(QResizeEvent* _e);


private:

    // The rendering thread.
    ClientGLThread glt;

    // Mutex for protecting the GL rendering context for multithreading.
    QMutex render_mutex;

    // The rendering thread uses this wait condition to save CPU ressources.
    QWaitCondition render_condition;

    // for the example implementation
    QPoint lastPos;
};


#endif // CLIENTGLWIDGET_H
