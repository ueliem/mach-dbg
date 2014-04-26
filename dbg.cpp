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
	const char * prog = "dummy2\0";
	const char * symbol = "main\0";
	t.attach_to_exec(prog, argv);
	if (t.is_attached_to_process()) {
		//t.resume_task();
		t.getAddressOrigin();
		printf("%s\n", "Attached.");
		t.set_breakpoint(0x100000ef0);
		//t.set_breakpoint(symbol);
		t.run();
	}
	return 0;
}
