# Alexandra Postolaki (posto022) Lab02
# TODO: complete recipe main
main: file1.o file2.o file3.o file4.o
	gcc -o main main.c file1.o file2.o file3.o file4.o

file1.o: file1.c file1.h
	gcc -c file1.c file1.h

# TODO: complete recipe file2~4
file2.o: file2.c file2.h
	gcc -c file2.c file2.h

file3.o: file3.c file3.h
	gcc -c file3.c file3.h

file4.o: file4.c file4.h
	gcc -c file4.c file4.h

main.o: main.c main.h
	gcc -c main.c main.h

.PHONY: clean

clean :
	rm *.o

