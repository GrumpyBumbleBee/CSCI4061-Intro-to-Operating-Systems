#include <stdio.h>
#include <signal.h>
#include <time.h> 
#include <unistd.h>

void sigint_handler(int sig) {
    printf("SIGINT (CTRL+C) received, but printing continues.\n");
}

void foo() {
    static int count = 0;
    printf("%lu: Count = %d\n", time(NULL), ++count);
    sleep(1);
}

int main() {
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = sigint_handler;
    sigemptyset(&act.sa_mask);

    if (sigaction(SIGINT, &act, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    while (1) {
        foo();
    }

    return 0;
}
