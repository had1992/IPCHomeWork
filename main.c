#include "header.h"

processCount = 0;
processNum = 0;
mqd_t mq;
struct SDBHandle sdbHandle;

void fatherClear(){
    for(;;){
        struct mq_attr mqAttr;
        mq_getattr(mq,&mqAttr);
        if(mqAttr.mq_curmsgs == 0){
            break;
        }
    }
    signal(SIGTERM,SIG_IGN);
    kill(0,SIGTERM);
    for(;;){
        int childPid = wait(NULL);
        if(childPid == -1){
            if (errno == ECHILD)
            {
                printf("No more children!\n");
                break;
            } else {
                printf("wait!\n");
            }
        }
    }

    syncSDBToLocalDB(LOCALDBNAME,&sdbHandle);

    printf("local DB:\n");
    printLocalDB(LOCALDBNAME);
    printf("share DB:\n");
    printSDB(&sdbHandle);

    mq_close(mq);
    shm_unlink(SDBNAME);
    sem_close(sdbHandle.sem);
    sem_unlink(SEMNAME);

    exit(EXIT_SUCCESS);
}

void SIGTERM_handler(int sig){
	fatherClear();
}

void SIGTERM_child_handler(int sig){
	printf("process:%d PID:%d has deal message num:%d\n",processNum,getpid(),processCount);
	exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[]) {
	mq = createMq();
	createSDB(&sdbHandle);

    signal(SIGTERM, SIGTERM_handler);

	for (int i = 0; i < childProcessNum; ++i) {
	    int pid = fork();
		if (pid == 0) {
			processCount = 0;
			processNum = i;
			signal(SIGTERM, SIGTERM_child_handler);
            printf("child Pid:%d\n",getpid());
			childProcess(mq, &sdbHandle);
			return 0;
		} else {
		}
	}

    printf("father Pid:%d\n",getpid());

	fatherProcess(mq, &sdbHandle);

	fatherClear();

	return 0;
}