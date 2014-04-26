all: dbg.cpp taskprobe.cpp breakpoint.cpp
	g++ -arch x86_64 -o bin/dbg.o dbg.cpp taskprobe.cpp breakpoint.cpp

run:
	sudo ./bin/dbg.o

clean:
	rm -rf *.o
