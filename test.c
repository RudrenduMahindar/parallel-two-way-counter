#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>


struct timeval tZero;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

int sum = 1;
int serveThreadFlag = -1;

void *add(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		sum += 1;
		if(sum > 50)
		{
			pthread_mutex_unlock(&mutex);
			break;
		}
		serveThreadFlag = -1;
		if(serveThreadFlag == -1)
			printf("I am inside add thread with sum = %d\n",sum);
		pthread_cond_broadcast(&cv);
		pthread_mutex_unlock(&mutex);
		usleep(1000);
	
	}


}

void *serve(void *arg)
{
	
	while(1)
	{

		pthread_mutex_lock(&mutex);
		if(sum > 50)
		{
			pthread_mutex_unlock(&mutex);
			break;
		}
		while(sum % 5 != 0 && sum <= 50)
		{
			printf("I am inside serve thread with sum = %d which is not divisible by 5 \n",sum);
			pthread_cond_wait(&cv,&mutex);

		}
		if(serveThreadFlag == -1)
		{
			serveThreadFlag = 1;
			printf("I am inside serve thread with sum = %d\n",sum);
		} 
		
		pthread_cond_broadcast(&cv);
		pthread_mutex_unlock(&mutex);
	
	}



}



int main()
{
	pthread_t addThread;
	pthread_create(&addThread,NULL,add,0);
	pthread_t serveThread;
	pthread_create(&serveThread,NULL,serve,0);
	void *addThreadResult = NULL;
	void *serveThreadResult = NULL;

	pthread_join(addThread,(void **)&addThreadResult);
	pthread_join(serveThread,(void **)&serveThreadResult);

	return 0;
}
