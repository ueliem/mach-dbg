#include <iostream>
#include <mach/mach.h> 
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mach/i386/thread_status.h>
#include "taskprobe.h"

int main(int argc, char * argv[]) {
	TaskProbe t = TaskProbe();
	t.attach_to_exec("dummy", argv);
	if (t.is_attached_to_process()) {
		//t.resume_task();
		printf("%s\n", "Attached.");
		t.run();
	}
	return 0;
}
