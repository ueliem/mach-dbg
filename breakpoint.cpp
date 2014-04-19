#include <iostream>
#include <mach/mach.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mach/i386/thread_status.h>
#include <mach/vm_map.h>
#include <mach/mach_vm.h>
#include "breakpoint.h"

BreakPoint::BreakPoint() {

}

/*int BreakPoint::set_trigger_function(bool (* func)) {
	trigger_logic_function = func;
}*/

int BreakPoint::set(mach_port_t task, vm_offset_t address) {
	vm_offset_t intermediary;
	mach_msg_type_number_t size = (size_t)1;
	//printf("%llu\n", size);
	int err;
	if(KERN_SUCCESS != (err = vm_protect(task, address, (size_t)1, false, VM_PROT_ALL))) {
		printf("%s\n", mach_error_string(err));
	}
	if (KERN_SUCCESS != (err = mach_vm_read(task, address, (mach_vm_size_t)1, (&intermediary), &size))) {
		printf("Read: %s\n", mach_error_string(err));
	}
	printf("Intermediary: %lu\n", intermediary);
	original_byte = intermediary;
	printf("Original Byte: %c\n", original_byte);
	vm_offset_t break_trigger;
	*(char *)break_trigger = 0xCC;
	if(KERN_SUCCESS != (err = mach_vm_write(task, address, break_trigger, size))) {
		printf("Write: %s\n", mach_error_string(err));
	}
	return 0;
}
