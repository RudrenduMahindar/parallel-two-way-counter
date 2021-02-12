#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>

#define MAX_SUM 10

struct timeval tZero;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t add_cv = PTHREAD_COND_INITIALIZER;
pthread_cond_t serve_cv = PTHREAD_COND_INITIALIZER;

bool add_data = true;

int sum = 0;

//int serveThreadFlag = -1;

void *add(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(add_data==false)
		{
			pthread_cond_wait(&add_cv, &mutex);
		}

		sum += 1;
		if(sum > MAX_SUM) // the add thread is exiting so wake up the serve thread
		{
			add_data = false;
			pthread_cond_broadcast(&serve_cv);
			pthread_mutex_unlock(&mutex);
			break;
		}
		
		printf("I am inside add thread with sum = %d\n",sum);
		add_data = false;
		pthread_cond_broadcast(&serve_cv);
		pthread_mutex_unlock(&mutex);
		//usleep(1000);
	
	}

}

void *serve(void *arg)
{
	
	while(1)
	{

		pthread_mutex_lock(&mutex);
		
		while(add_data==true)
		{
			//printf("I am inside serve thread with sum = %d which is not divisible by 5 \n",sum);
			//add_data = true;
			//pthread_cond_broadcast(&add_cv);
			pthread_cond_wait(&serve_cv,&mutex);
		}
		
		if(sum > MAX_SUM)
		{
			pthread_mutex_unlock(&mutex);
			break;
		}

		sum+=1;
		
		add_data = true;
		printf("I am inside serve thread with sum = %d\n",sum);
		 
		
		pthread_cond_broadcast(&add_cv);
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