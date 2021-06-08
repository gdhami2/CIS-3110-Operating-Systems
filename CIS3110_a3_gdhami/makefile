# Do not edit the contents of this file.
# Your code must compile using ths command.
a3: *.c
	touch virmem.c
	gcc -Werror -Wall -g -std=c99 -o virmem *.c

zip:
	zip -cvf cis3110_a3_gdhami.zip makefile virmem.c addresses.txt answers-a3.txt BACKING_STORE.bin 

.PHONY: clean
clean:
	rm -rf *o virmem
