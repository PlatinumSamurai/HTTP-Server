#include <stdio.h>
#include <unistd.h>

 int main() {
     char *file = "/bin/bash";
    char *const args[] = {"/bin/bash", "-c", "echo Hello World", NULL};
    char *const env[] = {"ENV=World", NULL};
	
    execve(file, args, NULL);
     return 0;
}
