#include "header.h"

const char * increaseOpStr = "increase";
const char * decreaseOpStr = "decrease";
size_t increaseOpStrLen = sizeof(increaseOpStr);
size_t decreaseOpStrLen = sizeof(decreaseOpStr);

mqd_t createMq(){
    struct mq_attr mqAttr;
    mqAttr.mq_msgsize = MQSTRSIZE;
    mqAttr.mq_maxmsg = MQMAXSIZE;
	mqd_t mq = mq_open(MQNAME,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR,&mqAttr);
	if(mq == (mqd_t)-1){
	    perror("mq_open");
	    assert(0);
	}
}

int strIsEqual(const char * str1, const char * str2, size_t len){
	for (int i = 0; i < len; ++i)
	{
		if (str1[i] != str2[i])
		{
			return false;
		}
	}
	return true;
}

void strMessageToStruct(const char * str, struct Message * msg){
	int index;
	char opStr[10];
	int op;
	sscanf(str,"[index=%d,op=%s]",&index,opStr);
	if (strIsEqual(increaseOpStr,opStr,increaseOpStrLen))
	{
		op = 0;
	}
	else if (strIsEqual(decreaseOpStr,opStr,decreaseOpStrLen))
	{
		op = 1;
	}
	else 
	{
		printf("str:%s\n",str);
		printf("opStr:%s\n",opStr);
		assert(0);
	}
	msg->index = index;
	msg->op = op;
}

void structToStrMessage(char * msgStr, struct Message * msg){
	assert(msg && msgStr);
	int flag = 0;
	if (msg->op == 0)
	{
		flag = sprintf(msgStr,"[index=%d,op=%s]",msg->index,increaseOpStr);
	}else if (msg->op == 1)
	{
		flag = sprintf(msgStr,"[index=%d,op=%s]",msg->index,decreaseOpStr);
	}
	assert(flag != -1);
}
