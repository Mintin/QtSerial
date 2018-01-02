#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QTime>
#include <QDebug>
#include "osapi/Thread.h"




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
  //  myThread thed;
    // thed.Run();


 //qInstallMessageHandler(myMessageOutput);
    w.show();
  // w.WriteSerial(data,5);

    return a.exec();
}
