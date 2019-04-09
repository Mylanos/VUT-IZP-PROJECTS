CFLAGS:-std=c99 -Werror -Wextra -pedantic -Wall
proj2: proj2.c
clean:
	-rm *.o $(objects)


1:
	./proj2 --log 0.25 1 >output.txt
	diff output.txt correct1.txt
2:
	./proj2 --log 1.131401114526 4 >output.txt
	diff output.txt correct2.txt
3:
	./proj2 --pow 0.25 2.5 1 >output.txt
	diff output.txt correct3.txt
4:
	./proj2 --pow 1.23 4.2 5 >output.txt
	diff output.txt correct4.txt
