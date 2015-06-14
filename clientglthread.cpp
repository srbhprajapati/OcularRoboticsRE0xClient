
#include "clientglthread.h"
#include "clientglwidget.h"
#include <QtOpenGL/QtOpenGL>
#include <gl/GLU.h>
#include <QtNetwork/QUdpSocket>

#include <QtCore/QDataStream>
#include <QtCore/QByteArray>


ClientGLThread::ClientGLThread(ClientGLWidget & _glw )
        : QThread(),
        glw(_glw),
        render_flag(true),
        resize_flag(false),
        viewport_size(_glw.size())
{
    // example implemenation init
    rotationX = 0;
    rotationY = 0;
    rotationZ = 0;

    //At a time the application will show maximum 100000 points
    // if number of points exceeds this, then newer points overwrites
    // older points
    points = new float [100000*3];

    pointCounter = 0;

    doRendering = true;

    _udp = new UdpHost();

}

void ClientGLThread::resizeViewport( const QSize& _size )
{
    // set size and flag to request resizing
    viewport_size = _size;
    resize_flag = true;
}

void ClientGLThread::stop( )
{
    // set flag to request thread to exit
    // REMEMBER: The thread needs to be woken up once
    // after calling this method to actually exit!
    render_flag = false;
}


void ClientGLThread::run( )
{
    // lock the render mutex of the Gl widget
    // and makes the rendering context of the glwidget current in this thread
    glw.lockGLContext();

    // general GL init
    initializeGL();

    // do as long as the flag is true
    while( render_flag )
    {
        // resize the GL viewport if requested
        if (resize_flag)
        {
            resizeGL(viewport_size.width(), viewport_size.height());
            resize_flag = false;
        }


        // render code goes here
        if(doRendering)
        {
            paintGL();
            // swap the buffers of the GL widget
            glw.swapBuffers();
        }
    }

    // unlock the render mutex before exit
    glw.unlockGLContext();
}

void ClientGLThread::initializeGL()
{
    // typical OpenGL init
    // see OpenGL documentation for an explanation
    glw.qglClearColor("black");
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void ClientGLThread::resizeGL(int width, int height)
{
    // nothing special
    // see OpenGL documentation for an explanation
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = (GLfloat)width / height;
    glFrustum(-x, x, -1.0, 1.0, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

void ClientGLThread::paintGL()
{
    // clear all and draw the scene
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}

void ClientGLThread::draw()
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();


        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-12.0f, 12.0f, -12.0f, 12.0f, 1.0f, 70.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(18.0f, 18.0f, 18.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f);

        glColor3f (1.0, 1.0, 1.0);

        glRotatef(rotationX, 1.0, 0.0, 0.0);
        glRotatef(rotationY, 0.0, 1.0, 0.0);
        glRotatef(rotationZ, 0.0, 0.0, 1.0);

    glBegin(GL_POINTS);

    for(int i=0; i<100000; i++)
        {
            glColor3f(points[3*i]*points[3*i], (points[3*i+1])*(points[3*i+1]), points[3*i+2]*points[3*i+2]);
            glVertex3f(points[3*i], points[3*i+1], points[3*i+2]);
        }
    glEnd();
}

void ClientGLThread::setRotation( GLfloat _x, GLfloat _y, GLfloat _z )
{
    rotationX += _x;
    rotationY += _y;
    rotationZ += _z;
}

bool ClientGLThread::start_laser(int Azimuthal_value, int Scanline_value)
{
    return _udp->runLaserSensor(Azimuthal_value, Scanline_value);
}

bool ClientGLThread::stopLaserSensor()
{
    return _udp->stopLaserSensor();
}

bool ClientGLThread::setFullFieldScan(int Azimuthal_value, int Scanline_value)
{
    return _udp->setFullFieldScan(Azimuthal_value, Scanline_value);
}

bool ClientGLThread::setBoundedElevationScan(float upper_bound, float lower_bound)
{
    return _udp->setBoundedElevationScan(upper_bound, lower_bound);
}

bool ClientGLThread::setRegionScan(float upper_bound, float lower_bound, float lAngular, float rAngular)
{
    return _udp->setRegionScan(upper_bound, lower_bound, lAngular, rAngular);
}


void ClientGLThread::updateScene(QByteArray pointData)
{

    doRendering = false;

    QDataStream dStream(&pointData, QIODevice::ReadOnly);
    float x=0, y=0, z=0;
    float x1=0, y1=0, z1=0;

    for(int i=0; i<100; i++)
    {
        if(pointCounter<299997)
        {
            dStream>>points[pointCounter];
            pointCounter++;
            dStream>>points[pointCounter];
            pointCounter++;
            dStream>>points[pointCounter];
            pointCounter++;
        }
        else
        {
            pointCounter=0;
            dStream>>points[pointCounter];
            pointCounter++;
            dStream>>points[pointCounter];
            pointCounter++;
            dStream>>points[pointCounter];
            pointCounter++;
        }
    }
    qDebug()<<"Datagram : "<<points[pointCounter-3]<<" "<<points[pointCounter-2]<<" "<<points[pointCounter-1];

    doRendering = true;
}
