#ifndef YMODEM_H
#define YMODEM_H

#include "QDebug"
#define    RYM_CODE_NONE  0x00
#define    RYM_CODE_SOH   0x01
#define    RYM_CODE_STX   0x02
#define    RYM_CODE_EOT   0x04
#define    RYM_CODE_ACK   0x06
#define    RYM_CODE_NAK   0x15
#define    RYM_CODE_CAN   0x18
#define    RYM_CODE_C     0x43
#define    uint8_t  unsigned char
#define    uint32_t  unsigned int
#define    _RYM_SOH_PKG_SZ (1+2+128+2)
#define    _RYM_STX_PKG_SZ (1+2+1024+2)
#define infoSize   3+128+2



class Ymodem
{
public:
        Ymodem();
    void YmodeInfo(uint8_t *data,int *len);
    void file_Pack(uint8_t *data,int *len);
    void sendOver(uint8_t *data,int *len);
    void getFilePath(char *path,char *name);
    void WriteSerial(uint8_t * Pack,int len);
    void YmodemStart(char *path);
    void YmodemStop(void);
private:
    char * filePath;
    char  fileName[30];
   //class MainWindow *Serial;

};



#endif // YMODEM_H
