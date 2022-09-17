#include <stdio.h>
#include <bios.h>
#include <dos.h>

#define DATA    0x0378
#define STATUS  DATA+1
#define CONTROL DATA+2
#define TRUE    1
#define FALSE   0

void open_intserv(void);
void close_intserv(void);
void int_processed(void);
void interrupt far intserv(void);

/*IRQ7 interrupt level */
int intlev=0x0f;
void interrupt far (*oldfunc) ();

/*global deßiüken */
int int_occurred=FALSE;

int main(void)
{
 open_intserv();
 outportb(CONTROL, inportb(CONTROL) | 0x10);
/*kontrol portunun 4. bitini 1 yap */

 clrscr();
 printf("Kesme bekleniyor...\n");

 while(1)
 {
  if(int_occurred)
  {
   sound(3000);
   delay(5);
   sound(2000);
   delay(5);
   sleep(1);
   nosound();
   int_occurred=FALSE;
  }
 }
 close_intserv();
 return(0);
}

void interrupt far intserv(void)
/*Kesme hizmet programç... */
{
 disable();
 int_processed();
 int_occurred=TRUE;
 enable();
}

void open_intserv(void)
/* IRQ7 kesme aktif yapilir.
 * Kesme oldußunda (low on / ACK) intserv caßçrçlçr.
 * Bu fonksiyon sirasinda bÅtÅn kesmeler pasif yapçlçr.
 * Cçkçüta aktif yapçlçr.
 */

{
 int int_mask;
 disable();                      /* bÅtÅn kesmeleri pasif yap */
 oldfunc=getvect(intlev);        /* eski vektîrÅ sakla */
 setvect(intlev,intserv);        /* yeni kesme programi */
 int_mask=inportb(0x21);         /* 1101 1111 */
 outportb(0x21,int_mask & ~0x80);/* bit 7=0 */
				 /* digerleri aynç */
 enable();
}

void close_intserv(void)
{
 int int_mask;
 disable();
 setvect(intlev,oldfunc);
 int_mask=inportb(0x21) | 0x80; /* bit 7=1 */
 outportb(0x21,int_mask);
 enable();
}

void int_processed(void)
/* PC tarafçndan kesmenin bitirildißi 8259'a bildirilir. */
{
 outportb(0x20,0x20);
}


