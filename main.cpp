#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QTime>
#include <QDebug>
#include "osapi/Thread.h"



class myThread : public OS_Thread
{
  private:
    // 线程主函数
    virtual int Routine()
    {
        while(1)
        {
            qDebug()<<"A Thread is Running";
            //printf("A Thread is Running \r\n");
            OS_Thread::Msleep(1500);
        }
        return 0; // 正常退出
    }

public:
    bool m_quitflag;

};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    myThread thed;
     thed.Run();


 //qInstallMessageHandler(myMessageOutput);
    w.show();
  // w.WriteSerial(data,5);

    return a.exec();
}
