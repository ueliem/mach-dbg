#include <iostream>
#include <mach/mach.h> 
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mach/i386/thread_status.h>
#include <ptrace.h>
#include "taskprobe.h"

TaskProbe::TaskProbe() {
	return;
}

int TaskProbe::attach_to_process(int pid) {
	int machout = task_for_pid(mach_task_self(), pid, &task);
	if (machout != KERN_SUCCESS) {
		printf("Failed to get task for pid. Error: %s\n", mach_error_string(machout));
		return -1;
	}
	attached_process = pid;
	pause_task();
	return 0;
}

bool TaskProbe::is_attached_to_process() {
	if(attached_process > 0) {
		return true;
	}
	return false;
}

int TaskProbe::pause_task() {
	if(task_suspend(task) != KERN_SUCCESS) {
		return -1;
	}
	return 0;
}

int TaskProbe::resume_task() {
	if(task_resume(task) != KERN_SUCCESS) {
		return -1;
	}
	return 0;
}
