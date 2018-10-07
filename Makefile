# NAME: Matthew Paterno
# EMAIL: mpaterno@g.ucla.edu
# ID: Matthew Paterno

.SILENT:

default: 
	gcc -Wall -Wextra -std=c99 -o lab0 -g lab0.c

check: default ioTest catchTest readTest writeTest copyTest
	if [[ -s log.txt ]]; then \
		cat log.txt; \
	  rm -f log.txt; \
	else \
		echo "Passed smokecheck."; \
	fi;
clean:
	rm -f lab0 *.o *.txt *.tar.gz
dist: default
	tar -czvf lab0-904756085.tar.gz lab0.c breakpoint.png backtrace.png README Makefile

# Smoke-tests to make sure program works. Called by make check


ioTest:
	echo "Hello World!" > input.txt; \
	./lab0 -i input.txt -o output.txt; \
	if [[ $$? -ne 0 ]]; then \
		echo "ERROR: Program did not reach end." >> logfile.txt; \
	fi; \
	rm -f input.txt output.txt
readTest:
	echo "Hello World!" > input.txt; \
	chmod u-r input.txt; \
	echo | ./lab0 -i input.txt &>/dev/null;\
	if [[ $$? -ne 2 ]]; \
	then \
		echo "ERROR: Program did not handle unreadable input.txt correctly." >> logfile.txt; \
	fi; \
	rm -f input.txt
writeTest:
	echo "Hello World!" > input.txt; \
	touch output.txt; \
	chmod u-w output.txt; \
	./lab0 -i input.txt -o output.txt &>/dev/null; \
	if [[ $$? -ne 3 ]]; \
	then \
		echo "ERROR: Program did not handle unwritable output.txt correctly." >> logfile.txt; \
	fi; \
	rm -f input.txt output.txt
copyTest:
	echo "Hello World!" > input.txt; \
	./lab0 -i input.txt -o output.txt &>/dev/null; \
	cmp input.txt output.txt; \
	if [[ $$? -ne 0 ]]; \
	then \
		echo "ERROR: Input.txt and Output.txt do not share same contents." >> logfile.txt; \
	fi; \
	rm -f input.txt output.txt

catchTest:
	echo | ./lab0 -s -c; \
	if [[ $$? -ne 4 ]]; \
	then \
	echo "ERROR: Catch unsuccessful." >> logfile.txt; \
	fi;