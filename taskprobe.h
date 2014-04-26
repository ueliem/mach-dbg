class TaskProbe {
	public:
		TaskProbe();
		bool running;
		int attach_to_exec(const char *, char **);
		int attach_to_process(int);
		bool is_attached_to_process();
		int attached_process;
		int pause_task();
		int resume_task();
		int step_task();
		int set_breakpoint(unsigned long);
		int set_breakpoint(const char *);
		int run();
		int getAddressOrigin();
		int getSymbolsFromTable();
	private:
		thread_act_port_array_t thread_list;
		mach_msg_type_number_t threadCount;
		mach_port_t task;
		vm_offset_t baseaddress;
};
