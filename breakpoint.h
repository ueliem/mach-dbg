class BreakPoint {
	public:
		BreakPoint();
		char original_byte;
		//int set_trigger(bool*);
		//int set(mach_port_t,vm_offset_t);
		int set(mach_port_t,const char *);
		int set(mach_port_t,unsigned long);
		int ResolveSymbolToAddress(const char *, void **);
	private:
		//bool (* trigger_logic_function)(bool);
};
