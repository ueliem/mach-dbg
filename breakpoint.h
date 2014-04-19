class BreakPoint {
	public:
		BreakPoint();
		char original_byte;
		//int set_trigger(bool*);
		int set(mach_port_t,vm_offset_t);
	private:
		//bool (* trigger_logic_function)(bool);
};
