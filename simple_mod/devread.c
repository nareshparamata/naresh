#include <stdio.h>
#include <fcntl.h>

int main()
{
   char buf[100];
   int stat,len;

   int fd = open("/dev/mydev", O_RDONLY);
   if(fd<0)
   {
     printf("Unable to open the device\n");
     exit(0);
   }
   printf("fd = %d\n",fd);

   /*** Read and display the contents ***/
   len = read(fd,buf,100);
   printf("%d bytes read from device \n", len);
   buf[len-1] = 0;
   puts(buf);
   close(fd);
}

   
