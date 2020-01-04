.PHONY: default
example_name=fifo
prefix=.exe
default: ${example_name}_server${prefix} ${example_name}_client${prefix}

${example_name}_server${prefix}: server.cpp
	g++ $^ -o $@

${example_name}_client${prefix}: client.cpp
	g++ $^ -o $@

run:
	./${example_name}_server${prefix} &
	./${example_name}_client${prefix}

clean:
	rm -rf *.exe *~ *.o
