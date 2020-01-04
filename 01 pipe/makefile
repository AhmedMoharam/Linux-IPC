.PHONY: default
example_name=pipe.exe

default: ${example_name}

${example_name}: main.cpp
	g++ $^ -o $@

clean:
	rm -rf *.exe *~ *.o
