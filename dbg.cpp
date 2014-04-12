#include <iostream>
#include <mach/mach.h> 
#include <sys/types.h>
#include <unistd.h>

class TaskProbe {
	public:
		TaskProbe();
		int attach_to_process(int);
	private:
		thread_act_port_array_t thread_list;
		mach_msg_type_number_t threadCount;
		mach_port_t task;
};

TaskProbe::TaskProbe() {
	return;
}

int TaskProbe::attach_to_process(int pid) {
	int machout = task_for_pid(mach_task_self(), pid, &task);
	printf("taskforpid %s\n", mach_error_string(machout));
	return 0;
}


int main(int argc, char * argv[]) {
	int pid = fork();
	if (pid == 0)
	{
		printf("I'm the child\n");
		//execv("first", const_cast<char**>(argv));
	}
	else
	{
		printf("I'm the parent, my child is %i\n", pid);
		TaskProbe t = TaskProbe();
		t.attach_to_process(pid);
	}
	return 0;
}
