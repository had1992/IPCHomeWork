#include "header.h"

void createSDB(struct SDBHandle * sdbHandle){
	int fd = shm_open(SDBNAME,O_CREAT|O_RDWR,S_IWUSR|S_IRUSR);
	if (fd == -1){
	    perror("createSDB");
        assert(0);
	}
	ftruncate(fd,SDBSIZE);
	int * addr = (int *)mmap(NULL,SDBSIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	for(int i = MININDEX; i <= MAXINDEX; i++){
		addr[i] = 0;
	}
	sdbHandle->fd = fd;
	sdbHandle->addr = (void *)addr;
	sdbHandle->sem = sem_open(SEMNAME,O_CREAT);
}

void updateSDB(struct Message * msg, struct SDBHandle * sdbHandle){
	int * addr = (int *)(sdbHandle->addr);

	sem_wait(sdbHandle->sem);
	if(msg->op == 0){
		addr[msg->index]++;
	} else if (msg->op == 1){
		addr[msg->index]--;
	} else {
		assert(0);
	}
	sem_post(sdbHandle->sem);
}

void syncSDBToLocalDB(const char * fileName, struct SDBHandle * sdbHandle){
	int fd = open(fileName,O_RDWR|O_CREAT|O_TRUNC);
	if(fd == -1){
        perror("createLocalDB");
        assert(0);
	}
	char tabHead[] = "index\tcount\n";
	write(fd,tabHead,strlen(tabHead));
	for (int i = MININDEX; i <= MAXINDEX; ++i)
	{
		char tmp[20];
		sprintf(tmp,"%d\t\t%d\n",i,((int*)(sdbHandle->addr))[i]);
		write(fd,tmp,strlen(tmp));
	}
	write(fd,'\0',1);
	close(fd);
}

void printLocalDB(const char * fileName){
	int fd = open(fileName,O_RDWR);
	char content[1000];
	int readSize = read(fd,content,1000);
	content[readSize] = '\0';
    printf("%s\n", content);
	close(fd);
}

void printSDB(struct SDBHandle * sdbHandle){
	char content[1000];
	strcpy(content,"index\tcount\n");
	for (int i = MININDEX; i <= MAXINDEX; ++i)
	{
		char tmp[20];
		sprintf(tmp,"%d\t\t%d\n",i,((int*)(sdbHandle->addr))[i]);
		strcpy(content+strlen(content),tmp);
	}
	printf("%s\n", content);
}