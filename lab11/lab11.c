#include <stdio.h>
#include <signal.h>
#include <time.h> 
#include <unistd.h>

void foo() {

	static int count = 0;
	printf("%lu: Count = %d\n", time(NULL), ++count);
	sleep(1);
}

int main() {

	/* Print infinitely. */
	while (1) foo();
}
