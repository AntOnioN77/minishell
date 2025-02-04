#include <stdio.h>
#include <errno.h>

int main() {
   for (int i = 150; i <= 200; i++) {
       errno = i;
       printf("errno %d: ", i);
       perror("");
   }
   return 0;
}