.PHONY: default
example_name=dgram
prefix=.exe
FLAGS=

default: ${example_name}_server${prefix} ${example_name}_client${prefix}

${example_name}_server${prefix}: server.cpp
	g++  $^ ${FLAGS} -o $@

${example_name}_client${prefix}: client.cpp
	g++  $^ ${FLAGS} -o $@

run:
	./${example_name}_server${prefix} &
	./${example_name}_client${prefix}

clean:
	rm -rf *.exe *~ *.o
