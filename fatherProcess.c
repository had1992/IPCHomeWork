#include <stdio.h>
#include <stdlib.h>
#include "header.h"



void fatherProcess(mqd_t mq, struct SDBHandle * sdbHandle){
	char randMsgStr[MQSTRSIZE];
	struct Message randMsg;
	for(;;){
		int randIndex = rand()%10;
		int randOp = rand()%2;
		randMsg.index = randIndex;
		randMsg.op = randOp;
		structToStrMessage(randMsgStr,&randMsg);

		if(mq_send(mq,randMsgStr,MQSTRSIZE,0) == -1){
		    perror("mq_send");
		    assert(0);
		}
	}
}
