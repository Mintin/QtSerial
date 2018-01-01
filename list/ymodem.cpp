#include <stdio.h>
#include <windows.h>
#include <sys/stat.h>
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
char fileName[]="update.bin";

void WriteSerial(uint8_t * Pack,int len)
 {

		for(int k=0;k<len;k++)
		{   
			printf("%02X ",Pack[k]);
		}
		 printf(">>>>>>>>>>>>>>>>\r\n");

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
void YmodeInfo()
{ int fileSize=0;
  char	NameStr[20]={0};
  uint32_t CRC=0;
  uint8_t finfo[_RYM_SOH_PKG_SZ] ={0};
  finfo[0]=0x01;
  finfo[1]=0x00;
  finfo[2]=0xFF;
  fileSize=getFileSize(fileName);
  memcpy(finfo+3,fileName,sizeof(fileName));
  //printf("fileName : %d",sizeof(fileName));
    itoa(fileSize,NameStr,10);
	memcpy(&finfo[3+sizeof(fileName)],NameStr,20);  
	CRC= crc16(finfo+3,128);
	finfo[_RYM_SOH_PKG_SZ-1]=(uint8_t)CRC;
	finfo[_RYM_SOH_PKG_SZ-2]=(uint8_t)(CRC>>8);
   //   itoa(getFileSize(fileName),NameStr,10);
	//  printf("NameStr: %s \r\n",NameStr);  
   //printf("CRC: %04X \r\n",CRC);
  WriteSerial(finfo,_RYM_SOH_PKG_SZ);
}
void sendOver(void)
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
	  WriteSerial(buf,_RYM_SOH_PKG_SZ);
 }

void file_Pack(void)
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
	fileSize=getFileSize(fileName);
	integer=   fileSize/1024;
	remainder=   fileSize%1024;
	printf("integer:%d  Remainder:%d \r\n",integer,remainder );
    ffile = fopen(fileName, "rb" );
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
	   WriteSerial(Pack,_RYM_STX_PKG_SZ);
		memset(Pack,0,_RYM_STX_PKG_SZ);
		//  memset(Pack,0,1024);
		  WaiteaAsk();
	fclose(ffile);
	}


}


int main(void)
{
YmodeInfo();
file_Pack();
sendOver();
	//YmodeInfo();
getchar();
return 0;}
