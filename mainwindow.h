/*  This class is
 *
 *
 *
 *
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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:


    //reads pending datagram sent by another Program at
    // a particular port
    void readPendingDatagrams();

private slots:
    void on_startLaserButton_clicked();

    void on_stopLaserButton_clicked();

    void on_fullScanModeButton_clicked();

    void on_openAction_clicked();

    void on_boundedElevationModeButton_clicked();

    void on_regionScanModeButton_clicked();

private:
    Ui::MainWindow *ui;

    QUdpSocket *socket;
};

#endif // MAINWINDOW_H
