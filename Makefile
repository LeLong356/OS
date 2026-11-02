all: sum_serial sum_multi-thread

sum_serial: sum_serial.c
	gcc -Wall -o $@ $<

sum_multi-thread: sum_multi-thread.c
	gcc -Wall -pthread -o $@ $<

clean:
	rm -f sum_serial sum_multi-thread
