/*
 * Application Name: OcularRoboticsClient
 * Summary:
 *      This application is used to control and interact
 *      with the virtual RE05 laser sensor(by Ocular Robotics)
 *      whose implementation is present in the simulation
 *      module of the PCL library.
 *           On running this Application the user will see a
 *      GUI window which will have all the respective functions
 *      for giving commands to the RE05 laser sensor simulation.
 *      The main screen on the left hand side of the GUI will show
 *      the point cloud of the sensed virtual data. The data is
 *      transferred from the Server(RE05 Sensor Simulator) to this
 *      application though UDP. All the communication and messages
 *      are sent by this application through the class object
 *      'udphost'.
 *          This application is majorly created using QtGui and
 *      QtOpenGL module. This application spawns a thread for seperate
 *      rendering of sensed data so that the User Interaction of the
 *      GUI remains free.
 *
 * */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow RE05Client;
    RE05Client.show();

    return a.exec();
}
