#include "ymodem.h"

#include <stdio.h>
#include <windows.h>
#include <sys/stat.h>
#include "mainwindow.h"
#include "osapi/Thread.h"
//#define  yprintf(X)     QDebug()<<"X"
//extern MainWindow w;
char upPath[128]={0};

class myThread : public OS_Thread
{
  private:
    // 线程主函数
    virtual int Routine()
    {

         Ymodem myYmdem;
         myYmdem.getFilePath(upPath);
         //myYmdem.YmodeInfo();





     qDebug()<<"myThread:"<<upPath;



        /*
        while(1)
        {
            qDebug()<<"A Thread is Running";
            //printf("A Thread is Running \r\n");
            OS_Thread::Msleep(1500);
        }
*/
        return 0; // 正常退出
    }

public:
    bool m_quitflag;

};


 Ymodem::Ymodem()
{
        remove("SerialLog.txt");
}

 void Ymodem::WriteSerial(uint8_t * Pack,int len)
  {
     FILE* fp;
     // MainWindow serial;
         char buf[10]={0};
          fp=fopen("SerialLog.txt","a");
          if (fp==0) { printf("can't open file Creat file \n");
          }
            for(int k=0;k<len;k++)
            {
                //printf("%02X ",Pack[k]);
                sprintf(buf,"%02X ",Pack[k]);
            //	fwrite(buf,sizeof(unsigned char),1,fp);
                fputs(buf,fp);
            }
            fputs("\r\n>>>>>>>>>>>>\r\n",fp);
         //   printf(">>>>>>>>>>>>>>>>\r\n");

             fclose(fp);
              // serial.WriteSerial(Pack,len);
            // Serial->WriteSerial(Pack,len);
      //       Serial->serial->write((char*)Pack,len);
           //Serial->write((char*)Pack,len);
           // serial.serial->write((char*)Pack,len);

 }



 void WaiteaAsk(void)
  {}

 int getFileSize(char *f)
 {    struct stat st ;
      //int res;
      if(stat( f, &st )==-1)
      { printf("get file size error !\r\n");
      return -1;}
      else {
         // printf(" file size = %d\n", st.st_size);
      return st.st_size;
        }
 }
 unsigned short crc16(const unsigned char *buf, unsigned long count)
 {
         unsigned short crc = 0;
         int i;

         while(count--) {
                 crc = crc ^ *buf++ << 8;

                 for (i=0; i<8; i++) {
                         if (crc & 0x8000) {
                                 crc = crc << 1 ^ 0x1021;
                         } else {
                                 crc = crc << 1;
                         }
                 }
         }
         return crc;
 }
 void Ymodem::getFilePath(char *path)
 {

     this->filePath=path;

 }
 void Ymodem::YmodeInfo(uint8_t *data,int *len)
 { int fileSize=0;
   char	NameStr[20]={0};
   uint32_t CRC=0;
   uint8_t finfo[_RYM_SOH_PKG_SZ] ={0};
   finfo[0]=0x01;
   finfo[1]=0x00;
   finfo[2]=0xFF;
   fileSize=getFileSize(filePath);
   memcpy(finfo+3,filePath,sizeof(filePath));
   //printf("fileName : %d",sizeof(fileName));
     _itoa(fileSize,NameStr,10);
     memcpy(&finfo[3+sizeof(filePath)],NameStr,20);
     CRC= crc16(finfo+3,128);
     finfo[_RYM_SOH_PKG_SZ-1]=(uint8_t)CRC;
     finfo[_RYM_SOH_PKG_SZ-2]=(uint8_t)(CRC>>8);
    //   itoa(getFileSize(fileName),NameStr,10);
     //  printf("NameStr: %s \r\n",NameStr);
    //printf("CRC: %04X \r\n",CRC);
     WriteSerial(finfo,_RYM_SOH_PKG_SZ);
     memcpy(data,finfo,_RYM_SOH_PKG_SZ);
     *len=_RYM_SOH_PKG_SZ;

 }
 void Ymodem::sendOver(uint8_t *data,int *len)
 {
     uint32_t CRC=0;
     uint8_t zero=0;
     uint8_t  buf[_RYM_SOH_PKG_SZ]={0};
     buf[0]=RYM_CODE_SOH;
     buf[1]=0x00;
     buf[2]=0xFF;
     memset(buf+3,zero,128);
     CRC= crc16(buf+3,128);
     buf[_RYM_SOH_PKG_SZ-1]=(uint8_t)CRC;
     buf[_RYM_SOH_PKG_SZ-2]=(uint8_t)(CRC>>8);
     memcpy(data,buf,_RYM_SOH_PKG_SZ);
     *len=_RYM_SOH_PKG_SZ;
      // WriteSerial(buf,_RYM_SOH_PKG_SZ);

  }

 void Ymodem::file_Pack(uint8_t *data,int *len)//(uint8_t *data,int &len);
 {
     FILE * ffile;
     int packHead=2;
     int packIdex=1;
     int fileSize;
     uint8_t repair=0x1A;
     int  integer=0;
     int remainder=0;
     unsigned  char Pack[_RYM_STX_PKG_SZ]={0};
     uint32_t CRC=0;
     fileSize=getFileSize(filePath);
     integer=   fileSize/1024;
     remainder=   fileSize%1024;
     printf("integer:%d  Remainder:%d \r\n",integer,remainder );
     ffile = fopen(filePath, "rb" );
     if(NULL==ffile)  { printf("Can not open file b.bin!\n");}
     else
     {

         for(int i=0;i<integer;i++)
         {   Pack[0]=0x02;
             Pack[1]=packIdex;
             Pack[2]=~packIdex;
             packIdex++;
             fread(Pack+3,sizeof(unsigned char),1024,ffile);
             CRC= crc16(Pack+3,1024);
             Pack[_RYM_STX_PKG_SZ-1]=(uint8_t)CRC;
             Pack[_RYM_STX_PKG_SZ-2]=(uint8_t)(CRC>>8);
             WriteSerial(Pack,_RYM_STX_PKG_SZ);
             memset(Pack,0,_RYM_STX_PKG_SZ);
              WaiteaAsk();
         }
         Pack[0]=0x02;
         Pack[1]=packIdex;
         Pack[2]=~packIdex;
         fread(Pack+3,sizeof(unsigned char),remainder,ffile);
         memset(Pack+remainder+3,repair,1024-remainder);
       CRC= crc16(Pack+3,1024);
       Pack[_RYM_STX_PKG_SZ-1]=(uint8_t)CRC;
        Pack[_RYM_STX_PKG_SZ-2]=(uint8_t)(CRC>>8);
         //WriteSerial(Pack,_RYM_STX_PKG_SZ);
          //data=Pack;
         memcpy(data,Pack,_RYM_STX_PKG_SZ);
         *len=_RYM_STX_PKG_SZ;

        // memset(Pack,0,_RYM_STX_PKG_SZ);
         //  memset(Pack,0,1024);
           WaiteaAsk();
     fclose(ffile);
     }
 }



 void Ymodem::YmodemStart(char *path)
 {
     strcpy(upPath,path);
    // myThread * UpThread=new myThread();
     //strcpy(upPath,path);
    // qDebug()<<"YmodemStart"<<path;
     //memcpy(upPath,path,128);

    // UpThread->Run();

 }
 void Ymodem::YmodemStop(void)
 {

 }






