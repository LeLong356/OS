#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PERMS 0644
#define MSG_KEY 0x123
struct my_msgbuf {
      long mtype;
      char mtext[200];
};

int msqid;                       
long send_type;                   
long recv_type;                  

void finish(void) {
     fflush(stdout);
     _exit(0);   
}


void *sender(void *arg) {
     struct my_msgbuf buf;
     int len;

     printf("message queue: ready to send messages.\n");
     printf("Enter lines of text, ^D to quit:\n");
     buf.mtype = send_type;

     while (fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL) {
            len = strlen(buf.mtext);
          
            if (buf.mtext[len - 1] == '\n') buf.mtext[len - 1] = '\0';
            if (msgsnd(msqid, &buf, len + 1, 0) == -1) 
            perror("msgsnd");
     }
     strcpy(buf.mtext, "end");
     len = strlen(buf.mtext);
     if (msgsnd(msqid, &buf, len + 1, 0) == -1)
     perror("msgsnd");

     if (msgctl(msqid, IPC_RMID, NULL) == -1) {
          perror("msgctl");
          exit(1);
     }
     printf("message queue: done sending messages.\n");
     finish();
     return NULL;
}

void *receiver(void *arg) {
     struct my_msgbuf buf;
     int toend;

     printf("message queue: ready to receive messages.\n");

     for (;;) { 
          if (msgrcv(msqid, &buf, sizeof(buf.mtext), recv_type, 0) == -1) {
               break;   
          }
          printf("recvd: \"%s\"\n", buf.mtext);
          toend = strcmp(buf.mtext, "end");
          if (toend == 0)
               break;
     }
     printf("message queue: done receiving messages.\n");
     finish();
     return NULL;
}

int main(int argc, char *argv[]) {
     pthread_t t_send, t_recv;

     if (argc != 2) {
          printf("Usage: %s 1   or   %s 2\n", argv[0], argv[0]);
          return 1;
     }
     if (strcmp(argv[1], "1") == 0) {
          send_type = 1;
          recv_type = 2;
     } else if (strcmp(argv[1], "2") == 0) {
          send_type = 2;
          recv_type = 1;
     } else {
          printf("Role must be 1 or 2\n");
          return 1;
     }

     
     if ((msqid = msgget(MSG_KEY, PERMS | IPC_CREAT)) == -1) {
          perror("msgget");
          exit(1);
     }

     pthread_create(&t_send, NULL, sender, NULL);
     pthread_create(&t_recv, NULL, receiver, NULL);
     pthread_join(t_send, NULL);   
     pthread_join(t_recv, NULL);
     return 0;
}