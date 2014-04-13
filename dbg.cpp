#include <iostream>
#include <mach/mach.h> 
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mach/i386/thread_status.h>
#include "taskprobe.h"

int main(int argc, char * argv[]) {
	int status;
	int pid = fork();
	if (pid == 0) {
		//printf("I'm the child\n");
		execv("dummy", const_cast<char**>(argv));
	}
	else if (pid > 0) {
		//printf("I'm the parent, my child is %i\n", pid);
		TaskProbe t = TaskProbe();
		printf("%s\n", "Getting ready to attach.");
		t.attach_to_process(pid);
		if (t.is_attached_to_process()) {
			//t.resume_task();
		}
		//waitpid(pid, &status, WNOHANG);
		waitpid(pid, &status, 0);
		printf("%i\n", status);
		if(WIFSTOPPED(status)) {
			printf("Stopped.");
		}
		if(WIFCONTINUED(status)) {
			printf("Continued.");
		}
		//if(WIFEXITTED(status)) {
		//	printf("Exitted.");
		//}
		if(WIFSIGNALED(status)) {
			printf("Signaled.");
		}
	}
	else {
		printf("Failed to fork child. ");
		return -1;
	}
	return 0;
}
