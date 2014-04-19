#include <iostream>
#include <mach/mach.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mach/i386/thread_status.h>
#include "taskprobe.h"
#include "breakpoint.h"

int main(int argc, char * argv[]) {
	TaskProbe t = TaskProbe();
	const char * prog = "dummy\0";
	t.attach_to_exec(prog, argv);
	if (t.is_attached_to_process()) {
		//t.resume_task();
		printf("%s\n", "Attached.");
		t.getAddressOrigin();
		t.set_breakpoint(0x0000000100000ef0);
		t.run();
	}
	return 0;
}
