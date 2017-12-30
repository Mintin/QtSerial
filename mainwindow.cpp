#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initPort();
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
      QByteArray buf;
      buf = serial->readAll();
      if(!buf.isEmpty())
      {
          QString str = ui->textEdit->toPlainText();
          str+=tr(buf);
          ui->textEdit->clear();
          ui->textEdit->append(str);
      }
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
          //关闭设置菜单使能
          ui->PortBox->setEnabled(false);
          ui->BaudBox->setEnabled(false);
          ui->BitNumBox->setEnabled(false);
          ui->ParityBox->setEnabled(false);
          ui->StopBox->setEnabled(false);
          ui->openButton->setText(tr("关闭串口"));
          ui->sendButton->setEnabled(true);
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

            baudList<<"50"<<"75"<<"100"<<"134"<<"150"<<"200"<<"300"
                  <<"600"<<"1200"<<"1800"<<"2400"<<"4800"<<"9600"
                  <<"14400"<<"19200"<<"38400"<<"56000"<<"57600"
                 <<"76800"<<"115200"<<"128000"<<"256000";

              ui->BaudBox->addItems(baudList);
              ui->BaudBox->setCurrentIndex(12);

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

                      //设置按钮可以被按下
                    //  ui->btnOpen->setCheckable(true);











  }


