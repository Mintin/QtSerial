#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QDebug"
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include "ymodem.h"
#include <QThread>


bool  hexStatus=false;

/*
class myThread :public QThread
{
public:
    myThread(MainWindow *parent)
    {

    }
    myThread()
    {
   //connect(this, SIGNAL(finished()),this, SLOT(deleteLater()));

    }
 private:
    void run()
    {
      qDebug()<<"Hello myThread"<<endl;


    }
};
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
   // qInstallMsgHandler(customMessageHandler);
//      myThread my;
    initPort();
     ui->sendButton->setEnabled(false);
        ui->openFill->setEnabled(false);
        ui->updata->setEnabled(false);
    connect(ui->openFill, SIGNAL(clicked(bool)), this, SLOT(openFile()));
    connect(ui->updata, SIGNAL(clicked(bool)), this, SLOT(upData()));
      //paint.setColor(Color.rgb(0,173,173));
     //ui->textEdit->setStyleSheet("background-color:black;");
  //  ui->textEdit->setTextColor(QColor(200, 200, 200));

    //  connect(ui->checkBox,SIGNAL(signal()),receiver,SLOT(slot()));

   /*
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
       {
           QSerialPort serial;
           serial.setPort(info);
           if(serial.open(QIODevice::ReadWrite))
           {
               ui->PortBox->addItem(serial.portName());
               serial.close();
           }
       }
       //设置波特率下拉菜单默认显示第三项
       ui->BaudBox->setCurrentIndex(3);
       //关闭发送按钮的使能
       ui->sendButton->setEnabled(false);
       qDebug() << tr("界面设定成功！");
   // connect(ui->hello,SIGNAL(clicked()),this,SLOT(HelloWorld()));
*/

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::hexShow()
{
    if(ui->checkBox->checkState()== Qt::Checked)//Unchecked
    {

   hexStatus=true;
    qDebug()<<"CheckBox is Checked";
    }
  else
 {  hexStatus=false;
    qDebug()<<"CheckBox is Unchecked";
    }

    //bool  hexStatus=false;
/*
radioButton->setChecked(true);
//返回选择状态
bool radio_sel = radioButton->isChecked();
*/
// qDebug()<<"CheckBox is push";



}


void MainWindow::on_clearButton_clicked()
{
    ui->textEdit->clear();
}

  void MainWindow::on_sendButton_clicked()
  {
      serial->write(ui->textEdit_2->toPlainText().toLatin1());
  }

  void MainWindow::Read_Data()
  {


    QString myStrTemp;
    QByteArray  buf= serial->readAll();
     if(buf.isEmpty()) return;
          if(hexStatus==true)
          {
            buf=buf.toHex();
          }
          myStrTemp=QString::fromLocal8Bit( buf );
          ui->textEdit->moveCursor(QTextCursor::End);
          ui->textEdit->insertPlainText(myStrTemp);
          buf.clear();
  }

  void MainWindow::on_openButton_clicked()
  {
      if(ui->openButton->text()==tr("打开串口"))
      {
          serial = new QSerialPort;
          //设置串口名
          serial->setPortName(ui->PortBox->currentText());
          //打开串口
          serial->open(QIODevice::ReadWrite);
          //设置波特率
          serial->setBaudRate(ui->BaudBox->currentText().toInt());
          //设置数据位数
          serial->setBaudRate(115200);   //设置波特率
          serial->setDataBits(QSerialPort::Data8);     //设置数据位
          serial->setStopBits(QSerialPort::OneStop);     //设置停止位
          serial->setParity(QSerialPort::NoParity);     //设置校验位
          serial->setFlowControl(QSerialPort::NoFlowControl);//设置流控制

          /*
          switch(ui->BitNumBox->currentIndex())
          {
          case 8: serial->setDataBits(QSerialPort::Data8); break;
          default: break;
          }
          //设置奇偶校验
          switch(ui->ParityBox->currentIndex())
          {
          case 0: serial->setParity(QSerialPort::NoParity); break;
          default: break;
          }
          //设置停止位
          switch(ui->StopBox->currentIndex())
          {
          case 1: serial->setStopBits(QSerialPort::OneStop); break;
          case 2: serial->setStopBits(QSerialPort::TwoStop); break;
          default: break;
          }
          //设置流控制
          serial->setFlowControl(QSerialPort::NoFlowControl);
          */
               //serial->setBaudRate()
               // QSerialPort::

                //serial->setTimeout(500);         //设置超时时间
          //关闭设置菜单使能
          ui->PortBox->setEnabled(false);
          ui->BaudBox->setEnabled(false);
          ui->BitNumBox->setEnabled(false);
          ui->ParityBox->setEnabled(false);
          ui->StopBox->setEnabled(false);
          ui->openButton->setText(tr("关闭串口"));
          ui->sendButton->setEnabled(true);
          ui->openFill->setEnabled(true);
          ui->updata->setEnabled(true);
          //连接信号槽
          QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
      }
      else
      {
          //关闭串口
          serial->clear();
          serial->close();
          serial->deleteLater();
          //恢复设置使能
          ui->PortBox->setEnabled(true);
          ui->BaudBox->setEnabled(true);
          ui->BitNumBox->setEnabled(true);
          ui->ParityBox->setEnabled(true);
          ui->StopBox->setEnabled(true);
          ui->openFill->setEnabled(false);
          ui->updata->setEnabled(false);
          ui->openButton->setText(tr("打开串口"));
          ui->sendButton->setEnabled(false);
      }
  }

//  void MainWindow::on_openButton_clicked()
  void MainWindow::initPort()
  {
      //读取串口信息
         foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
         {
             qDebug()<<"Name:"<<info.portName();
             qDebug()<<"Description:"<<info.description();
             qDebug()<<"Manufacturer:"<<info.manufacturer();

             //这里相当于自动识别串口号之后添加到了cmb，如果要手动选择可以用下面列表的方式添加进去
             QSerialPort serial;
             serial.setPort(info);
             if(serial.open(QIODevice::ReadWrite))
             {
                 //将串口号添加到cmb
                 ui->PortBox->addItem(info.portName());
                 //关闭串口等待人为(打开串口按钮)打开
                 serial.close();
             }
         }

         QStringList baudList;//波特率
         QStringList parityList;//校验位
         QStringList dataBitsList;//数据位
         QStringList stopBitsList;//停止位

            baudList<<"1200"<<"2400"<<"4800"<<"9600"<<"14400"<<"19200"<<"38400"
                   <<"56000"<<"57600"<<"76800"<<"115200"<<"128000"<<"256000";

              ui->BaudBox->addItems(baudList);
              ui->BaudBox->setCurrentIndex(3);

                parityList<<"无"<<"奇"<<"偶";
                parityList<<"标志";
                parityList<<"空格";
                ui->ParityBox->addItems(parityList);
                ui->ParityBox->setCurrentIndex(0);
                dataBitsList<<"5"<<"6"<<"7"<<"8";
                ui->BitNumBox->addItems(dataBitsList);
                ui->BitNumBox->setCurrentIndex(3);
                stopBitsList<<"1";
                stopBitsList<<"1.5";
                 stopBitsList<<"2";
                 ui->StopBox->addItems(stopBitsList);
                  ui->StopBox->setCurrentIndex(0);
 connect(ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(hexShow()));

  }
   void MainWindow::WriteSerial(unsigned char *data,int len )
   {

          serial->write((char*)data,len);

   }

  void MainWindow::openFile()
{

     QString  fileName;
     QByteArray  tempName; // = str.toLatin1();

      //PathUp="Hello World";
      //updatPath  QString  FilePath
     PathUp= QFileDialog::getOpenFileName(this, tr("Open File"),QDir::currentPath(),tr("ALL Files (*.*)"));
     if (PathUp.isEmpty())     return;
     QFileInfo fileInfo(PathUp);
     fileName=fileInfo.fileName();
     tempName=fileName.toLatin1();
     strcpy(this->fileName,tempName.data());
     ui->BaudBox->setCurrentIndex(10);
/*

    QString str = “hello”; //QString转char *
     QByteArray ba = str.toLatin1();
    char *mm = ba.data();

*/



     //QString fpath = QFileDialog::FileName();
    // qDebug()<<"File Name:"<<fileName;

   // PathUp = QFileDialog::getOpenFileName(this, tr("Open File"),QDir::currentPath(),tr("ALL Files (*.*)"));
   //
  //  qDebug()<<"file Path:"<<PathUp;
  }




  class myThread:public QThread
  {

   public:
      myThread(MainWindow * p )
      {

     mate=p;
      }
      void run()
  {

       int TxLen=0;
        uint8_t TxData[1024+10]={0};
        QByteArray Path=mate->PathUp.toLatin1();
        Ymodem myYmdem;
        myYmdem.getFilePath("123","456");

        myYmdem.getFilePath(Path.data(),mate->fileName);
        myYmdem.YmodeInfo(TxData,&TxLen);
        mate->serial->write((char*)TxData,TxLen);
        qDebug()<<"Send updata info over";

  }

private: MainWindow* mate;
  };
  void MainWindow::upData()
 {



       myThread *thread=new  myThread(this);
       connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
       thread->wait();
       thread->start();

    //  myThread my;
    //  my.start();
      /*
      int TxLen=0;
        uint8_t TxData[1024+10]={0};
        QByteArray Path=PathUp.toLatin1();
        Ymodem myYmdem;
        myYmdem.getFilePath(Path.data(),this->fileName);
        myYmdem.YmodeInfo(TxData,&TxLen);
        serial->write((char*)TxData,TxLen);
*/
  //     qDebug()<<"upData:"<<this->fileName;
      //  serial->write((char*)TxData,TxLen);
     /*   for(int i=0;i<TxLen;i++)
        {
            printf("%02X ",TxData[i]);
        }
*/
        //printf("Hello World \r\n");
        //std::cout <<"%02x "<<TxData;
        //qDebug<<"%02X"
       // myYmdem.file_Pack(TxData,&TxLen);
       // serial->write((char*)TxData,TxLen);
       // myYmdem.sendOver(TxData,&TxLen);
       // serial->write((char*)TxData,TxLen);


       //myYmdem.YmodemStart(Path.data());
       //myYmdem.getFilePath(Path.data());




    //  serial->write((char*)data,strlen(data));



    // Ymodem myYmdem;
    // QByteArray Path=PathUp.toLatin1();
    // myYmdem.YmodemStart(Path.data());
     //qDebug()<<"MainWindow-upData"<<Path.data();
   //  char path[50]={0};
    //  Ymodem myYmdem;
     // QByteArray Path=PathUp.toLatin1();
  //   myYmdem.getFilePath(Path.data());
    //  myYmdem.YmodeInfo();
    //  myYmdem.file_Pack();
    // myYmdem.sendOver();
      //myYmdem.sendOver();
      //  myYmdem->getFilePath();

       //char *pp=Path.data();
     // myYmdem.getFilePath();//qs_data.ascii();

     qDebug()<<"upData Over";
     printf("Hello ");

  }


