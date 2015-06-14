
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
    faceColors[0] = "red";
    faceColors[1] = "green";
    faceColors[2] = "blue";
    faceColors[3] = "cyan";
    faceColors[4] = "yellow";
    faceColors[5] = "magenta";

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

        // swap the buffers of the GL widget
        //glw.swapBuffers();

        //glw.doneCurrent(); // release the GL render context to make picking work!

        // wait until the gl widget says that there is something to render
        // glwidget.lockGlContext() had to be called before (see top of the function)!
        // this will release the render mutex until the wait condition is met
        // and will lock the render mutex again before exiting
        // waiting this way instead of insane looping will not waste any CPU ressources
        //glw.renderCondition().wait(&glw.renderMutex());

        //glw.makeCurrent(); // get the GL render context back

        // DEACTIVATED -- alternatively render a frame after a certain amount of time
        // prevent to much continous rendering activity
        // msleep(16); //sleep for 16 ms
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
//    _udp->sendData();
    // draws the cube
    /*
    static const GLfloat coords[6][4][3] =
        {
            {
                {
                    +1.0, -1.0, +1.0
                }
                , { +1.0, -1.0, -1.0 },
                { +1.0, +1.0, -1.0 }, { +1.0, +1.0, +1.0 }
            },
            { { -1.0, -1.0, -1.0 }, { -1.0, -1.0, +1.0 },
              { -1.0, +1.0, +1.0 }, { -1.0, +1.0, -1.0 } },
            { { +1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 },
              { -1.0, +1.0, -1.0 }, { +1.0, +1.0, -1.0 } },
            { { -1.0, -1.0, +1.0 }, { +1.0, -1.0, +1.0 },
              { +1.0, +1.0, +1.0 }, { -1.0, +1.0, +1.0 } },
            { { -1.0, -1.0, -1.0 }, { +1.0, -1.0, -1.0 },
              { +1.0, -1.0, +1.0 }, { -1.0, -1.0, +1.0 } },
            { { -1.0, +1.0, +1.0 }, { +1.0, +1.0, +1.0 },
              { +1.0, +1.0, -1.0 }, { -1.0, +1.0, -1.0 } }
        };

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

*/
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

/*
    for (int i = 0; i < 6; ++i)
    {
        // assign names for each surface
        // this make picking work
        glLoadName(i);
        glBegin(GL_QUADS);
        glw.qglColor(faceColors[i]);
        for (int j = 0; j < 4; ++j)
        {
            glVertex3f(coords[i][j][0], coords[i][j][1],
                       coords[i][j][2]);
        }
        glEnd();
    }

*/
}

int ClientGLThread::faceAtPosition(const QPoint &pos)
{
    // we need to lock the rendering context
    glw.lockGLContext();

    // this is the same as in every OpenGL picking example
    const int MaxSize = 512; // see below for an explanation on the buffer content
    GLuint buffer[MaxSize];
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(MaxSize, buffer);
    // enter select mode
    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix((GLdouble)pos.x(),
                  (GLdouble)(viewport[3] - pos.y()),
                  5.0, 5.0, viewport);
    GLfloat x = (GLfloat)viewport_size.width() / viewport_size.height();
    glFrustum(-x, x, -1.0, 1.0, 4.0, 15.0);
    draw();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();


    // finally release the rendering context again
    if (!glRenderMode(GL_RENDER))
    {
        glw.unlockGLContext();
        return -1;
    }
    glw.unlockGLContext();

    // Each hit takes 4 items in the buffer.
    // The first item is the number of names on the name stack when the hit occured.
    // The second item is the minimum z value of all the verticies that intersected
    // the viewing area at the time of the hit. The third item is the maximum z value
    // of all the vertices that intersected the viewing area at the time of the hit
    // and the last item is the content of the name stack at the time of the hit
    // (name of the object). We are only interested in the object name
    // (number of the surface).

    // return the name of the clicked surface
    return buffer[3];
}

void ClientGLThread::setRotation( GLfloat _x, GLfloat _y, GLfloat _z )
{
    rotationX += _x;
    rotationY += _y;
    rotationZ += _z;
}

void ClientGLThread::sendData()
{
//    QByteArray Data;
//    QDataStream dStream(&Data, QIODevice::WriteOnly);

//    for(int i=0; i<100; i++)
//    {
////        QString str = "Packet";
////        str.append(QString::number(i));
////        Data.append(str);
//        for(int j=0; j<10; j++)
//        {

//            //Data.append(";");

//            qint16 x = (qrand()%(36000)) + 0.12341234;
//            qint16 y = (qrand()%(36000))+ 0.12341234;
//            qint16 z = (qrand()%(36000))+ 0.12341234;


//            dStream<<x;
//            dStream<<y;
//            dStream<<z;

//        }
//        int bytesWritten = socket->writeDatagram(Data, QHostAddress::LocalHost, 1234);
//        int size = Data.size();
//        Data.clear();
//    }


//    QByteArray tempBa;
//    QDataStream tempDs(&tempBa, QIODevice::WriteOnly);

//    qint16 x = (qrand()%(36000)) + 0.12341234;
//    qint16 y = (qrand()%(36000))+ 0.12341234;
//    qint16 z = (qrand()%(36000))+ 0.12341234;

//    tempDs<<x;
//    tempDs<<y;
//    tempDs<<z;


//    QDataStream temp1Ds(&tempBa, QIODevice::ReadOnly);

//    qint16 x_trans;
//    qint16 y_trans;
//    qint16 z_trans;

//    temp1Ds>>x_trans;
//    temp1Ds>>y_trans;
//    temp1Ds>>z_trans;

//    qDebug()<<"akjfsdlkas";
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
