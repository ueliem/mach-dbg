#include <iostream>
#include <mach/mach.h> 
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mach/i386/thread_status.h>
#include "taskprobe.h"

int main(int argc, char * argv[]) {
	int status;
	TaskProbe t = TaskProbe();
	t.attach_to_exec("dummy", argv);
	if (t.is_attached_to_process()) {
		//t.resume_task();
		printf("%s\n", "Attached.");
	}
	//waitpid(pid, &status, WNOHANG);
	waitpid(t.attached_process, &status, 0);
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
	return 0;
}
