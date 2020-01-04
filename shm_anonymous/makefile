.PHONY: default
example_name=shm_anonymous.exe
FLAGS=-lpthread

default: ${example_name}

${example_name}: main.cpp
	g++ $^ ${FLAGS} -o $@

clean:
	rm -rf *.exe *~ *.o
