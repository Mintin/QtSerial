#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow(); 
 void  WriteSerial(unsigned char *date,int len );
   QSerialPort *serial;
public slots:
    void initPort();
   // char CharToHex(char ch);
    void on_clearButton_clicked();
    void on_sendButton_clicked();
    void on_openButton_clicked();
    void Read_Data();
    void hexShow();
    void openFile();
    void upData();

private:
    Ui::MainWindow *ui;

     QString PathUp;
};

#endif // MAINWINDOW_H
