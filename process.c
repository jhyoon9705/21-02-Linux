// process.c
#include <stdio.h>
#include <unistd.h>

int main() {
  printf("Before fork\n");
  pid_t pid = fork();
  
  int i;
  int iterator=0;
  char* msg;
  
  if(pid == -1) {
    printf("Fork failed!\n");
    return -1;
  }
  
  if(pid == 0) {
    iterator = 2;
    msg = "Child process...";
  }
  else {
    iterator = 3;
    msg = "Parent process...";
  }
  
  for( ; iterator>0; iterator--)
    puts(msg);
  
  return 0;
}
