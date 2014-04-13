all: dbg.cpp taskprobe.cpp
	g++ -o bin/dbg.o dbg.cpp taskprobe.cpp

run:
	sudo ./bin/dbg.o

clean:
	rm -rf *.o