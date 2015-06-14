#include "clientglwidget.h"
#include <qcolordialog.h>
#include <QMouseEvent>

ClientGLWidget::ClientGLWidget(QWidget *parent)
    : QGLWidget(parent),
    glt(*this)

{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    // Buffer swap is handled in the rendering thread
    setAutoBufferSwap(false);

    // start the rendering thread
    initRendering();
}


void ClientGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void ClientGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    // modify scence variables and render the next frame
    GLfloat dx = (GLfloat)(event->x() - lastPos.x()) / width();
    GLfloat dy = (GLfloat)(event->y() - lastPos.y()) / height();

    if (true)//event->button() & Qt::LeftButton)
    {
        glt.setRotation(180 * dy, 180 * dx, 0.0);
        render();
    }
    else if (event->button() & Qt::RightButton)
    {
        glt.setRotation(180 * dy,0.0,180 * dx);
        render();
    }
    lastPos = event->pos();
}

void ClientGLWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    // implement Mouse Double Click Event here
}

void ClientGLWidget::initRendering( )
{
    doneCurrent();
    context()->moveToThread((QThread*)&glt);

    // start the rendering thread
    glt.start();
    // wake the waiting render thread
    renderCondition().wakeAll();
}

void ClientGLWidget::finishRendering( )
{
    // request stopping
    glt.stop();
    // wake up render thread to actually perform stopping
    renderCondition().wakeAll();
    // wait till the thread has exited
    glt.wait();
}

void ClientGLWidget::closeEvent( QCloseEvent * _e )
{
    // request stopping
    finishRendering();
    // close the widget (base class)
    QGLWidget::closeEvent(_e);
}

void ClientGLWidget::paintEvent( QPaintEvent * )
{
   render();
}

void ClientGLWidget::resizeEvent( QResizeEvent * _e )
{
    // signal the rendering thread that a resize is needed
    glt.resizeViewport(_e->size());

    render();
}

void ClientGLWidget::lockGLContext( )
{
    // lock the render mutex for the calling thread
    renderMutex().lock();
    // make the render context current for the calling thread
    makeCurrent();
}

void ClientGLWidget::unlockGLContext( )
{
    // release the render context for the calling thread
    // to make it available for other threads
    doneCurrent();
    // unlock the render mutex for the calling thread
    renderMutex().unlock();
}

void ClientGLWidget::render( )
{
    // let the wait condition wake up the waiting thread
    renderCondition().wakeAll();
}

QWaitCondition & ClientGLWidget::renderCondition( )
{
    return(render_condition);
}

QMutex & ClientGLWidget::renderMutex( )
{
    return(render_mutex);
}


bool ClientGLWidget::start_laser(int Azimuthal_value, int Scanline_value)
{
    return glt.start_laser(Azimuthal_value, Scanline_value);
}

bool ClientGLWidget::stopLaserSensor()
{
    return glt.stopLaserSensor();
}

bool ClientGLWidget::setFullFieldScan(int Azimuthal_value, int Scanline_value)
{
    return glt.setFullFieldScan(Azimuthal_value, Scanline_value);
}

bool ClientGLWidget::setBoundedElevationScan(float upper_bound, float lower_bound)
{
    return glt.setBoundedElevationScan(upper_bound, lower_bound);
}

bool ClientGLWidget::setRegionScan(float upper_bound, float lower_bound, float lAngular, float rAngular)
{
    return glt.setRegionScan(upper_bound, lower_bound, lAngular, rAngular);
}


void ClientGLWidget::updateScene(QByteArray pointData)
{
    glt.updateScene(pointData);
}
