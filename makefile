all: dbg.cpp taskprobe.cpp breakpoint.cpp
	g++ -o bin/dbg.o dbg.cpp taskprobe.cpp breakpoint.cpp

run:
	sudo ./bin/dbg.o

clean:
	rm -rf *.o