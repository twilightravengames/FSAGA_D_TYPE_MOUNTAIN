#include <conio.h>
#include <stdio.h>

main()
{
 int xcor;
 int ycor;
 for (xcor=0;xcor<100;xcor++)
 {
  for (ycor=0;ycor<100;ycor++)
  {
   if (xcor == 0)
   {
   printf("XCOR %d YCOR %d", xcor, ycor);
   }
  }
 }
}
