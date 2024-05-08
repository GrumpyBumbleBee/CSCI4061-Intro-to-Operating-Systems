#include <stdio.h>
#include <signal.h>
#include <time.h> 
#include <unistd.h>


int printing = 1;

void sigint_handler(int sig) {
    printing = !printing; // Toggles printing
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
        if (printing)
            foo();
    }

    return 0;
}
