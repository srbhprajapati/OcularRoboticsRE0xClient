/*
 * This class contains the UI implementation of
 * the application. There are three files corresponding
 * to this class:
 *  1. Header   (mainwindow.h)
 *  2. Source   (mainwindow.cpp)
 *  3. Designer (mainwindow.ui)
 *
 * The object of this created in main.cpp holds a pointer
 * to ClientGLWidget (which is the main widget for rendering
 * Point Cloud data received over UDP from the Server). This
 * also receives virtually sensed data from the server and passes
 * it to the ClientGLWidget for rendering.
 *
 * */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    //Constructor
    explicit MainWindow(QWidget *parent = 0);

    //Destructor
    ~MainWindow();

public slots:

    //reads pending datagram sent by another Program at
    // a particular port
    void readPendingDatagrams();

private slots:

    //Slot Called when Start Laser Button is Clicked
    void on_startLaserButton_clicked();

    //Slot Called when Stop Laser Button is Clicked
    void on_stopLaserButton_clicked();

    //Slot Called when Full Scan Mode is selected
    void on_fullScanModeButton_clicked();

    //Slot Called when Bounded Elevation Scan Mode is selected
    void on_boundedElevationModeButton_clicked();

    //Slot Called when Region Scan Mode is selected
    void on_regionScanModeButton_clicked();

    //Slot Called when Open option is selected by the user for
    //loading a different model for sensing
    void on_openAction_clicked();


    void on_actionSave_triggered();

private:

    //Pointer to the designer form
    Ui::MainWindow *ui;

    //Socket for receiving sensed data from server
    QUdpSocket *socket;
};

#endif // MAINWINDOW_H
