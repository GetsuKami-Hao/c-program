#include<stdio.h>

typedef union{
  unsigned short int value;
  unsigned char byte[2];
}to;


int main(void)
{
  to chek;
  chek.value = 0xabcd;
  
  if(chek.byte[0] == 0xcd && chek.byte[1] == 0xab)
  {
    printf("小端字节节序....\nbyte[0] = %x\nbyte[1] = %x\n,",chek.byte[0],chek.byte[1]);
  }

  if(chek.byte[0] == 0xab && chek.byte[1] == 0xcd)
  {
    printf("大端字节节序....\nbyte[0] = %x\nbyte[1] = %x\n,",chek.byte[0],chek.byte[1]);
  }

}
