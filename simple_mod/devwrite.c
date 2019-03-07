#include <stdio.h>
#include <fcntl.h>

int main()
{
   char buf[100];
   int stat;

   int fd = open("/dev/mydev", O_WRONLY);
   if(fd<0)
   {
     printf("Unable to open the device\n");
     exit(0);
   }
   printf("fd = %d\n",fd);

   printf("Enter some string to write into device\n");
   fgets(buf,100,stdin);
   stat = write(fd,buf,strlen(buf)+1);
   printf("written %d characters\n",stat);
   close(fd);
}

   
