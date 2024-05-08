#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define MSGSZ 128

struct msgbuf {
   long mtype;
   char mtext[MSGSZ];
};

int main(void) {
   struct msgbuf buf;

   key_t key = 100;
   int mid;
   // TODO: get the message queue id
   mid = msgget(key, 0666 | IPC_CREAT);
   
   while(1) {
      // TODO: receive a message from shared queue.
      msgrcv(mid, (void *)&buf, sizeof(buf.mtext), 1, 0); //
      printf("Received: %s", buf.mtext);
   }

   return 0;
}
