#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#define MAX_LINE 1024

int main(int argc, char *argv[])
{
   char buffer[MAX_LINE];
   printf(1, "Enter number : \n");
   read(1, buffer, MAX_LINE);
   int number = atoi(buffer);
   int reg_value;

   __asm__("movl %%edx, %0" : "=r" (reg_value));
   __asm__("movl %0, %%edx" :  : "r"(number) );

   count_num_of_digits();

   __asm__("movl %0, %%edx" :  : "r"(reg_value));
   exit();
}