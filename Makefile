build: good.c
	gcc -o good good.c

debug:
	gcc -g -o good good.c
	gdb good

clean:
	rm -f good