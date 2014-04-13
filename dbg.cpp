#include <iostream>
#include <mach/mach.h> 
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mach/i386/thread_status.h>

class TaskProbe {
	public:
		TaskProbe();
		int attach_to_process(int);
		bool is_attached_to_process();
		int attached_process;
		int pause_task();
		int resume_task();
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
			t.resume_task();
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
