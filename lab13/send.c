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
   // TODO: get message queue id
   mid = msgget(key, 0666 | IPC_CREAT);

   // set type to 10
   buf.mtype = 10;
   while(fgets(buf.mtext, MSGSZ, stdin) != NULL) {
   	// TODO: send a message
      buf.mtype = 1; // Set message type
      msgsnd(mid, (void *)&buf, sizeof(buf.mtext), 0); // Send message
   }

   // destory message queue
   msgctl(mid, IPC_RMID, NULL);

   return 0;
}
