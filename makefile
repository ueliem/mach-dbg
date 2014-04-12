all: dbg.cpp
	g++ -o bin/dbg.o dbg.cpp

run:
	sudo ./bin/dbg.o

clean:
	rm -rf *.o