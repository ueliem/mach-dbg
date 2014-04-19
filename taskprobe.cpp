#include <iostream>
#include <mach/mach.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mach/i386/thread_status.h>
#include <mach/vm_map.h>
#include <mach/mach_vm.h>
#include <dlfcn.h>
//#include <sys/ptrace.h>
#include "taskprobe.h"
#include "breakpoint.h"

TaskProbe::TaskProbe() {
	return;
}

int TaskProbe::getAddressOrigin() {
	vm_map_size_t vmsize;
    uint32_t nesting_depth = 0;
    struct vm_region_submap_info_64 vbr;
    mach_msg_type_number_t vbrcount = 16;

	mach_vm_region_recurse(task, &baseaddress, &vmsize, &nesting_depth, (vm_region_recurse_info_t)&vbr, &vbrcount);
	printf("%s%lld\n", "Address: ", baseaddress);
	return 0;
}

/*int TaskProbe::getAddressBySymbol(char * symbol[]) {

}*/

int TaskProbe::attach_to_exec(const char * executablename, char * argv[]) {
	int childpid = fork();
	if (childpid == 0) {
		//ptrace(PT_TRACE_ME, 0, 0, 0);
		execv(executablename, const_cast<char**>(argv));
	}
	else if (childpid > 0) {
		//printf("About to pause.");
		attach_to_process(childpid);
	}
	else {
		printf("Failed to fork child. ");
		return -1;
	}
	return 0;
}

int TaskProbe::attach_to_process(int pid) {
	int machout = task_for_pid(mach_task_self(), pid, &task);
	if (machout != KERN_SUCCESS) {
		printf("Failed to get task for pid. Error: %s\n", mach_error_string(machout));
		return -1;
	}
	//ptrace(PT_ATTACH, pid, 0, 0);
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
	//ptrace(PT_CONTINUE, attached_process, (caddr_t)1, 0);
	return 0;
}

int TaskProbe::step_task() {
	//ptrace(PT_STEP, attached_process, (caddr_t)1, 0);
	return 0;
}

int TaskProbe::set_breakpoint(long long address) {
	BreakPoint b = BreakPoint();
	b.set(task, address + baseaddress/*MACH_VM_MIN_ADDRESS*/);
	return 0;
}

int TaskProbe::run() {
	running = true;
	int status;
	resume_task();
	//waitpid(pid, &status, WNOHANG);
	waitpid(attached_process, &status, 0);
	while (running && WIFSTOPPED(status)) {
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
		//waitpid(pid, &status, WNOHANG);
		waitpid(attached_process, &status, WNOHANG);
	}
	return 0;
}
