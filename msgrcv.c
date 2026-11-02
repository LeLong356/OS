#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/*
 * Filename: msgrcv.c
 */

#define PERMS 0644
#define MSG_KEY 0x123

struct my_msgbuf {
    long mtype;
    char mtext[200];
};

int main(void) {
    struct my_msgbuf buf;
    int msqid;
    int toend;

    if ((msqid = msgget(MSG_KEY, PERMS | IPC_CREAT)) == -1) { /* connect to the queue */
        perror("msgget");
        exit(1);
    }
    printf("message queue: ready to receive messages.\n");

    for (;;) { /* normally receiving never ends but just to make conclusion */
               /* this program ends with string of end */
        if (msgrcv(msqid, &buf, sizeof(buf.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        printf("recvd: \"%s\"\n", buf.mtext);
        toend = strcmp(buf.mtext, "end");
        if (toend == 0)
            break;
    }
    printf("message queue: done receiving messages.\n");
    return 0;
}
