#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
char binToHex(char num []);
char* binToSpecial (char num [],int i);


char* binToSpecial (char num [],int address){
  char num1 [] = "0000";
  char num2 [] = "0000";
  char num3 [] = "0000";
  char num4 [] = "0000";
  char num5 [] = "0000";
  int i,j;
  int count = 0;
  for(i=0; i<5; i++){
    for(j=0;j<4;j++){
      if(i == 0)
        num1[j] = num[count];
      else if(i == 1)
        num2[j] = num[count];
      else if(i == 2)
        num3[j] = num[count];
      else if(i == 3)
        num4[j] = num[count];
      else if(i == 4)
        num5[j] = num[count];
      count++;
    }
  }
  char a = binToHex(num1);
  char b = binToHex(num2);
  char c = binToHex(num3);
  char d = binToHex(num4);
  char e = binToHex(num5);

  /*printf("num = %s\n",num); */
 /*  printf("num1 = %s\nnum2 = %s\nnum3 = %s\nnum4 = %s\nnum5 = %s\n",num,num1,num2,num3,num4,num5); */
  if(i < 1000)
    printf("0%d\tA%c-B%c-C%c-D%c-E%c\n",address,a,b,c,d,e);
  else{
    printf("%d\tA%c-B%c-C%c-D%c-E%c\n",address,a,b,c,d,e);
  }
  return ("A%c-B%c-C%c-D%c-E%c\n",a,b,c,d,e); 
}


char binToHex(char num []){
  if(!strcmp(num,"0000"))
    return '0';
  if(!strcmp(num,"0001"))
    return '1';  
  if(!strcmp(num,"0010"))
    return '2';
  if(!strcmp(num,"0011"))
    return '3';
  if(!strcmp(num,"0100"))
    return '4';
  if(!strcmp(num,"0101"))
    return '5';
  if(!strcmp(num,"0110"))
    return '6';
  if(!strcmp(num,"0111"))
    return '7';
  if(!strcmp(num,"1000"))
    return '8';
  if(!strcmp(num,"1001"))
    return '9';
  if(!strcmp(num,"1010"))
    return 'a';
  if(!strcmp(num,"1011"))
    return 'b';
  if(!strcmp(num,"1100"))
    return 'c';  
  if(!strcmp(num,"1101"))
    return 'd';  
  if(!strcmp(num,"1110"))
    return 'e';  
  if(!strcmp(num,"1111"))
    return 'f';  
}
