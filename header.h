#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/types.h>
#include "string.h"

#define childProcessNum 4
#define MQSTRSIZE 30
#define MQMAXSIZE 30
#define SDBSIZE 10000
#define MININDEX 0
#define MAXINDEX 9
#define LOCALDBNAME "localDB"
#define SDBNAME "/SDB"
#define MQNAME "/mq"
#define SEMNAME "/sem"

typedef enum
{
    true=1, false=0
}bool;

extern int processCount;
extern int processNum;

struct Message{
    int index;
    int op;
};
struct SDBHandle{
    int fd;
    void * addr;
    sem_t * sem;
};

extern mqd_t createMq();

extern void fatherProcess(mqd_t mq, struct SDBHandle *);

extern void childProcess(mqd_t mq, struct SDBHandle *);

extern void strMessageToStruct(const char * str, struct Message * res);

extern void structToStrMessage(char * msgStr, struct Message * msg);

extern void createSDB(struct SDBHandle * sdbHandle);

extern void updateSDB(struct Message * msg, struct SDBHandle * sdbHandle);

extern void syncSDBToLocalDB(const char * fileName, struct SDBHandle * sdbHandle);

extern void printLocalDB(const char * fileName);

extern void printSDB(struct SDBHandle * sdbHandle);

#endif

