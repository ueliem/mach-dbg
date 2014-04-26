#include <iostream>
#include <mach/mach.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mach/i386/thread_status.h>
#include <mach/vm_map.h>
#include <mach/mach_vm.h>
#include <dlfcn.h>
#include "breakpoint.h"

BreakPoint::BreakPoint() {

}

int BreakPoint::ResolveSymbolToAddress(const char * sym, void ** addr) {
	void * handle = dlopen("dummy", RTLD_LAZY);
	if (nullptr == handle) {
		printf("Failed to open with dlopen! Error: %s\n", dlerror());
		return -1;
	}
	if(nullptr == (*addr = dlsym(handle, sym))) {
		printf("Failed to resolve symbol using dlsym(3). Error: %s\n", dlerror());
		return -1;
	}
	//unsigned long long * addrptr = static_cast<unsigned long long *>(handle);
	printf("Found handle for %s at some address.\n", sym);
	//addr = handle;
	return 0;
}

/*int BreakPoint::set(mach_port_t task, const char * sym) {
	void * symaddr;
	ResolveSymbolToAddress(sym, &symaddr);
	set(task, symaddr);
	return 0;
}*/

int BreakPoint::set(mach_port_t task, unsigned long address) {
	printf("Setting breakpoint at address: %lu\n", address);
	int err;
	if(task_suspend(task) != KERN_SUCCESS) {
		printf("Suspend: %s\n", mach_error_string(err));
		return -1;
	}
	vm_offset_t intermediary;
	mach_msg_type_number_t size = (size_t)1;
	//printf("%llu\n", size);
	//char orig = static_cast<char>(*address);
	if(KERN_SUCCESS != (err = vm_protect(task, (vm_offset_t)address, (size_t)1, false, VM_PROT_ALL))) {
		printf("Protect: %s\n", mach_error_string(err));
	}
	/*if (KERN_SUCCESS != (err = mach_vm_read(task, address, (mach_vm_size_t)1, (&intermediary), &size))) {
		printf("Read: %s\n", mach_error_string(err));
	}
	printf("Intermediary: %lu\n", intermediary);
	original_byte = intermediary;
	printf("Original Byte: %c\n", original_byte);
	char value = 0xCC;
	vm_offset_t break_trigger = (unsigned long)&value;
	*(char *)break_trigger = 0xCC;
	if(KERN_SUCCESS != (err = mach_vm_write(task, address, break_trigger, size))) {
		printf("Write: %s\n", mach_error_string(err));
	}*/
	if(task_resume(task) != KERN_SUCCESS) {
		printf("Resume: %s\n", mach_error_string(err));
		return -1;
	}
	return 0;
}
