all:
	gcc -std=c99 main.c -o museum_overseer -pthread
progfib: 
	gcc -std=c99 part_2/fib.c -o hw4_progfib -pthread
progcnt:
	gcc -std=c99 part_2/count.c -o hw4_progcnt -pthread
progqsort: 
	gcc -std=c99 part_2/qsort.c -o hw4_progqsort -pthread
proghw3:
	gcc -D_POSIX_C_SOURCE -std=c99 -Iinc part_3/main.c part_3/defs.c part_3/artist_ds.c -o artist_manager -pthread
clean:
	rm hw4_progfib
	rm hw4_progcnt
	rm hw4_progqsort
	rm artist_manager
	rm museum_overseer

