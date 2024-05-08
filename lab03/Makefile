fork_wait: fork_wait.c
	gcc -o fork_wait fork_wait.c

fork_exec: fork_exec.c
	gcc -o fork_exec fork_exec.c

.PHONY: all clean

all: fork_wait fork_exec

clean:
	rm -f fork_wait fork_exec
