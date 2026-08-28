#include"stdint.h"
#include "calendar.h"
#include "zlib.h"
#include "fatfs_flash_spi.h"
#include "ff.h"
#include "malloc.h"
#include "string.h"
#include "hextode.h"
#include "stream1.h"
FIL fsrc;
//												/* Work area (file system object) for logical drives */
//FRESULT res_flash; 
UINT bwk;            					/* File R/W count */

int compressing=0;
unsigned char Combuf[42];
unsigned char *basepos;
unsigned char *curpos;
unsigned char*maxpos;
#define STREAM_BUFSIZE	42	/* initial output buffer size */
		//static int point=18000;
//#define SRAMIN 0
//#define SRAMEX 1
#include<stdio.h>
#include<zlib.h>
//#define ARC_MAGIC       (0.552284749)//?
#include<stdlib.h>
	//	FILE*fp;
		static uint8_t fontl;//??????/F0 /F1 ??
		static uint8_t fontsize;//???
		static uint16_t lastx;
		static uint16_t lasty;
		static uint16_t pages_i;
		static uint16_t page_id;


z_stream z;
 /*****************DATAEEPROM   ?????******************/
void DataEEPROM_Write(uint32_t Address,uint32_t data)
{	
				/* Set the FTDW bit */
//	DIS_INT ;
	FLASH->PEKEYR=PEKEY1;                //unlock  
	FLASH->PEKEYR=PEKEY2;  
	while(FLASH->PECR&FLASH_PECR_PELOCK);  
	FLASH->PECR|=FLASH_PECR_FIX; 
	FLASH->PECR |= (uint32_t)FLASH_PECR_FIX;
	*(__IO uint32_t *)Address = data;
	FLASH->PECR|=FLASH_PECR_PELOCK; 
//	EN_INT;
}
void DataEEPROM_Writeint32(uint32_t Address,int32_t data)
{	
				/* Set the FTDW bit */
//	DIS_INT ;
	FLASH->PEKEYR=PEKEY1;                //unlock  
	FLASH->PEKEYR=PEKEY2;  
	while(FLASH->PECR&FLASH_PECR_PELOCK);  
FLASH->PECR|=FLASH_PECR_FIX; 
	FLASH->PECR |= (uint32_t)FLASH_PECR_FIX;
	*(__IO uint32_t *)Address = data;
	FLASH->PECR|=FLASH_PECR_PELOCK;  
//	EN_INT;
}
	/*****************??????ND?????******************/	
/****************DATAEEPROM ERASE*******************************/
void DataEEPROM_Erase(uint32_t Page_Address)
{
      /* Unlocking the Data memory and FLASH_PECR register access*/
         FLASH->PEKEYR = FLASH_PEKEY1;
         FLASH->PEKEYR = FLASH_PEKEY2;
	  /* Set the ERASE bit */
    FLASH->PECR |= FLASH_PECR_ERASE;

    /* Set PROG bit */
    FLASH->PECR |= FLASH_PECR_PROG;
  
    /* Write 00000000h to the first word of the program page to erase */
	for(uint8_t i;i<30;i++)
	{
    *(__IO uint32_t *)Page_Address = 0x00000000;
		Page_Address +=4;
		
	}
  /* Set the PRGLOCK Bit to lock the program memory access */
  FLASH->PECR |= FLASH_PECR_PRGLOCK;
}
/**********************END*************************/

/*******************??DATAEEPROM ??*************************/
uint32_t DataEEPROM_Read(uint32_t Address)
{	
				uint32_t	Data_temper;
//				DIS_INT ;	
				FLASH->ACR |=(uint32_t)FLASH_ACR_PRE_READ;
				FLASH->ACR |=(uint32_t)FLASH_ACR_PRFTEN;
				Data_temper=*(__IO uint32_t *)Address;
//				EN_INT;
				return  Data_temper;
}
int PDF_init(){
		mem_init(); //????????,SRAMIN==0
	//	mem_init(SRAMEX); //????????,SRAMEX==1	
		fontl=0;//??????/F0 /F1 ??
		fontsize=0;//????
		lastx=0;
		lasty=0;
		pages_i=0;
		page_id=0;
		basepos=Combuf;//(unsigned char *)mymalloc(STREAM_BUFSIZE);
	//memset(basepos,0,1024);
		curpos=basepos;
		maxpos= basepos+STREAM_BUFSIZE;
		return 0;
}



void
pdc_flush_stream()
{	
	UINT bw;
		size_t size;
	uint32_t i=0;
	size = (size_t) (curpos - basepos);
//	for(i=0;i<size;i++)
//		{
//		printf("%c",basepos[i]);
//		}
	f_lseek(&fsrc,fsrc.fsize);
		f_write(&fsrc, basepos, size, &bw);
	//UART1_SendByte(0x08);
		bw=0;
	
	curpos = basepos;
}
static void
pdc_check_stream(size_t len)
{


    if (curpos+len <=maxpos)
	return;
		
    pdc_flush_stream();
    if (curpos + len <=maxpos)
	return;

}


void
pdf_stream(const void *data, size_t size)
{
    int estimate = 0;  
	if (compressing) {
	z.avail_in		= (uInt) size;
	z.next_in		= (Bytef *) data;
	z.avail_out	= 0;
				while(z.avail_in>0)
				{
						if (z.avail_out == 0) {
						/* estimate output buffer size */
						estimate = (int) (z.avail_in/4 + 16);
						pdc_check_stream((size_t) estimate);
						z.next_out = (Bytef *)curpos;
						z.avail_out = (uInt)(maxpos - curpos);	
						}
						deflate(&z,Z_NO_FLUSH);
						curpos = z.next_out;
				}
	}else
	{	
		pdc_check_stream(size);
	memcpy(curpos, data, size);
	curpos+= size;
	}
}

static void
pdc_begin_compress()
{//deflateInit(&z, -1);
  deflateReset(&z);
   compressing =1;
	z.avail_in = 0;

}
static void
pdc_end_compress()
{
    int status;
	if(compressing)
	{
		do {
		pdc_check_stream(128);
		z.next_out = (Bytef *)curpos;
		z.avail_out = (uInt) (maxpos - curpos);
			//printf("%d\n",(uInt) (maxpos - curpos));
		status = deflate(&z, Z_FINISH);
		
		curpos = z.next_out; 
		}while (status != Z_STREAM_END);
}
	compressing =0;
}
	

int pdf_write(char *Inbuf,unsigned int Inlen){
	pdf_stream(Inbuf,Inlen);
		//Outlen+=Inlen;
		//Comlen+=Inlen;
		return 0;
}
void page_ids(){//????
		page_id++;

}

void PDF_begin_document(){
		PDF_init();
	

		deflateInit(&z, -1);
	if(DataEEPROM_Read(0x08080178)==0)
	{
		pdf_write("%PDF-1.4\n",sizeof("%PDF-1.4\n")-1);
	}else
	{
	pdf_write("\r\n",sizeof("\r\n"));
	}
}




int PDF_setparameter_fill(float a,float b,float c){
		char Inbuf[40];int i;
		if(a==0&&b==0&&c==0){
		sprintf(Inbuf,"0 0 0\nrg\n");
		}else{
		sprintf(Inbuf,"%.2f %.2f %.2f\nrg\n", a, b, c);
		}
		for(i=0;Inbuf[i];i++);
		pdf_write(Inbuf,i);
	
		return 0;
}

void  PDF_setparameter_stroke(float a,float b,float c){
		char Inbuf[40];int i;
		sprintf(Inbuf,"%.2f %.2f %.2f\nRG\n", a,b,c);
		for(i=0;Inbuf[i];i++);
		pdf_write(Inbuf,i);


}

void PDF_setcolor(char *s,char *l,float a,float b,float c){
      
		if(strcmp(s,"fill")==0){
			PDF_setparameter_fill( a, b, c);
		}else
		{
			PDF_setparameter_stroke( a, b, c);
		}	
		
}
int PDF_load_font(char *fontname,int a,char *encoding,char*option)
{
		if(strcmp(fontname,"Helvetica")==0)
		{	
		  return 3;
		}
		else if(strcmp(fontname,"Helvetica-Bold")==0)
		{
			  return 4;
		}else
		return 3;
}
void PDF_setfont(int font,int fontsizel){
		char Inbuf[50];int i;
		fontl=font;
		//printf("%d\n",font);
		fontsize=fontsizel;
		sprintf(Inbuf,"/F%d %d Tf\n",fontl,fontsize);
		//strcat(Inbuf,Inbuf1);
		for(i=0;Inbuf[i];i++);
	pdf_stream(Inbuf,i);
	

}
void writeobj(int t){
		char buf[30];int i;
		sprintf(buf,"%d 0 obj\n",t);
		for(i=0;buf[i];i++);
		pdf_write(buf,i);
}
void writeR(int t){
		char buf[30];int i;
		sprintf(buf," %d 0 R\n",t);
		for(i=0;buf[i];i++);
		pdf_write(buf,i);
}
void writeRp(int t){
		char buf[30];int i;
		sprintf(buf," %d 0 R",t);
		for(i=0;buf[i];i++);
		pdf_write(buf,i);
}
void writec(unsigned long t){
		char buf[15];int i;
		sprintf(buf,"%ld",t);
		for(i=0;buf[i];i++);
		pdf_write(buf,i);
}

void contents_id(){
		if(page_id==1){	
			writeobj(3);
		}else{
		writeobj(page_id*4+2);
		}

//		pdf_write("<</Length ",sizeof("<</Length ")-1);
//	
//		writec(100);
		pdf_write("<</Length 100\n/Filter/FlateDecode\n>>\n",sizeof("<</Length 100\n/Filter/FlateDecode\n>>\n")-1);
}
void PDF_begin_page_ext(){
	lastx=0;
		lasty=0;
		page_ids();
		contents_id();
		pdf_write("stream\n",sizeof("stream\n")-1);	
		pdc_begin_compress();

}




void PDF_begin_text()
{
		pdf_write("BT\n",sizeof("BT\n")-1);
}

void PDF_end_text()
{
		pdf_write("ET\n",sizeof("ET\n")-1);
}

void PDF_show_xy(char *content, uint16_t x,uint16_t y){
		 char Inbuf[130];uint8_t i;
		sprintf(Inbuf,"%d %d Td\n(%s)Tj\n",x-lastx,y-lasty,content);
		for(i=0;Inbuf[i];i++);
		pdf_write(Inbuf,i);
		lastx=x;
		lasty=y;

}
void PDF_show_xydata(uint8_t yue,uint8_t ri,uint8_t nian,uint8_t shi,uint8_t fen,uint8_t miao,float tempr,uint16_t x,uint16_t y){
	unsigned int i;char srbuf[41];
	uint8_t D_M_Y_Flag=0;

	D_M_Y_Flag=DataEEPROM_Read(0x08080060);
	if(D_M_Y_Flag==0)
	{
			i=yue;
			yue=ri;
			ri=i;
	}
    sprintf(srbuf,"%d %d Td\n(%02d/%02d/%02d   %02d:%02d:%02d   %0.1f)Tj\n",x-lastx,y-lasty,yue,ri,nian,shi,fen,miao,tempr);
			for(i=0;srbuf[i];i++);	
		pdf_write(srbuf,i);
	
		lastx=x;
		lasty=y;	

}
	
void PDF_show_xydata2(uint8_t yue,uint8_t ri,uint8_t nian,uint16_t x,uint16_t y){

	//char content[12];
	unsigned int i;char srbuf[27];

uint8_t D_M_Y_Flag=0;

	D_M_Y_Flag=DataEEPROM_Read(0x08080060);
	if(D_M_Y_Flag==0)
	{
			i=yue;
			yue=ri;
			ri=i;
	}

    sprintf(srbuf,"%d %d Td\n(%02d/%02d/%02d)Tj\n",x-lastx,y-lasty,yue,ri,nian);
		for(i=0;srbuf[i];i++);
		pdf_write(srbuf,i);
	
		lastx=x;
		lasty=y;

}
void PDF_show_xydata3(uint8_t yue,uint8_t ri,uint8_t nian,uint16_t x,uint16_t y){

	//char content[12];
	unsigned int i;char srbuf[27];

	//sprintf(content,"",yue,ri,nian);	

        sprintf(srbuf,"%d %d Td\n(%02d:%02d:%02d)Tj\n",x-lastx,y-lasty,yue,ri,nian);
		for(i=0;srbuf[i];i++);
	
		pdf_write(srbuf,i);
	
		lastx=x;
		lasty=y;
}
void PDF_show_xydata8(uint16_t yue,uint16_t ri,uint16_t nian,uint16_t x,uint16_t y){

	//char content[12];
unsigned int i;char srbuf[35];

	if(DataEEPROM_Read(0x08080178)==1)
{
	//sprintf(content,"",yue,ri,nian);	
 sprintf(srbuf,"%d %d Td\n(%02d    %02d    %02d)Tj\n",x-lastx,y-lasty,yue,ri,nian);
}else
{					x=371;
        sprintf(srbuf,"%d %d Td\n(%02dd%02dh%02dm)Tj\n",x-lastx,y-lasty,yue,ri,nian);
}
		for(i=0;srbuf[i];i++);
	
		pdf_write(srbuf,i);
	
		lastx=x;
		lasty=y;
}
void PDF_show_xydata4(uint8_t yue,uint8_t ri,uint16_t x,uint16_t y){

unsigned int i;char srbuf[21];

	

        sprintf(srbuf,"%d %d Td\n(%d/%d)Tj\n",x-lastx,y-lasty,yue,ri);
		for(i=0;srbuf[i];i++);//UART1_SendByte(i);
		pdf_write(srbuf,i);
	
		lastx=x;
		lasty=y;

}
void PDF_show_xydata5(float yue,uint16_t x,uint16_t y){

unsigned int i;char srbuf[23];

	

    sprintf(srbuf,"%d %d Td\n(%.1f)Tj\n",x-lastx,y-lasty,yue);
		for(i=0;srbuf[i];i++);
		pdf_write(srbuf,i);
	
		lastx=x;
		lasty=y;

}
void PDF_show_xydataHMone(uint32_t yue,uint16_t x,uint16_t y){

unsigned int i;char srbuf[22];

	
		if(yue==1)
		{
			sprintf(srbuf,"%d %d Td\n(one time)Tj\n",x-lastx,y-lasty);
		}else
		{
			sprintf(srbuf,"%d %d Td\n(%02d:%02d)Tj\n",x-lastx,y-lasty,yue/60,yue%60);
		}
		for(i=0;srbuf[i];i++);
		pdf_write(srbuf,i);
	
		lastx=x;
		lasty=y;

}
void PDF_show_xydataHM(uint32_t yue,uint16_t x,uint16_t y){

unsigned int i;char srbuf[26];


	

		
	
			sprintf(srbuf,"%d %d Td\n(%02d:%02d)Tj\n",x-lastx,y-lasty,yue/60,yue%60);
	
		for(i=0;srbuf[i];i++);
		pdf_write(srbuf,i);
	
		lastx=x;
		lasty=y;

}
void PDF_show_xydata6(uint16_t yue,uint16_t x,uint16_t y){

unsigned int i;char srbuf[26];
    sprintf(srbuf,"%d %d Td\n(%d)Tj\n",x-lastx,y-lasty,yue);
		for(i=0;srbuf[i];i++);
		pdf_write(srbuf,i);
	
		lastx=x;
		lasty=y;

}

void PDF_show_xytou(uint8_t yue,uint8_t ri,uint8_t nian,uint8_t shi,uint8_t fen,uint8_t miao,uint16_t x,uint16_t y){
		char Inbuf[40];uint16_t i;uint8_t D_M_Y_Flag=0;
//		sprintf(content,"%02d/%02d/%02d  %02d:%02d:%02d",yue,ri,nian,shi,fen,miao);
	D_M_Y_Flag=DataEEPROM_Read(0x08080060);
	if(D_M_Y_Flag==0)
	{
			i=yue;
			yue=ri;
			ri=i;
	}
        sprintf(Inbuf,"%d %d Td\n(%02d/%02d/%02d  %02d:%02d:%02d)Tj\n",x-lastx,y-lasty,yue,ri,nian,shi,fen,miao);
		for(i=0;Inbuf[i];i++);
		pdf_write(Inbuf,i);
		lastx=x;
		lasty=y;
}
//??length_id;
void length_stream(int i){
		//pdc_end_compress();
		if(i==1){	
		writeobj(4);
		}else{
		writeobj(i*4+3);//page_id
		}
	pdf_write("<</Length 100\n/Filter/FlateDecode\n>>\nstream\n",sizeof("<</Length 100\n/Filter/FlateDecode\n>>\nstream\n")-1);
		//pdf_write("<</Length 100\n>>\nstream\n",sizeof("<</Length 100\n>>\nstream\n")-1);
	//	pdc_begin_compress();

}
void font_write(int i)
{
	
	writeobj(i*4+4);
			pdf_write("<</ProcSet 0 0 R\n/Font<</F1 5555 0 R\n/F2 5560 0 R\n/F3 5 0 R\n",sizeof("<</ProcSet 0 0 R\n/Font<</F1 5555 0 R\n/F2 5560 0 R\n/F3 5 0 R\n"));
			pdf_write("/F4 6 0 R\n>>\n>>\nendobj\n",sizeof("/F4 6 0 R\n>>\n>>\nendobj\n"));

}
void PDF_end_page_ext(){
		//pdc_flush_stream();
//	
		pdc_end_compress();

		pdf_write("\nendstream\nendobj\n",sizeof("\nendstream\nendobj\n"));
	
		//pdc_flush_stream();
		lastx=0;
		lasty=0;
        //Outlen=0;
}

void PDF_end_document(){
	//page_id

		int t=0,j=0,i;
		(void) deflateEnd(&z);
		for(i=0;i<page_id;i++)
		{
			if(i==0){
			   writeobj(2);
			}else{
				 writeobj(5+i*4);
			}
			pdf_write("<</Type/Page\n/Parent ",sizeof("<</Type/Page\n/Parent ")-1);
			j++;
			if(j>10){
			pages_i++;
			t++;
			j=1;
			}
			if(i<10){
				writeR(1);
			}else{
			writeR(page_id*4+4+pages_i);
			}
			pdf_write("/Contents [ ",sizeof("/Contents [ ")-1);
			if(i==0){
			  writeRp(3);writeRp(11);
				if(DataEEPROM_Read(0x08080178)==1)
				{
					writeRp(5554);
				}
			}else{
				writeRp(i*4+6);
				if(DataEEPROM_Read(0x08080178)==1)
				{
				writeRp(5594);writeRp(4);
				}else
				{
				writeRp(4);
				}
				
			}
			pdf_write("]\n/Resources ",sizeof("]\n/Resources ")-1);
			writeR(8);//writeR(i*4+8);
			pdf_write("/MediaBox[0 0 595 842]\n>>\nendobj\n",sizeof("/MediaBox[0 0 595 842]\n>>\nendobj\n")-1);
		}
//pages

		for(i=0;i<pages_i+1;i++){
			if(i==0){
				writeobj(1);	
			
			}else{
				writeobj(page_id*4+4+i);
			}
			pdf_write("<</Type/Pages\n/Count  ",sizeof("<</Type/Page\n/Count  ")-1);

			if(i==pages_i)
			{
						if(page_id%10!=0)//???????10 20 30 ?
						{ t=page_id%10;
								writec(t);	pdf_write("\n",sizeof("\n")-1);
								
						}else{

							pdf_write("10\n",sizeof("10\n")-1);
							t=10;
						
						}
				
						if(pages_i>0)
						{
							pdf_write("/Parent ",sizeof("/Parent ")-1);
							writeR(page_id*4+4+pages_i+1); 
						}
					  pdf_write("/Kids[",sizeof("/Kids[")-1);
					  if(i==0)
					  {
							for(j=0;j<t;j++)
							{
								if(j==0){
								   writeRp(2);
								}else{
								  writeRp(5+j*4);
								}
							}
					  }else
					  {
							for(j=0;j<t;j++)
							{
							writeRp(5+i*40+j*4);
							}
						
					  }
					pdf_write("]>>\nendobj\n",sizeof("]>>\nendobj\n")-1);
			
			}else
			{
					pdf_write("10\n/Parent ",sizeof("10\n/Parent ")-1);
					writeR(page_id*4+4+pages_i+1);
					pdf_write("/Kids[",sizeof("/Kids[")-1);
					for(j=0;j<10;j++){
					if(i==0&&j==0){
					writeRp(2);		
					}else{
					writeRp(5+j*4+i*40);
					}
					}
					pdf_write("]>>\nendobj\n",sizeof("]>>\nendobj\n")-1);
			
			}
			  

		}
		   if(pages_i>0)
		   {
					//pagesfather
					writeobj(page_id*4+5+pages_i);
					pdf_write("<</Type/Pages\n/Count ",sizeof("<</Type/Pages\n/Count ")-1);
					writec(page_id);
					pdf_write("\n/Kids[",sizeof("\n/Kids[")-1);
						for(j=0;j<pages_i+1;j++)
						{
							if(j==0){
								writeRp(1);
							}else{
								writeRp(page_id*4+4+j);
							}
						}
					pdf_write("]>>\nendobj\n",sizeof("]>>\nendobj\n")-1);
					}

			//font_id
			for(i=0;i<2;i++)
			{
					if(i==0){
						writeobj(5);	pdf_write("<</Type/Font\n/Subtype/",sizeof("<</Type/Font\n/Subtype/")-1);
					   pdf_write("Type1\n/BaseFont/",sizeof("Type1\n/BaseFont/")-1);
						pdf_write("Helvetica\n",sizeof("Helvetica\n")-1);
					}else if(i==1){
						writeobj(6);	pdf_write("<</Type/Font\n/Subtype/",sizeof("<</Type/Font\n/Subtype/")-1);
						pdf_write("Type1\n/BaseFont/",sizeof("Type1\n/BaseFont/")-1);
						pdf_write("Helvetica-Bold\n",sizeof("Helvetica-Bold\n")-1);
					}
				pdf_write("\n>>\nendobj\n",sizeof("\n>>\nendobj\n")-1);

			}
			if(pages_i>0){
				 writeobj(page_id*4+7+pages_i+1); 
			}else{
				 writeobj(page_id*4+pages_i+6+1); 
			}
			pdf_write("<</Type/Catalog\n/Pages ",sizeof("<</Type/Catalog\n/Pages ")-1);
			if(pages_i>0){
			writeR(page_id*4+5+pages_i);
			}else{
			writeR(1);
			}
			pdf_write(">>\nendobj\n",sizeof(">>\nendobj\n")-1);
			//trailer
			pdf_write("trailer\n<</Size ",sizeof("trailer\n<</Size ")-1);
			if(pages_i>0){
			     	writec((page_id*4+10+pages_i+1)); 
			}else{
					writec((page_id*4+9+pages_i+1)); 
			}
			pdf_write("\n/Root ",sizeof("\n/Root ")-1);
			if(pages_i>0){
				 writeR(page_id*4+7+pages_i+1); 
			}else{
				 writeR(page_id*4+6+pages_i+1); 
			}
			pdf_write(">>\nstartxref\n-84\n%%EOF",sizeof(">>\nstartxref\n-84\n%%EOF")-1);
			pdc_flush_stream();
}

void PDF_moveto(uint16_t x,uint16_t y){
	char Inbuf[20];uint8_t i;
	sprintf(Inbuf,"%d %d m\n",x,y);
	for(i=0;Inbuf[i];i++);
	pdf_stream(Inbuf,i);
}
void PDF_lineto(uint16_t x,uint16_t y){
	char Inbuf[20];uint8_t i;
	sprintf(Inbuf,"%d %d l\n",x,y);
	for(i=0;Inbuf[i];i++);
	pdf_stream(Inbuf,i);
}
void PDF_lineto3(uint16_t x,float y){
	char Inbuf[30];uint8_t i;
	sprintf(Inbuf,"%d.12 %.1f73 l\n",x,y);
	for(i=0;Inbuf[i];i++);
	pdf_stream(Inbuf,i);
}
void PDF_moveto2(uint16_t x,float y){
char Inbuf[20];uint8_t i;
sprintf(Inbuf,"%d.0 %.1f0 m\n",x,y);
for(i=0;Inbuf[i];i++);
pdf_stream(Inbuf,i);
}
void PDF_lineto2(float x,float y){
char Inbuf[30];uint8_t i;
//sprintf(Inbuf,"%.1f0 %.1f0 l\n",x,y);
sprintf(Inbuf,"%.1f12 %.2f0 l\n",x,y);
for(i=0;Inbuf[i];i++);
pdf_stream(Inbuf,i);
}
void tch(uint16_t inx,uint16_t iny,uint16_t width,uint16_t height)
{	char Inbuf[80];uint8_t i;
	sprintf(Inbuf,"1 0 0\nRG\n1 0 0\nrg\n%d %d %d %d re\nB\n",inx,iny,width,height);
	for(i=0;Inbuf[i];i++);
	pdf_stream(Inbuf,i);
}
void quxian()
{	char Inbuf[80];int i;
	sprintf(Inbuf,"0.5 0.1 0.2\nRG\n0.7 g\n300 300 m\n300 400 400 400 400 300 c\nb\n");
	for(i=0;Inbuf[i];i++);
	pdf_stream(Inbuf,i);
}
void biaogel(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4 )
{PDF_moveto(x1,y1);PDF_lineto(x2,y2);PDF_lineto(x3,y3);PDF_lineto(x4,y4);PDF_lineto(x1,y1);	pdf_stream("S\n",2);	
}
void biaoge(uint16_t initx,uint16_t inity,uint16_t lw,uint16_t lh,uint16_t line,uint16_t list )
{
			uint16_t i,ttw,tth;
			ttw=(line-1)*lh;  //ttw=totalwidth;
			tth=(list-1)*lw;  //tth=totalheight

			//hang
			for(i=0;i<line;i++){
				PDF_moveto(initx,842-i*lh-inity); 

				PDF_lineto(tth+initx,842-i*lh-inity);

			}

			//lie
			for(i=0;i<list;i++){
				 PDF_moveto(i*lw+initx,842-inity); 

				 PDF_lineto(i*lw+initx,842-ttw-inity);

					  
			}

		
}
void xuxiansz(float x,int y,int a){
		char Inbuf[30];int i;
		sprintf(Inbuf,"%.1f w\n[%d %d] 0 d\n",x,y,a);
		for(i=0;Inbuf[i];i++);
		pdf_stream(Inbuf,i);
}
void xuxianl(uint16_t b,float c,uint16_t d,float f)
{
		char Inbuf[30];int i;
		sprintf(Inbuf,"%d %.1f m\n%d %.1f l\n",b,c,d,f);
		for(i=0;Inbuf[i];i++);
		pdf_stream(Inbuf,i);
	
}


void xuxian(int b,int c,int d,int f)
{
		char Inbuf[30];int i;
		sprintf(Inbuf,"%d %d m\n%d %d l\n",b,c,d,f);
		for(i=0;Inbuf[i];i++);
		pdf_stream(Inbuf,i);
	
}
void CSV_show_data_huiche(uint16_t yue,uint16_t ri,uint16_t nian,uint8_t shi,uint8_t fen,uint8_t miao,float tempr)
	{

	unsigned int i;char srbuf[50];UINT bwl=0;

    sprintf(srbuf,"%02d/%02d/%02d	,%02d:%02d:%02d,%0.1f,\n",yue,ri,nian,shi,fen,miao,tempr);
	
		for(i=0;srbuf[i];i++);
		f_lseek(&fsrc,fsrc.fsize);
				f_write(&fsrc, srbuf, i, &bwl);
	
	

}
void PDF_show_xyeyu_sheshidu(double x,uint16_t y){
		 char Inbuf[139];uint8_t i;uint8_t j;uint8_t t=0;int16_t bu=0;


		sprintf(Inbuf,"1 0 0 1 %.1f %d Tm\n-0.051 Tc[<03A302C1>] TJ\n",x,y);
	
		for(i=0;Inbuf[i];i++);
		pdf_write((unsigned char*)Inbuf,i);

}
void PDF_show_xyeyu(uint16_t x,uint16_t y,uint8_t gl)
{
		 char Inbuf[200];uint8_t i;
	if(gl==0)
	{

		sprintf(Inbuf,"1 0 0 1 %d %d Tm\n[<02BD>-4<0314>10<0326>11<0328>",x,y);//one time  //O
		for(i=0;Inbuf[i];i++);
		pdf_write((unsigned char*)Inbuf,i);
		sprintf(Inbuf,"3<0321>12<032C030C>4<032F>6<0326>11<0328>] TJ\n",x,y);//one time  //O
	}

	if(gl==1)
	{

		sprintf(Inbuf,"1 0 0 1 %d %d Tm\n[<02C3>11<032C0316030F0328>5<0310>-10<030C0003>] TJ\n",x,y);//T
	}
	if(gl==2)
	{

		sprintf(Inbuf,"1 0 0 1 %d %d Tm\n[<02BB>4<0328>3<032C0325>11<030C0003>] TJ\n",x,y);//H
	}
	
	if(gl==3)
	{

		sprintf(Inbuf,"1 0 0 1 %d %d Tm\n[<02BF0328>6<0326>11<031B>-6<03190316>",x,y);//πoHN
				for(i=0;Inbuf[i];i++);
		pdf_write((unsigned char*)Inbuf,i);
		sprintf(Inbuf,"-2<0326>11<031B>-6<0316>] TJ\n",x,y);//πoHN
	}
	if(gl==4)
	{

		sprintf(Inbuf,"1 0 0 1 %d %d Tm\n[<02BF0328>6",x,y);//πoBb
				for(i=0;Inbuf[i];i++);
		pdf_write((unsigned char*)Inbuf,i);
			sprintf(Inbuf,"<030F033C0339>-6<03160326>33<031B>-6<0316>] TJ\n",x,y);//πoBb
	
	}
	if(gl==5)
	{
		sprintf(Inbuf,"1 0 0 1 %d %d Tm\n[<0314>] TJ\n",x,y);//one time  //O
		
	}
		if(gl==6)
	{
		sprintf(Inbuf,"1 0 0 1 %d %d Tm\n[<02AA02AA>] TJ\n",x,y);//one time  //O
		
	}
	if(gl==7)
	{
		sprintf(Inbuf,"1 0 0 1 %d %d Tm\n[<02BA02BA>] TJ\n",x,y);//one time  //O
		
	}
		for(i=0;Inbuf[i];i++);
		pdf_write((unsigned char*)Inbuf,i);

 
	

}
void PDF_show_xyeyul(char *content,double x,uint16_t y){
		 char Inbuf[139];uint8_t i;uint8_t j;uint8_t t=0;int16_t bu=0;
		sprintf(Inbuf,"1 0 0 1 0 0 Tm\n%0.1f %d TD[(%s)]TJ\n",x,y,content);
	
		for(i=0;Inbuf[i];i++);
		pdf_write((unsigned char*)Inbuf,i);
}




		uint16_t days[8],years[8],hours[8],miniter[8],seconds[8];
//	FATFS fst;
	
int16_t Alarm_above[8];uint32_t	Alarm_Accumulative[8];
	uint16_t Alarm_stop_flag[8];
	uint16_t Alarm_above_flag[8];
	uint16_t Alarm_start_stop[8];
	static char Device_ID2[11];
	char logo2[31]="freshliance";
	char Device2[31]="Fresh Tag1";
	char Firmware2[31]="Firmware_V1.0";
	char time_zone2[12]="UTC+0:00";

	char url2[31]="www.freshliance.com";
	
char time_zonew[100]="None";
				uint16_t Alarm_stop_flag_max_n[8];
	uint16_t Alarm_stop_flag_max_L[8];
	uint16_t Alarm_stop_flag_max[8];
	uint16_t Alarm_stop_flag_A[8];
	uint8_t	below_above_flag[8];


	void CSV_Creat()
	{
		
		uint32_t 	Number_Record=0;	

		int16_t Data_max=0,Data_min=0,Average=0;
		float Data_max_li=0,Data_min_li=0,Average_li=0,T_Mkt_li=0,point_x;;	
		uint8_t Set_Interl_time=1;uint32_t First_Y_M_D=0,First_H_M_S=0;
		uint16_t day_flag=0,monthl=2,year=10,hour=17,min=18,sec=16,pgnum=0;	
			uint32_t	dayl=27;
					FRESULT res_flash;uint32_t i=0;
		uint8_t PC_Set_Barcode_Year=0,PC_Set_Barcode_Month=0,PC_Set_Barcode_Num_4=0,PC_Set_Barcode_Num_5=0,PC_Set_Barcode_Num_6=0,PC_Set_Barcode_Num_7=0,PC_Set_Barcode_Life_1=0,PC_Set_Barcode_Life_2=0;
uint32_t	PC_Set_Barcode_YM=0,PC_Set_Barcode_Num=0,PC_Set_Barcode_Life=0;
		uint16_t MAK_page=0;
uint8_t MAK_lie=0;
uint8_t MAK_hang=0;
uint8_t MAK_NUM=0;
uint8_t MAK_point=0;
uint32_t MAK_one_num=0;uint16_t lie=0,h=0,pg=0,lie_num,h_num,j=0,t=0,vnum=0;
uint8_t MAK_read=0;char Life_day='B';uint8_t C_F_FLAG=0;
uint32_t Data_wendu_Adress=0x00100000;uint8_t Wendu_data_buf[2]={0};UINT bwl=0;	
	uint32_t pointl=0;

float wendu=14.9;	char BUFF[22]="0:/17020007.CSV";	
	int16_t  D_wendu_value=0;	
					res_flash = f_mount(&fst,"0:",1); // 挂载文件系统 
					if(res_flash ==FR_NO_FILESYSTEM) // 如果没有文件系统存在 
					{ 
						
									res_flash=f_mkfs("0:",0,0);       //创建一个文件系统 到spi flash   0:FDISK
								
								res_flash = f_mount(&fst,"0:",1);  //为1那么就会强制的进行加载操作
									uint8_t	*Tempe_Data_wendu;	char usb_name2[15]="freshliance";	
											Tempe_Data_wendu=mymalloc(512);	
											for(i=0;i<512;i++)
											{
													Tempe_Data_wendu[i]=0x00;		
												}
											j=0;t=0;
										for(i=0;i<11;i++)
										{
											if(usb_name2[i]=='\0'||t==1)
											{
												Tempe_Data_wendu[i+j]=0x20;t=1;
											}else
											{
												Tempe_Data_wendu[i+j]=usb_name2[i];		
											}
										}
											Tempe_Data_wendu[11+j]=0x08;
										Tempe_Data_wendu[22+j]=0xF2;Tempe_Data_wendu[23+j]=0x78;Tempe_Data_wendu[24+j]=0x9B;Tempe_Data_wendu[25+j]=0x4C;
											TM_FATFS_FLASH_SPI_disk_write(Tempe_Data_wendu, 76, 1);
											f_open(&fsrc, "0:/setting.txt" ,FA_CREATE_NEW|FA_WRITE);
											for(i=0;i<512;i++)
											{
													Tempe_Data_wendu[i]=0xFF;		
											}
										for(i=0;i<8;i++)
										{
											f_write(&fsrc, Tempe_Data_wendu , 512, &bwk);
										}
											f_close(&fsrc);
											f_open(&fsrc, "0:/system" ,FA_CREATE_NEW|FA_WRITE);
											for(i=0;i<3;i++)
											{
												f_write(&fsrc, Tempe_Data_wendu , 512, &bwk);
											}
											f_close(&fsrc);
											myfree(Tempe_Data_wendu);
					}
		if(DataEEPROM_Read(0x0808003C)<2)
		{

				goto Endpdfl;
		}
			if(DataEEPROM_Read(0x08080058)==1)
			{
					goto Endpdfl;
			
			}				
				PC_Set_Barcode_YM=DataEEPROM_Read(0x08080010);
//				PC_Set_Barcode_Year=(uint8_t)(PC_Set_Barcode_YM>>8);
				PC_Set_Barcode_Month=(uint8_t)(PC_Set_Barcode_YM>>0);
				PC_Set_Barcode_Num=DataEEPROM_Read(0x08080014);
				PC_Set_Barcode_Num_4=(uint8_t)(PC_Set_Barcode_Num>>24);
				PC_Set_Barcode_Num_5=(uint8_t)(PC_Set_Barcode_Num>>16);
				PC_Set_Barcode_Num_6=(uint8_t)(PC_Set_Barcode_Num>>8);
				PC_Set_Barcode_Num_7=(uint8_t)(PC_Set_Barcode_Num>>0);
				/********************转换************/
				PC_Set_Barcode_Num_4=PC_Set_Barcode_Num_4-0x30;
				PC_Set_Barcode_Num_5=PC_Set_Barcode_Num_5-0x30;
				PC_Set_Barcode_Num_6=PC_Set_Barcode_Num_6-0x30;
				PC_Set_Barcode_Num_7=PC_Set_Barcode_Num_7-0x30;
				/*************************************/
				
				PC_Set_Barcode_Life=DataEEPROM_Read(0x08080018);
				PC_Set_Barcode_Life_1=(uint8_t)(PC_Set_Barcode_Life>>8);
				PC_Set_Barcode_Life_2=(uint8_t)(PC_Set_Barcode_Life>>0);	
				
				PC_Set_Barcode_Life_1=PC_Set_Barcode_Life_1-0x30;
				


			Life_day=ASCfromHex(PC_Set_Barcode_Life_2);
			sprintf(Device_ID2,"%02d%d%d%d%d%d%c",PC_Set_Barcode_Month,PC_Set_Barcode_Num_4,PC_Set_Barcode_Num_5,PC_Set_Barcode_Num_6,PC_Set_Barcode_Num_7,PC_Set_Barcode_Life_1,Life_day);
			sprintf(BUFF,"0:/%s.CSV",Device_ID2);
			Number_Record=DataEEPROM_Read(0x08080024);   //????????	
//				Set_Interl_time=(uint8_t)(DataEEPROM_Read(0x0808000C)>>8);	
				First_Y_M_D=DataEEPROM_Read(0x0808001C);				//???????  ?????
				year=(uint8_t)(First_Y_M_D>>16);															//????
				monthl=(uint8_t)(First_Y_M_D>>8);
				dayl=(uint8_t)First_Y_M_D;
//				printf("%d\n",dayl);
				First_H_M_S=DataEEPROM_Read(0x08080020);				//???? ?? ???  ?????
				hour=(uint8_t)(First_H_M_S>>16);
				min=(uint8_t)(First_H_M_S>>8);
				sec=(uint8_t)First_H_M_S;
//				MAK_NUM=DataEEPROM_Read(0x0808018C);//MAK_NUM_ADDRESS
				Data_wendu_Adress=0x00100000;
				Set_Interl_time=(uint8_t)DataEEPROM_Read(0x08080068);
				pointl=Number_Record;
				if(pointl>0)
				{
					res_flash =f_open(&fsrc,BUFF,FA_CREATE_NEW| FA_WRITE);
					if(res_flash!=FR_OK)
					{
							f_close(&fsrc);
							goto Endpdfl;
					}	
				}else
				{
							goto Endpdfl;
				}
		f_lseek(&fsrc,fsrc.fsize);
		f_write(&fsrc, "YY/MM/DD,HH/MM/SS,Temperature,\n", sizeof("YY/MM/DD,HH/MM/SS,Temperature,\n")-1, &bwl);
		for(i=0;i<pointl;i++)
		{
					SPI_FLASH_BufferRead(Wendu_data_buf, Data_wendu_Adress, 2);
					Data_wendu_Adress+=2;
					D_wendu_value=((uint16_t)Wendu_data_buf[1] << 8) + Wendu_data_buf[0];
					wendu=(float)D_wendu_value/10;
					CSV_show_data_huiche(year,monthl,dayl,hour,min,sec,wendu);
					min=min+Set_Interl_time;
					if(min>=60)
					{
						hour=hour+min/60;
						min=min%60;
						if(hour>=24)
						{
							dayl++;
							hour=hour%24;
							if(monthl==4||monthl==6||monthl==9||monthl==11)
							{
									if(dayl>=31)
									{
										dayl=1;monthl++;
									}
							}
							else if(monthl==2)
							{
									if((year%4==0&&year%100!=0)||year%400==0)//??
									{
										if(dayl>=30)
										{
											dayl=1;monthl++;
										}
									}
									else
									{
										if(dayl>=29)
										{
											dayl=1;monthl++;
										}
									}
							}
							else
							{
									if(dayl>=32)
									{
										dayl=1;monthl++;
									}
							
							}
							if(monthl>12)
							{
								year++;
								monthl=1;
							}
						
						}
					}
				}
					
		
	
			f_close(&fsrc);		
					Endpdfl:
							{}					
	}



void PDF_Creat()
{

//char 	Star_delay[3]="N/A";
uint8_t font=0,color_flag1=0,color_flag2=0,color_flag3=0;

uint16_t day_flag=0,month=2,day=27,year=10,hour=17,min=18,sec=16,pgnum=0;	
uint32_t pointl=0;
float wendu=14.9;
uint16_t lie=0,h=0,pg=0,lie_num,h_num,j=0,i=0,vnum=0;
uint16_t Stop_time_month=12,Stop_time_day=15,Stop_time_year=15,Stop_time_hour=17,Stop_time_min=18,Stop_time_sec=20;
int32_t ynum=0,yver=0;
uint16_t r_day=0,r_hour=0,r_minter=0;
uint32_t Number_Record_tem=0,Number_Record_S_tem=0;
//int16_t Data_max=0,Data_min=0,Average=0;
//int32_t T_sum=0;
float Data_max_li=0,Data_min_li=0,Average_li=0,T_Mkt_li=0,point_x;;
uint8_t Set_Interl_time=1;
uint32_t Data_wendu_Adress=0x00100000;
uint32_t First_Y_M_D=0,First_H_M_S=0;
//uint32_t First_Y_M_D_ADDRESS 		=		0x0808001C;
//uint32_t First_H_M_S_ADDRESS 		=		0x08080020;
uint8_t Wendu_data_buf[2]={0};
int16_t  D_wendu_value=0;			
uint32_t 	Number_Record=0;	
int16_t Data_max=0,Data_min=0,Average=0;	
int32_t T_sum=0;uint8_t t=0;
uint16_t MAK_page=0;
uint8_t MAK_lie=0;
uint8_t MAK_hang=0;
uint8_t MAK_NUM=0;
uint8_t MAK_point=0;
uint32_t MAK_one_num=0;
uint8_t MAK_read=0;
char BUFF[22]="0:/17020007.pdf";
uint8_t PC_Set_Barcode_Year=0,PC_Set_Barcode_Month=0,PC_Set_Barcode_Num_4=0,PC_Set_Barcode_Num_5=0,PC_Set_Barcode_Num_6=0,PC_Set_Barcode_Num_7=0,PC_Set_Barcode_Life_1=0,PC_Set_Barcode_Life_2=0;
uint32_t	PC_Set_Barcode_YM=0,PC_Set_Barcode_Num=0,PC_Set_Barcode_Life=0;	
char Life_day='B';
uint8_t pdf_language=0;
uint8_t C_F_FLAG=0;
uint8_t D_M_Y_Flag=0;
//uint16_t kaiji_delay=0;
uint16_t	Alarm_Accumulative_num=0;
uint16_t Stop_time_monthl=12,Stop_time_dayl=15,Stop_time_yearl=15,Stop_time_hourl=17,Stop_time_minl=18,Stop_time_secl=20;
		float High_alarm=0;uint16_t High_alarm_Flag=0;
	float Low_alarm=0;uint16_t Low_alarm_Flag=0;
uint8_t USB_Flag=0;uint8_t url_Flag=0;
uint16_t pdf_pointy=0;
				uint32_t monthls[8];
uint8_t point_show=0;
FRESULT res_flash;

		if(DataEEPROM_Read(0x0808003C)<2)
		{

				goto Endpdf;
		}




pdf_language=DataEEPROM_Read(0x08080178);
D_M_Y_Flag=DataEEPROM_Read(0x08080060);
		point_show=DataEEPROM_Read(0x08080160);
C_F_FLAG=DataEEPROM_Read(0x0808017c);
	Set_Interl_time=DataEEPROM_Read(0x08080068);
		


			if(point_show>8)
			{
					point_show=8;
			}
//		for(i=0;i<point_show;i++)
//		{
//				below_above_flag[i]=0;
//				Alarm_stop_flag[i]=0;
//				Alarm_above_flag[i]=0;
//				Alarm_start_stop[i]=0;
//				Alarm_stop_flag_max_n[i]=0;
//				Alarm_stop_flag_max_L[i]=0;
//				Alarm_stop_flag_max[i]=0;
//				Alarm_stop_flag_A[i]=0;
//				below_above_flag[i]=0;
//				monthls[i]=0;
//		}

		
					j=0;
				for(i=0;i<point_show;i++)
					{
					
							j=j+4;
							Alarm_above[i]=DataEEPROM_Read(0x0808006C+j);	
							Alarm_Accumulative[i]=DataEEPROM_Read(0x080800AC+j);
					
					}


					for(i=0;i<point_show;i++)
					{
						below_above_flag[i]=DataEEPROM_Read(0x08080120+i*4);
						
					}	
		 High_alarm=(float)Alarm_above[0]/10;
		 Low_alarm=(float)Alarm_above[1]/10;






				PC_Set_Barcode_YM=DataEEPROM_Read(0x08080010);
				PC_Set_Barcode_Year=(uint8_t)(PC_Set_Barcode_YM>>8);
				PC_Set_Barcode_Month=(uint8_t)(PC_Set_Barcode_YM>>0);
				PC_Set_Barcode_Num=DataEEPROM_Read(0x08080014);
				PC_Set_Barcode_Num_4=(uint8_t)(PC_Set_Barcode_Num>>24);
				PC_Set_Barcode_Num_5=(uint8_t)(PC_Set_Barcode_Num>>16);
				PC_Set_Barcode_Num_6=(uint8_t)(PC_Set_Barcode_Num>>8);
				PC_Set_Barcode_Num_7=(uint8_t)(PC_Set_Barcode_Num>>0);
				/********************转换************/
				PC_Set_Barcode_Num_4=PC_Set_Barcode_Num_4-0x30;
				PC_Set_Barcode_Num_5=PC_Set_Barcode_Num_5-0x30;
				PC_Set_Barcode_Num_6=PC_Set_Barcode_Num_6-0x30;
				PC_Set_Barcode_Num_7=PC_Set_Barcode_Num_7-0x30;
				/*************************************/
				
				PC_Set_Barcode_Life=DataEEPROM_Read(0x08080018);
				PC_Set_Barcode_Life_1=(uint8_t)(PC_Set_Barcode_Life>>8);
				PC_Set_Barcode_Life_2=(uint8_t)(PC_Set_Barcode_Life>>0);	
				
				PC_Set_Barcode_Life_1=PC_Set_Barcode_Life_1-0x30;
				


			Life_day=ASCfromHex(PC_Set_Barcode_Life_2);
			sprintf(Device_ID2,"%02d%02d%d%d%d%d%d%c",PC_Set_Barcode_Year,PC_Set_Barcode_Month,PC_Set_Barcode_Num_4,PC_Set_Barcode_Num_5,PC_Set_Barcode_Num_6,PC_Set_Barcode_Num_7,PC_Set_Barcode_Life_1,Life_day);


							uint8_t	*Tempe_Data_wendu;
							Tempe_Data_wendu=mymalloc(1536);
							char *url;
							char *Device_ID;
							char *Trip_description; 
							char *logo;
							char *Device;
							char *Firmware;
							char *time_zone;
							time_zone=mymalloc(11);
							logo=mymalloc(5);
							Device=mymalloc(7);
							Firmware=mymalloc(9);
							url=mymalloc(4);
							Device_ID=mymalloc(10);
							Trip_description=mymalloc(12);
							time_zone="time zone:(";
							logo="logo:";
							Device="Device:";
							Firmware="Firmware:";
							url="url:";

							Device_ID="Device ID:";
							Trip_description="description:"; 
							TM_FATFS_FLASH_SPI_disk_read(Tempe_Data_wendu, 108,3);
							for(i=0;i<1536;i++)
							{
												if(Tempe_Data_wendu[i]==time_zone[0])
												{
																			for(j=0;j<11;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==time_zone[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==11)
																			{
																					for(j=0;j<12;j++)
																					{
																						if(Tempe_Data_wendu[i+11+j]==0x29&&Tempe_Data_wendu[i+12+j]==0x0D)
																						{
																								break;
																						}else
																						{
																								time_zone2[j]=Tempe_Data_wendu[i+11+j];
																						}
																					}time_zone2[j]='\0';
																			}
												
												}


												if(Tempe_Data_wendu[i]==logo[0])
												{
																			for(j=0;j<5;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==logo[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==5)
																			{
																					for(j=0;j<30;j++)
																					{
																							if(Tempe_Data_wendu[i+5+j]==0x2E)
																						{
																									break;
																						}
																						if(Tempe_Data_wendu[i+5+j]==0x3E&&Tempe_Data_wendu[i+6+j]==0x3E)
																						{
																									break;
																						}
																						logo2[j]=Tempe_Data_wendu[i+5+j];
																					}
																					logo2[j]='\0';
																			}
												
												}
												
												if(Tempe_Data_wendu[i]==Device[0])
												{
																			for(j=0;j<7;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==Device[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==7)
																			{
																					for(j=0;j<30;j++)
																					{
																						if(Tempe_Data_wendu[i+7+j]==0x2E)
																						{
																									break;
																						}
																						if(Tempe_Data_wendu[i+7+j]==0x3E&&Tempe_Data_wendu[i+8+j]==0x3E)
																						{
																									break;
																						}
																						Device2[j]=Tempe_Data_wendu[i+7+j];
																					}
																					Device2[j]='\0';
																			}
												
												}
												
												if(Tempe_Data_wendu[i]==Firmware[0])
												{
																			for(j=0;j<9;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==Firmware[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==9)
																			{
																					for(j=0;j<30;j++)
																					{
																						if(Tempe_Data_wendu[i+9+j]==0x2E)
																						{
																									break;
																						}
																						if(Tempe_Data_wendu[i+9+j]==0x3E&&Tempe_Data_wendu[i+10+j]==0x3E)
																						{
																									break;
																						}
																						Firmware2[j]=Tempe_Data_wendu[i+9+j];
																					}
																					Firmware2[j]='\0';
																			}
												
												}

												if(Tempe_Data_wendu[i]==url[0])
												{
																			for(j=0;j<4;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==url[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==4)
																			{
																					for(j=0;j<30;j++)
																					{
																						if(Tempe_Data_wendu[i+4+j]==0x2E&&Tempe_Data_wendu[i+5+j]==0x0D)
																						{
																									break;
																						}
																						if(Tempe_Data_wendu[i+4+j]==0x3E&&Tempe_Data_wendu[i+5+j]==0x3E)
																						{
																									break;
																						}
																						url2[j]=Tempe_Data_wendu[i+4+j];
																					}
																					url2[j]='\0';url_Flag=1;
																			}
												
												}
												if(Tempe_Data_wendu[i]==Device_ID[0])
												{
																			for(j=0;j<10;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==Device_ID[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==10)
																			{
																					if(j==10)
																					{
																							for(j=0;j<11;j++)
																							{
																								if(Tempe_Data_wendu[i+10+j]==0x2E)
																								{
																											break;
																								}
																								if(Tempe_Data_wendu[i+10+j]==0x3E&&Tempe_Data_wendu[i+11+j]==0x3E)
																								{
																											break;
																								}
																								Device_ID2[j]=Tempe_Data_wendu[i+10+j];
																							}
																							Device_ID2[j]='\0';
																					}
																			}
												
												}
												
												
											if(Tempe_Data_wendu[i]==Trip_description[0])
											{
																		for(j=0;j<12;j++)
																		{
																			if(Tempe_Data_wendu[i+j]==Trip_description[j])
																			{
																			}else
																			{
																				break;
																			}
																		}
																		if(j==12)
																		{
																				for(j=0;j<100;j++)
																				{
																					if(Tempe_Data_wendu[i+12+j]==0x2E)
																					{
																								break;
																					}
																					if(Tempe_Data_wendu[i+12+j]==0x3E&&Tempe_Data_wendu[i+13+j]==0x3E)
																					{
																								break;
																					}
																					time_zonew[j]=Tempe_Data_wendu[i+12+j];
																				}
																				time_zonew[j]='\0';
																		}
											
											}
							}
						 myfree(Tempe_Data_wendu);
						 myfree(time_zone);
							myfree(logo);
							myfree(Device);
							myfree(Firmware);
							myfree(url);
	
							myfree(Device_ID);
							myfree(Trip_description); 					
							sprintf(BUFF,"0:/%s.pdf",Device_ID2+2);		
//		Number_Record=720;
		Number_Record=DataEEPROM_Read(0x08080024);   //????????	
		First_Y_M_D=DataEEPROM_Read(0x0808001C);				//???????  ?????
		year=(uint8_t)(First_Y_M_D>>16);															//????
		month=(uint8_t)(First_Y_M_D>>8);
		day=(uint8_t)First_Y_M_D;
		First_H_M_S=DataEEPROM_Read(0x08080020);				//???? ?? ???  ?????
		hour=(uint8_t)(First_H_M_S>>16);
		min=(uint8_t)(First_H_M_S>>8);
		sec=(uint8_t)First_H_M_S;
		Stop_time_sec=sec;
		MAK_NUM=DataEEPROM_Read(0x0808020C);//MAK_NUM_ADDRESS
		Data_wendu_Adress=0x00100000;
			pointl=Number_Record;
			Data_max_li=(((float)((int16_t)DataEEPROM_Read(0x08080028)))/10);
			Data_min_li=(((float)((int16_t)DataEEPROM_Read(0x0808002C)))/10);	
			Average_li=(((float)((int16_t)(DataEEPROM_Read(0x08080030)/pointl)))/10);
			T_Mkt_li=Average_li+0.3;
		if(DataEEPROM_Read(0x0808019C)==1)
		{
			
					goto Endpdf;
		}
		if(pointl>0)
		{
					res_flash = f_mount(&fst,"0:",1); // 挂载文件系统 

			
							if(pdf_language==1)
							{
												res_flash=f_open(&fsrc,BUFF,FA_OPEN_EXISTING|FA_WRITE);
													if(res_flash!=FR_OK)
													{
															f_rename("0:/system",BUFF);
															res_flash=f_open(&fsrc,BUFF,FA_OPEN_EXISTING|FA_WRITE);
															DataEEPROM_Write(0x0808019C,1);
													}
								}else
								{
										
														res_flash =f_open(&fsrc,BUFF,FA_CREATE_NEW| FA_WRITE);
														DataEEPROM_Write(0x0808019C,1);
											
										
									}
								
						
			
		}else
		{
				goto Endpdf;
		}

		for(j=0;j<pointl;j++)
			{
											SPI_FLASH_BufferRead(Wendu_data_buf, Data_wendu_Adress, 2);
											Data_wendu_Adress+=2;
											D_wendu_value = ((uint16_t)Wendu_data_buf[1] << 8) + Wendu_data_buf[0];

										///////////////////////////////////////////////////////////////////////////////////////
											for(i=0;i<point_show;i++)
											{
																		if(Alarm_Accumulative[i]%Set_Interl_time!=0&&Alarm_Accumulative[i]>Set_Interl_time)
																		{
																			Alarm_Accumulative_num=Alarm_Accumulative[i]/Set_Interl_time+1;
																		}else
																		{
																			Alarm_Accumulative_num=Alarm_Accumulative[i]/Set_Interl_time;
																		}
						
																		if(below_above_flag[i]==0&&D_wendu_value<Alarm_above[i])
																		{
																						if(Alarm_above_flag[i]==0)
																						{
																							Alarm_stop_flag_max_L[i]++;
																						}
																						Alarm_stop_flag_A[i]++;
																							if(Alarm_stop_flag_max[i]<=(Alarm_Accumulative_num))
																								{
																												Alarm_stop_flag_max[i]++;
																											if(Alarm_stop_flag_max[i]>=Alarm_Accumulative_num)
																											{
																																if(Alarm_stop_flag[i]==0)
																																{
																																	Alarm_stop_flag[i]=1;
																																}
																											}
																								}else
																								{
																														if(Alarm_stop_flag[i]==0)
																														{
																															Alarm_stop_flag[i]=1;
																														}
																								}									
																		
																						Alarm_above_flag[i]++;
																				
																			if(Alarm_stop_flag_max_n[i]<Alarm_above_flag[i])
																			{
																					Alarm_stop_flag_max_n[i]=Alarm_above_flag[i];
																			}
								
																		}else if(below_above_flag[i]==1&&D_wendu_value>Alarm_above[i])
																		{
																						if(Alarm_above_flag[i]==0)
																						{
																							Alarm_stop_flag_max_L[i]++;
																						}
																				Alarm_stop_flag_A[i]++;
																								if(Alarm_stop_flag_max[i]<=(Alarm_Accumulative_num))
																								{
																										
																									
																												Alarm_stop_flag_max[i]++;
																									if(Alarm_stop_flag_max[i]>=Alarm_Accumulative_num)
																											{
																																if(Alarm_stop_flag[i]==0)
																																{
																																	Alarm_stop_flag[i]=1;
																																}
																											}
																									
																								}else
																								{
																														if(Alarm_stop_flag[i]==0)
																														{
																															Alarm_stop_flag[i]=1;
																														}
																								}

																					Alarm_above_flag[i]++;
																		
																			if(Alarm_stop_flag_max_n[i]<Alarm_above_flag[i])
																			{
																					Alarm_stop_flag_max_n[i]=Alarm_above_flag[i];
																			}			
																		
																		}
																		else
																		{
																			
																			Alarm_above_flag[i]=0;
																			
																				
																		}
																		if(Alarm_stop_flag[i]==1)
																		{
														
																							day_flag=0;Stop_time_sec=sec;
																							Alarm_start_stop[i]=j+1;
																						Number_Record_S_tem=j*Set_Interl_time;

																						r_day= Number_Record_S_tem/1440;      //24*60=1440???
																						r_hour=(Number_Record_S_tem%1440)/60;
																						r_minter=(Number_Record_S_tem%1440)%60;

																						Stop_time_hourl=r_hour+hour;

																						Stop_time_minl=r_minter+min;

																								if(Stop_time_secl>=60)
																								{	
																									Stop_time_minl+=(Stop_time_secl/60);
																									Stop_time_secl=(Stop_time_secl%60);

																								}

																								if(Stop_time_minl>=60)
																									{	
																					
																										Stop_time_hourl+=(Stop_time_minl/60);
																										Stop_time_minl=(Stop_time_minl%60);

																									}
																									
																									
																								if(Stop_time_hourl>=24)
																								{		
																						
																									day_flag+=(Stop_time_hourl/24);
																									Stop_time_hourl=(Stop_time_hourl%24);
																									
																							
																								}

																								
																								Day_Canclude(year,month,day,r_day+day_flag);
																								years[i]=temper_year;
																								monthls[i]=temper_month;
																								days[i]=temper_day;
																								hours[i]=Stop_time_hourl;
																								miniter[i]=Stop_time_minl;
																								seconds[i]=sec;
																						
																						
																									Alarm_stop_flag[i]=2;
																									
																							
																			}
																								
												}
	
										
												
												
									

		
}
	Data_wendu_Adress=0x00100000;			
		First_Y_M_D=DataEEPROM_Read(0x0808001C);				//???????  ?????
		year=(uint8_t)(First_Y_M_D>>16);															//????
		month=(uint8_t)(First_Y_M_D>>8);
		day=(uint8_t)First_Y_M_D;
		First_H_M_S=DataEEPROM_Read(0x08080020);				//???? ?? ???  ?????
		hour=(uint8_t)(First_H_M_S>>16);
		min=(uint8_t)(First_H_M_S>>8);
		sec=(uint8_t)First_H_M_S;			
	if(pointl%600==0)
		{
			pgnum=pointl/600;
		}else
		{
			pgnum=pointl/600+1;
		}
		PDF_begin_document();
		//??? 2 0 obj stream 3 0 obj
		PDF_begin_page_ext();
		if(pdf_language==1)
		{
				PDF_begin_text();
				PDF_setcolor("fill", "rgb", 0, 0, 0);
				pdf_stream("/F3 4.78 Tf\n",12);
				PDF_show_xyeyul("X",195.7,549);
				PDF_show_xyeyul("X",400.4,549);
				PDF_show_xyeyul("X",474.7,552);	
				PDF_end_text();
		}
		font = PDF_load_font( "Helvetica",0, "host", "");
		PDF_setcolor("fill", "rgb", 0, 0, 0); 
		PDF_begin_text();
		PDF_setfont(font,7);//TIMe 10??
		PDF_show_xydata2(month,day,year,(uint16_t)(63),(uint16_t)74);
		PDF_show_xydata3(hour,min,sec,(uint16_t)(63),(uint16_t)63);
		pointl--;
		for(j=1;j<11;j++)
		{       if(pointl>=10)
							{
								Number_Record_tem+=(pointl/10);
								Number_Record_S_tem=Number_Record_tem*Set_Interl_time;
								r_day= Number_Record_S_tem/1440;      //24*60=1440???
								r_hour=(Number_Record_S_tem%1440)/60;
								r_minter=(Number_Record_S_tem%1440)%60;
								Stop_time_hour=r_hour+hour;
								Stop_time_min=r_minter+min;
							}
							else
							{
								Stop_time_hour=r_hour+hour;
								Stop_time_min=r_minter+min;
							}						
					Stop_time_sec=(pointl%10)*Set_Interl_time*6*j+sec;
				if(Stop_time_sec>=60)
				{	
					Stop_time_min+=(Stop_time_sec/60);
					Stop_time_sec=(Stop_time_sec%60);
				}

				if(Stop_time_min>=60)
					{	
						Stop_time_hour+=(Stop_time_min/60);
						Stop_time_min=(Stop_time_min%60);
					}
				if(Stop_time_hour>=24)
				{		
					day_flag+=(Stop_time_hour/24);
					Stop_time_hour=(Stop_time_hour%24);
				}

				
				Day_Canclude(year,month,day,r_day+day_flag);
				Stop_time_year=temper_year;
				Stop_time_month=temper_month;
				Stop_time_day=temper_day;
				day_flag=0;

					PDF_show_xydata2(Stop_time_month,Stop_time_day,Stop_time_year,(uint16_t)(63+j*47),(uint16_t)74);
					PDF_show_xydata3(Stop_time_hour,Stop_time_min,Stop_time_sec,(uint16_t)(63+j*47),(uint16_t)63);
			
		}

		r_minter=r_minter+(pointl%10)*Set_Interl_time+Set_Interl_time;
				if(r_minter>=60)
				{
					r_hour+=r_minter/60;
					r_minter=r_minter%60;
				}
				if(r_hour>=24)
				{	
					r_day+=r_hour/24;
					r_hour=r_hour%24;
				}
		
	pointl++;
			char C_F[2];
				PDF_setfont(font,5);
				PDF_show_xy("o",184,646); 
				PDF_show_xy("o",181,632); 
				PDF_show_xy("o",185,618); 
				PDF_show_xy("o",201,604);
			if(pdf_language==1)
			{	
				PDF_show_xy("o",187,421);	
			}
			if(C_F_FLAG==1)
			{
				C_F[0]=0x46;

			}	else
			{
			
				C_F[0]=0x43;
			}
			C_F[1]='\0';
			PDF_setfont(font,8);
			if(pdf_language==1)
			{	
				PDF_show_xy("]",198,418);	
			}
				PDF_setfont(font,9);	
				PDF_show_xy(C_F,187,642); 
				PDF_show_xy(C_F,184,628);
				PDF_show_xy(C_F,188,614); 
				PDF_show_xy(C_F,204,600);
			if(pdf_language==1)
			{	
					PDF_show_xy(C_F,190,417);
			}
		
				if(C_F_FLAG==0)
				{
							High_alarm_Flag=(((float)High_alarm)*4+201);
							Low_alarm_Flag=(((float)Low_alarm)*4+201);
				}else
				{
							High_alarm_Flag=(((float)(High_alarm-32)*10/18)*4+201);
							Low_alarm_Flag=(((float)(Low_alarm-32)*10/18)*4+201);
				}
		if(point_show>0)
					{
									PDF_setfont(font,7);
									PDF_show_xydata5(High_alarm,552,High_alarm_Flag);
								
					}
					if(point_show>1)
					{
									PDF_show_xydata5(Low_alarm,552,Low_alarm_Flag);
					}
					if(point_show>0)
						{
						
								PDF_show_xy(C_F,571,High_alarm_Flag);	
						}
						if(point_show>1)
						{
								PDF_show_xy(C_F,571,Low_alarm_Flag);
						}
						if(point_show>0)
						{
										PDF_setfont(font,5);

										PDF_show_xy("o",568,High_alarm_Flag+3);	
						}
						if(point_show>1)
						{
										PDF_show_xy("o",568,Low_alarm_Flag+3);	
						}	
						pdf_pointy=524;	
							if(pdf_language==1)
							{		
								pdf_pointy=521;	
							}								
    PDF_setfont(font,5);
		for(i=0;i<point_show;i++)
		{
	

						PDF_show_xydata5((float)(Alarm_above[i])/10,50,pdf_pointy-12*i);
		
		
		}
		for(i=0;i<point_show;i++)
		{

						if(Alarm_Accumulative[i]!=1)
						{
							PDF_show_xydataHMone(Alarm_Accumulative[i],176,pdf_pointy-12*i);
						}
							
			
		}
							if(pdf_language==0)
							{
									for(i=0;i<point_show;i++)
									{
												if(below_above_flag[i]==0)
											{
														PDF_show_xy("<",131,pdf_pointy-12*i);
											}else
											{
														PDF_show_xy(">",131,pdf_pointy-12*i);
											
											}
									}
	
						}
			for(i=0;i<point_show;i++)
			{


//										printf("%d\n",);
									if(Alarm_stop_flag[i]==2&&Alarm_stop_flag_max_n[i]>0)
									{
									
														if(pdf_language==0)
														{
																PDF_show_xy("ALARM",235,pdf_pointy-12*i);
														}
													PDF_show_xytou(monthls[i],days[i],years[i],hours[i],miniter[i],seconds[i],281,pdf_pointy-12*i);
													PDF_show_xydataHM(Alarm_start_stop[i]*Set_Interl_time,348,pdf_pointy-12*i);
													PDF_show_xydata6(Alarm_stop_flag_max_L[i],409,pdf_pointy-12*i);PDF_show_xydataHM(Alarm_stop_flag_max_n[i]*Set_Interl_time,461,pdf_pointy-12*i);PDF_show_xydataHM(Alarm_stop_flag_A[i]*Set_Interl_time,517,pdf_pointy-12*i);	

									}else
									{
															if(pdf_language==0)
														{
																PDF_show_xy("OK",239,524-12*i);		
														}
														PDF_show_xy("-",301,pdf_pointy-12*i);


									}
							
			
			
			}	
			

      
						
			
	
				if(C_F_FLAG==1)
				{
						Data_max_li=32+Data_max_li*18/10;Data_min_li=32+Data_min_li*18/10;Average_li=32+Average_li*18/10;T_Mkt_li=32+T_Mkt_li*18/10;			
				}


		if(pdf_language==0)
		{	
					PDF_setfont(font,10);
					PDF_show_xydata6(DataEEPROM_Read(0x08080064),278,739);
					PDF_show_xytou(Stop_time_month,Stop_time_day,Stop_time_year,Stop_time_hour,Stop_time_min,sec,135,739);
					PDF_show_xy(Device_ID2,100,694);
					PDF_show_xy(time_zone2,207,750);
					PDF_show_xydata5(Data_max_li,149,642);			
					PDF_show_xydata5(Data_min_li,149,628);	PDF_show_xydata5(Average_li,152,614);	PDF_show_xydata5(T_Mkt_li,173,600);
					PDF_show_xytou(month,day,year,hour,min,sec,420,642);
					PDF_show_xytou(Stop_time_month,Stop_time_day,Stop_time_year,Stop_time_hour,Stop_time_min,sec,371,628);
					PDF_show_xydata6(pointl,371,614);
					PDF_show_xydata8(r_day,r_hour,r_minter,371,600);
				
		}else
		{		
							PDF_setfont(font,7);

							PDF_show_xy(Device_ID2,208,694);
							PDF_show_xy(time_zone2,258,750);
							PDF_show_xydata5(Data_max_li,164,642);
							PDF_show_xydata5(Data_min_li,161,628);
							PDF_show_xydata5(Average_li,139,614);
							PDF_show_xydata5(T_Mkt_li,181,600);
							PDF_show_xytou(day,month,year,hour,min,sec,424,642);
							PDF_show_xydata6(Set_Interl_time,383,708);
							PDF_show_xydata6(DataEEPROM_Read(0x0808006c),389,694);
							PDF_show_xydata6(DataEEPROM_Read(0x08080064),278,739);
							PDF_show_xytou(Stop_time_month,Stop_time_day,Stop_time_year,Stop_time_hour,Stop_time_min,sec,175,739);
							PDF_show_xytou(Stop_time_day,Stop_time_month,Stop_time_year,Stop_time_hour,Stop_time_min,sec,408,628);
							PDF_show_xydata6(pointl,405,614);
							if(r_day>99)
							{
								PDF_show_xydata8(r_day,r_hour,r_minter,443,600);
							}else
							{
								PDF_show_xydata8(r_day,r_hour,r_minter,447,600);
							}

		}		
		PDF_setfont(font,10);
		if(!(url_Flag==0&&pdf_language==1))
		{
			PDF_show_xy(url2,57,47);	
		}
		PDF_show_xydata4(1,pgnum+1,503,47);	
		PDF_setfont(font,7);//TIMe 10??
				if(C_F_FLAG==0)
				{
						for(i=0;i<21;i++)
						{
							if(i>18)
							{
								j=-2;
							
							}else
							{
								j=0;
							}
							PDF_show_xydata5(50-i*4,58+j,403-i*16);
						}
							
				}else
				{
						for(i=0;i<21;i++)
						{
							if(i<4)
							{
								j=-2;
							}else if(i>15&&i<18)
							{
								j=2;
							}else if(i>18)
							{
								j=-2;
							
							}else
							{
								j=0;
							}
							PDF_show_xydata5((50-i*4)*18/10+32,58+j,403-i*16);
						}
				}

			if(pdf_language==0)
			{
						PDF_setfont(font,26);//TIMe 10??
						PDF_show_xy("Data Report",50,772);
						PDF_setcolor("fill", "rgb", 1, 0, 0); 
						PDF_show_xy(logo2,415,772);
						PDF_setcolor("fill", "rgb", 0, 0, 0);
						PDF_setfont(font,10);
						PDF_show_xy("Work days:          days",225,739);
						PDF_show_xy("Highest Temperature:",50,642);	
						PDF_show_xy("Lowest Temperature:",50,628);	
						PDF_show_xy("Average Temperature:",50,614);
						PDF_show_xy("Mean Kinetic Temperature:",50,600);
						PDF_show_xy("Start Time/First Point:",312,642); 
						PDF_show_xy("Stop Time:",312,628);
						PDF_show_xy("Data Point:",312,614);
						PDF_show_xy("Trip Length:",312,600);
						PDF_show_xy("Note:All times shown are based on",50,750);
						if(D_M_Y_Flag==0)
						{
								PDF_show_xy("and 24-Hour clock[DD/MM/YY HH:MM:SS]",260,750);	
						}else
						{
								PDF_show_xy("and 24-Hour clock[MM/DD/YY HH:MM:SS]",260,750);	
						}
						PDF_show_xy("File Created Time:",50,739);
						PDF_show_xy("Device:",50,708);
						PDF_show_xy("Device ID:",50,694);
						PDF_show_xy("Log Interval:",161,694);
						PDF_show_xy("Firmware:",163,708);
						PDF_show_xy("Trip description:",312,708);	
						PDF_show_xy("Start Mode: Key_start      Start Delay:",50,680);		
							PDF_show_xydata6(Set_Interl_time,223,694);
						PDF_show_xydata6(DataEEPROM_Read(0x0808006c),219,680);
						PDF_show_xy(Device2,87,708);
						PDF_show_xy(Firmware2,210,708);
						for(i=0;time_zonew[i]!='\0';i++);
						if(i>29)
						{
									if(i>69)
								{
									PDF_show_xy(time_zonew+69,312,680);
									time_zonew[70]='\0';
								}
								PDF_show_xy(time_zonew+29,312,694);
								time_zonew[30]='\0';
						}
						PDF_show_xy(time_zonew,386,708);
						PDF_setfont(font,11);//TIMe 10??
						PDF_show_xy("Device Configuration",50,722);		
						PDF_show_xy("Logging Summary",50,658);
						if(C_F_FLAG==0)
						{
								PDF_show_xy("Temperature[ C]",50,417);		

						}else
						{
								PDF_show_xy("Temperature[ F]",50,417);		
						}
						
						PDF_setfont(font,7);
						PDF_show_xy("o",116,422);
						PDF_setfont(font,9);
						PDF_show_xy("Alarm settings",101,571);PDF_show_xy("Statistics and measurement results",314,571);
						PDF_setfont(font,5);
						PDF_show_xy("Threshold",60,548);
						PDF_show_xy("Below/Over",116,548);
						PDF_show_xy("Allow Time",172,548);
						PDF_show_xy("Status",234,548);
						PDF_show_xy("First triggered",284,548);
						PDF_show_xy("Time from",344,550);PDF_show_xy("start to violation",340,545);
						PDF_show_xy("Number of violation",392,548);
						PDF_show_xy("Maximum",456,550);PDF_show_xy("time of violation",452,545);
						PDF_show_xy("Total time",510,550);
						PDF_show_xy("of violation",510,545);
						for(i=0;i<point_show;i++)
						{
											if(Alarm_Accumulative[i]==1)
											{
													//one time
													PDF_show_xy("one time",172,524-12*i);
											}
						}
					
											
			}else
			{	
					PDF_end_text();	
					PDF_begin_text();
					pdf_write("/F1 7.35 Tf\n",sizeof("/F1 7.35 Tf\n"));
					for(i=0;i<point_show;i++)
					{
									if(Alarm_Accumulative[i]==1)
									{
											//one time
										PDF_show_xyeyu(172,521-12*i,0);//Однократно
									}
						}
					for(i=0;i<point_show;i++)
					{
							if(below_above_flag[i]==0)
							{
										//Decrease
									PDF_show_xyeyu(115,521-12*i,3);//Понижение
							}else
							{
									//Increase
								PDF_show_xyeyu(115,521-12*i,4);//Повышение
							}
					}
					for(i=0;i<point_show;i++)
					{
											if(Alarm_stop_flag[i]==2&&Alarm_stop_flag_max_n[i]>0)
											{
													//Alarm
													PDF_show_xyeyu(233,521-12*i,1);//Тревога
											}else
											{
													//Norm
												PDF_show_xyeyu(233,521-12*i,2);//Норма
											}
					}
					pdf_write("/F1 9.3 Tf\n",sizeof("/F1 9.7 Tf\n"));
					PDF_show_xyeyu(290,739,5);
					PC_Set_Barcode_Year=7;PC_Set_Barcode_Month=6;
					if(D_M_Y_Flag==0)
					{
						PC_Set_Barcode_Year=6;PC_Set_Barcode_Month=7;
					}
					PDF_show_xyeyu(382-PC_Set_Barcode_Year,750,PC_Set_Barcode_Year);PDF_show_xyeyu(393,750,PC_Set_Barcode_Month);
			}
		PDF_end_text();
		PDF_end_page_ext();
		font_write(1);
		// 4 0 obj
		length_stream(1);
		pdc_begin_compress();
		PDF_setcolor("stroke", "rgb", 0, 0, 0);
		biaoge(54,61,79,12,2,7);
		biaoge(54,73,79,704,2,7);
		PDF_moveto(53,56);
		PDF_lineto(540,56);
		pdf_write("S\n",2);
		font = PDF_load_font( "Helvetica",0, "host", "");
		PDF_setcolor("fill", "rgb", 0, 0, 0); 
					PDF_begin_text();
					PDF_setfont(font,5);//TIMe 10??
					for(i=0;i<6;i++)
					{

						PDF_show_xy("o",119+79*i,776);
					}	
					PDF_setfont(font,9);//TIMe 10??
					for(i=0;i<6;i++)
					{
			
						if(C_F_FLAG==0)
						{
									PDF_show_xy("C",122+79*i,771);
						}else
						{
									PDF_show_xy("F",122+79*i,771);
						}
					}	
					
		if(pdf_language==0)
		{	
			PC_Set_Barcode_Year=141;
				PDF_show_xy("File Created Time:",56,786);
				for(i=0;i<6;i++)
				{
					if(C_F_FLAG==0)
					{
								PDF_show_xy("Date   Time",59+79*i,771);
					}else
					{
								PDF_show_xy("Date   Time",59+79*i,771);
					}

				}
		}
		if(pdf_language==1)
		{	
			PC_Set_Barcode_Year=175;PDF_setfont(font,7);
		}
		PDF_show_xytou(Stop_time_month,Stop_time_day,Stop_time_year,Stop_time_hour,Stop_time_min,sec,PC_Set_Barcode_Year,786);
		if(!(url_Flag==0&&pdf_language==1))
		{
				font = PDF_load_font( "Helvetica-Bold",0, "host", "");
				PDF_setfont(font,10);//TIMe 10??
				PDF_show_xy(url2,57,47);
		}
		PDF_end_text();
		pdc_end_compress();
		pdf_write("\nendstream\nendobj\n",sizeof("\nendstream\nendobj\n"));
/////////////////
////////////////////////////
		length_stream(2);
		pdc_begin_compress();
		PDF_setcolor("stroke", "rgb", 0, 0, 0);
		biaoge(48,124,251,45,2,2);
		biaoge(310,124,240,45,2,2);
		biaoge(48,190,502,57,2,2);
		biaoge(48,262,168,12,2,2);
		biaoge(48,262,504,12,2,2);
		PC_Set_Barcode_Year=36;
		if(pdf_language==1)
		{	
			PC_Set_Barcode_Year=39;
		}
			biaoge(48,274,56,PC_Set_Barcode_Year,2,10);
		biaoge(48,274+PC_Set_Barcode_Year,56,12,(1+point_show),10);
		biaoge(76,435,474,323,2,2);
		PDF_moveto(53,56);
		PDF_lineto(550,56);
		pdf_stream("S\n",2);
		PDF_setcolor("stroke", "rgb", 0, 0, 1);
		Data_wendu_Adress=0x00100000;		
		SPI_FLASH_BufferRead(Wendu_data_buf, Data_wendu_Adress, 2);
		Data_wendu_Adress+=2;
		D_wendu_value = ((uint16_t)Wendu_data_buf[1] << 8) + Wendu_data_buf[0];
		PDF_moveto2(76,(((float)D_wendu_value/10)*4+203));	
			if(pointl<474)
			{
					if(pointl==1)
				{
						PDF_lineto3(550,(((float)D_wendu_value/10)*4+203));
				}else
				{
								for(j=1;j<pointl;j++)
								{
										SPI_FLASH_BufferRead(Wendu_data_buf, Data_wendu_Adress, 2);
										Data_wendu_Adress+=2;
										D_wendu_value = ((uint16_t)Wendu_data_buf[1] << 8) + Wendu_data_buf[0];
										if(pointl==2)
										{
											PDF_lineto3(550,(((float)D_wendu_value/10)*4+203));
										}
										else
										{
											PDF_lineto3(76+(j*474)/(pointl-1),(((float)D_wendu_value/10)*4+203));
										}
					
								}
					}
			}else if(pointl<4740)
			{				pdf_stream("0.1 w\n",6);
								Data_wendu_Adress=0x00100000;
							point_x=((float)474)/(pointl-1);
								for(j=0;j<=pointl;j++)
							{
												SPI_FLASH_BufferRead(Wendu_data_buf, Data_wendu_Adress, 2);
												Data_wendu_Adress+=2;
												D_wendu_value = ((uint16_t)Wendu_data_buf[1] << 8) + Wendu_data_buf[0];
												PDF_lineto2(76+point_x*j,(((float)D_wendu_value/10)*4+203));
							}
			}else
			{
				Data_wendu_Adress=0x00100000;
								vnum=pointl/474;
								for(j=0;j<474;j++)
							{
											for(i=0;i<vnum;i++)
											{
												SPI_FLASH_BufferRead(Wendu_data_buf, Data_wendu_Adress, 2);
												Data_wendu_Adress+=2;
												D_wendu_value = ((uint16_t)Wendu_data_buf[1] << 8) + Wendu_data_buf[0];
												ynum+=D_wendu_value;

											}
											yver=ynum/vnum;
												PDF_lineto3(76+j,((((float)yver)/10)*4+203));
											
											ynum=0;
							}
			}
			pdf_write("S\n\n",3);
			if(point_show>0)
			{
					PDF_setcolor("stroke", "rgb", 1, 0, 0);
					xuxiansz(1,3,3);
						
							 xuxianl(76,High_alarm_Flag+2,550,High_alarm_Flag+2);	
			}
			if(point_show>1)
			{
					xuxianl(76,Low_alarm_Flag+2,550,Low_alarm_Flag+2);	
			}
			if(point_show>0)
			{
						pdf_stream("S\n",2);
			}
        PDF_setcolor("stroke", "rgb", 0, 0, 0);
				xuxiansz(0.1,1,3);
		for(i=0;i<19;i++)
		{
			xuxian(76,387-i*16,550,387-i*16);
		
		}
		for(i=0;i<9;i++)
		{
			xuxian(123+i*47,405,126+i*47,85);
	
		}
			pdf_stream("S\n\n",4);     //2016 10 17  ??
	

		
		pdc_end_compress();
		pdf_write("\nendstream\nendobj\n",sizeof("\nendstream\nendobj\n")-1);

		Data_wendu_Adress=0x00100000;
		for(pg=0;pg<pgnum;pg++)
		{
		//??? stream 10 0 obj
		PDF_begin_page_ext();
		font = PDF_load_font( "Helvetica-Bold",0, "host", "");
		PDF_setcolor("fill", "rgb", 0, 0, 0); 
		PDF_begin_text();
        PDF_setfont(font,10);//TIMe 10??
		PDF_show_xydata4(pg+2,pgnum+1,503,47);
		font = PDF_load_font( "Helvetica",0, "host", "");
		PDF_setcolor("fill", "rgb", 0, 0, 1); 
		PDF_setfont(font,6);
	if(pg==pointl/600)
		{
			if(pointl%100==0)
			{lie_num=pointl%600/100;
			}else
			{lie_num=pointl%600/100+1;
			}
			h_num=100;
		}
		else
		{
			lie_num=6;h_num=100;
		}

	
		for(lie=0;lie<lie_num;lie++)
		{			if(pg==pgnum-1)
				{
					if(lie==lie_num-1)
					{
						if(pointl%100==0)
						{
							h_num=100;
						}else
						{	
							h_num=pointl%100;
						
						}
					}
				}
				
				for(h=0;h<h_num;h++)
				{	
					SPI_FLASH_BufferRead(Wendu_data_buf, Data_wendu_Adress, 2);
					Data_wendu_Adress+=2;
					D_wendu_value=((uint16_t)Wendu_data_buf[1] << 8) + Wendu_data_buf[0];
					if(C_F_FLAG==0)
					{
								wendu=(float)D_wendu_value/10;
					}else
					{
								wendu=32+((float)D_wendu_value*18)/100;
					}		
					if(MAK_point<MAK_NUM)
							{
										if(MAK_read==0)
										{				
															MAK_one_num=DataEEPROM_Read(0x08080210+MAK_point*4);//MAK_ONE_ADDRESS
															if(MAK_one_num%600==0)
														{
																MAK_page=MAK_one_num/600;
														}
														else
														{
																MAK_page=MAK_one_num/600+1;
														}
														MAK_hang=MAK_one_num%100;
														if(MAK_one_num%100==0)
														{
															MAK_lie=MAK_one_num%600/100;
														}else
														{
															MAK_lie=MAK_one_num%600/100+1;
														}
														if(MAK_hang==0)
														{
																MAK_hang=100;
														}
														MAK_read=1;
											}
										if(MAK_read==1)
										{
													if(MAK_page==(pg+1))
													{
														if(MAK_lie==(lie+1))
														{
																if(MAK_hang==(h+1))
																{	
																	font = PDF_load_font( "Helvetica-Bold",0, "host", "");
																	PDF_setcolor("fill", "rgb", 0, 0, 0);
																	PDF_show_xy("MARK",59+lie*79,760-h*7);
																		font = PDF_load_font( "Helvetica",0, "host", "");
																		PDF_setcolor("fill", "rgb", 0, 0, 1);

																	
																	MAK_read=0;MAK_point++;
																}
														}
													}
										}
							}

								PDF_show_xydata(month,day,year,hour,min,sec,wendu,59+lie*79,760-h*7);
	
				
					min=min+Set_Interl_time;
					if(min>=60)
					{
						hour=hour+min/60;
						min=min%60;
						if(hour>=24)
						{
							day++;
							hour=hour%24;
							if(month==4||month==6||month==9||month==11)
							{
									if(day>=31)
									{
										day=1;month++;
									}
							}
							else if(month==2)
							{
									if((year%4==0&&year%100!=0)||year%400==0)//??
									{
										if(day>=30)
										{
											day=1;month++;
										}
									}
									else
									{
										if(day>=29)
										{
											day=1;month++;
										}
									}
							}
							else
							{
									if(day>=32)
									{
										day=1;month++;
									}
							
							}
							if(month>12)
							{
								year++;
								month=1;
							}
						
						}
					}
				
				}
		}



		PDF_end_text();
		PDF_end_page_ext();
		}
		PDF_end_document();
		f_close(&fsrc);
		myfree(basepos);
		Endpdf:
		{	
		}

}
