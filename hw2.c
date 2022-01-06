#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>

#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + (philCount - 1)) % philCount
#define RIGHT (phnum + 1) % philCount
#define SLEEPMILISECOND 1000

void test(int phnum);
void take_fork(int phnum);
void put_fork(int phnum);
void* philospher(void* num);
void* waiter(void* num);
void init();
void freeMemory();
void printInfo();

int philCount;
int waiterCount;
int philThinkTime;
int waiterwpTime;
int restaurantClosetime;
int totalPilav;
int waiterLiveCount;
int *state;
int *wter;
int *phil;
int *philEatenCount;
time_t begin;
time_t end;

pthread_mutex_t mutexlock;
pthread_mutex_t pilavZeroLock;
sem_t *S;
sem_t mutex;
sem_t full;
sem_t empty;
 
int main(int argc, char* argv[])
{
	printf("Filozof sayisini giriniz:");
	scanf("%d",&philCount);
	printf("Garson sayisini giriniz:");
	scanf("%d",&waiterCount);
	printf("Filozof dusunme suresini (ms) giriniz:");
	scanf("%d",&philThinkTime);
	printf("Garson whatsapp'da takilma (ms) giriniz:");
	scanf("%d",&waiterwpTime);
	printf("Restorantin acik kalma suresini (sn) giriniz:");
	scanf("%d",&restaurantClosetime);

		state = (int *) malloc(sizeof(int *) * philCount);
		wter = (int *) malloc(sizeof(int *) * waiterCount);
		phil = (int *) malloc(sizeof(int *) * philCount);
		philEatenCount = (int *) malloc(sizeof(int *) * philCount);
		S = (sem_t *) malloc(sizeof(sem_t *) * philCount);

		init();
		int i;
		pthread_t thread_id[philCount];
		pthread_t threadwaiter_id[waiterLiveCount];
		pthread_attr_t attr;
		if (pthread_attr_init (&attr) == -1){
				perror("init");
				exit(1);
		}

		// initialize the semaphores
		sem_init(&mutex, 0, 1);
		sem_init(&empty, 0, 2*philCount);
		sem_init(&full, 0, 0);
	 
		for (i = 0; i < philCount; i++)
			sem_init(&S[i], 0, 0);

		begin = time(NULL);
		for (i = 0; i < philCount; i++) {
			pthread_create(&thread_id[i], &attr, philospher, &phil[i]);
		}
		for (i = 0; i < waiterCount; i++) {
			pthread_create(&threadwaiter_id[i], &attr, waiter, &wter[i]);
		}
		
		for (i = 0; i < philCount; i++)
			pthread_join(thread_id[i], NULL);
		for (i = 0; i < waiterCount; i++)
			pthread_join(threadwaiter_id[i], NULL);

		printInfo();
}
void test(int phnum)
{
    if (state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        // state that eating
		sem_wait(&full);		
		if(totalPilav==0)
			pthread_mutex_lock(&pilavZeroLock);
		pthread_mutex_lock(&mutexlock);
			state[phnum] = EATING;
			philEatenCount[phnum]++;
			totalPilav--;
		pthread_mutex_unlock(&mutexlock);
		sem_post(&empty);

        // has no effect during takefork
        // used to wake up hungry philosophers
        // during putfork
        sem_post(&S[phnum]);
    }
}
 
// take up chopsticks
void take_fork(int phnum)
{
    sem_wait(&mutex);
    // state that hungry
    state[phnum] = HUNGRY;
	
    // eat if neighbours are not eating
    test(phnum);
 
    sem_post(&mutex);
 
    // if unable to eat wait to be signalled
    sem_wait(&S[phnum]);
 
}
 
// put down chopsticks
void put_fork(int phnum)
{
    sem_wait(&mutex);
    // state that thinking
    state[phnum] = THINKING;

    test(LEFT);
    test(RIGHT);
 
    sem_post(&mutex);
}
 
void* philospher(void* num)
{
    while (1) {
        int* i = num;
			if (time(NULL)-begin>=restaurantClosetime)
				break;
			usleep(philThinkTime*SLEEPMILISECOND);
			take_fork(*i);
			put_fork(*i);
    }
    pthread_exit(NULL);
}
void *waiter(void *num)
{
	while (1)
	{
		if (time(NULL)-begin>=restaurantClosetime)
			break;
		usleep(waiterwpTime*SLEEPMILISECOND);
		sem_wait(&empty);
		pthread_mutex_lock(&mutexlock);
			if(totalPilav==0)
				pthread_mutex_unlock(&pilavZeroLock);
			if(totalPilav < 2*philCount){
				totalPilav+=1;
			}
			waiterLiveCount--;
		pthread_mutex_unlock(&mutexlock);
		sem_post(&full);
	}
	pthread_exit(NULL);
}
void init(){
	totalPilav = philCount*2;
	waiterLiveCount = 0;
	for(int i = 0;i< philCount;i++){
		phil[i] = i;
		philEatenCount[i] = 0;
		state[i] = HUNGRY;
	}
}
void printInfo(){
		printf("\n-------------\n");
		for(int i = 0;i < philCount ;i++){
			printf("\nphilosopher %d eaten %d times ", i, philEatenCount[i]);
		}
}
 
 void freeMemory(){
	free(state);
	free(phil);
	free(philEatenCount);
	free(S);
	free(wter);
 }