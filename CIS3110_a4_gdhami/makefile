
all: igoodcnt

igoodcnt: *.c
	gcc -o igoodcnt igoodcnt.c -lpthread -lrt

zip:
	zip -cvf CIS3110_a4_gdhami.zip makefile answers-a4.txt igoodcnt.c

.PHONY: clean
clean:
	rm -rf *o igoodcnt
