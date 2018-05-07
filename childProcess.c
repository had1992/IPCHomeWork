#include <stdio.h>
#include <mqueue.h>
#include "header.h"

void childProcess(mqd_t mq, struct SDBHandle * sdbHandle){
	char receiveMsgStr[MQSTRSIZE];
	struct Message receiveMsg;
	for(;;){
		assert(mq_receive(mq,receiveMsgStr,MQSTRSIZE,NULL));
		strMessageToStruct(receiveMsgStr,&receiveMsg);

		updateSDB(&receiveMsg, sdbHandle);

		processCount++;

		usleep(500000);
	}
}
