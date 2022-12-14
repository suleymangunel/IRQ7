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

/*global de?i?ken */
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
/*Kesme hizmet program?... */
{
 disable();
 int_processed();
 int_occurred=TRUE;
 enable();
}

void open_intserv(void)
/* IRQ7 kesme aktif yapilir.
 * Kesme oldu?unda (low on / ACK) intserv ca??r?l?r.
 * Bu fonksiyon sirasinda b?t?n kesmeler pasif yap?l?r.
 * C?k??ta aktif yap?l?r.
 */

{
 int int_mask;
 disable();                      /* b?t?n kesmeleri pasif yap */
 oldfunc=getvect(intlev);        /* eski vekt?r? sakla */
 setvect(intlev,intserv);        /* yeni kesme programi */
 int_mask=inportb(0x21);         /* 1101 1111 */
 outportb(0x21,int_mask & ~0x80);/* bit 7=0 */
				 /* digerleri ayn? */
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
/* PC taraf?ndan kesmenin bitirildi?i 8259'a bildirilir. */
{
 outportb(0x20,0x20);
}


