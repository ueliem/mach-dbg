#include <iostream>
#include <mach/mach.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mach/i386/thread_status.h>
#include <mach/vm_map.h>
#include <mach/mach_vm.h>
#include <dlfcn.h>
#include <sys/ptrace.h>
#include <errno.h>
#include <sys/mman.h>
#include "taskprobe.h"
#include "breakpoint.h"

TaskProbe::TaskProbe() {
	return;
}

int TaskProbe::attach_to_exec(const char * executablename, char * argv[]) {
	int childpid = fork();
	if(childpid == 0) {
		ptrace(PT_TRACE_ME,0,0,0);
		execl(executablename, argv[1] ,0);
		printf("Failed to execute %s!\n", argv[1]);
	}
	else if (childpid > 0) {
		int machout = task_for_pid(mach_task_self(), childpid, &task);
		if (machout != KERN_SUCCESS) {
			printf("Failed to get task for pid. Error: %s\n", mach_error_string(machout));
			return -1;
		}
		//Making the call to PT_ATTACH is unnecessary, apparently. Doing it causes PT_CONTINUE to fail.
		/*if (-1 == ptrace(PT_ATTACH, pid, 0, 0)) {
			int errsv = errno;
			printf("Failed to attach to process with ptrace!");
			printf("Error: %s\n", strerror(errsv));
			return -1;
		}*/
		attached_process = childpid;
	}
	else {
		printf("Failed to fork child. \n");
		return -1;
	}
	return 0;
}

int TaskProbe::attach_to_process(int pid) {
	if (-1 == ptrace(PT_ATTACH, pid, 0, 0)) {
		int errsv = errno;
		printf("Failed to attach to process with ptrace!");
		printf("Error: %s\n", strerror(errsv));
		return -1;
	}
	attached_process = pid;
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

int TaskProbe::step_task() {
	//ptrace(PT_STEP, attached_process, (caddr_t)1, 0);
	return 0;
}

/*int TaskProbe::set_breakpoint(const char * sym) {
	BreakPoint b = BreakPoint();
	b.set(task, sym);
	return 0;
}*/

int TaskProbe::set_breakpoint(unsigned long addr) {
	BreakPoint b = BreakPoint();
	b.set(task, addr);
	return 0;
}

int TaskProbe::getAddressOrigin() {
	return 0;
}

int TaskProbe::run() {
	running = true;
	int status, ret;
	wait(&status);
	//The child must be confirmed to have actually stopped before PT_CONTINUE can work.
	if (WIFSTOPPED(status)) {
		printf("Child stopped initially. Process pid: %d\n", attached_process);
	}
	errno = 0;
	if(0 != (ret = ptrace(PT_CONTINUE, attached_process, (caddr_t)1, 0))) {
		printf("ret: %d\n", ret);
		printf("errno: %s\n", strerror(errno));
	}
	printf("Child started. Process pid: %d\n", attached_process);
	while (running) {
		sleep (1);
		wait(&status);
		if(WIFSTOPPED(status)) {
			printf("child stopped!\n");
		}
		if(WIFEXITED(status)) {
			printf("child exitted!\n");
			break;
		}
	}
	printf("Parent exitting.\n");
	return 0;
}
