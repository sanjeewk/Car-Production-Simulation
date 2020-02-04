#include "definitions.h"
#include "main.h"
#include <omp.h>

sem_t sem_worker;
sem_t sem_space;

sem_t sem_skeleton;
sem_t sem_engine;
sem_t sem_chassis;
sem_t sem_body;

sem_t sem_window;
sem_t sem_tire;
sem_t sem_battery;
sem_t sem_car;

unsigned int num_cars;
unsigned static int NUM_PARTS = 17; // number of parts for making one car
unsigned int num_spaces;
unsigned int num_workers;

int main(int argc, char** argv)
{
	  if (argc < 4) {
	  printf("Usage: %s <number of cars> <number of spaces> <number of workers>\n", 
	  argv[0]);
	  return EXIT_SUCCESS;
	  }
	  num_cars     = atoi(argv[1]);
	  num_spaces   = atoi(argv[2]);
	  num_workers  = atoi(argv[3]);

	///////////////////////////////////////////////////////
	int x =17*num_cars;
	// if(num_workers>num_spaces){
	// 	num_workers = num_spaces;
	// }
	printf("Name: Sanjeew Kanagaraj  UID: 3035492513\n");
	printf("Job defined, %d workers will build %d cars with %d storage spaces\n", 
			num_workers, num_cars, num_spaces);
	fflush(stdout);

	resource_pack *rpack = (struct resource_pack*) malloc(sizeof(struct resource_pack));

	// put semaphores into resource_pack
	initResourcePack(rpack, num_spaces, num_workers);

	//threads
	pthread_t threads[num_cars * NUM_PARTS];
	// pthread_t *threads = malloc(num_workers * sizeof(pthread_t));

	// prepare work_pack
	work_pack wpack; 
	wpack.resource = rpack;
	wpack.tid = 0;
	wpack.jobQ = queueCreate(num_cars * NUM_PARTS);
	int i, j, k, rc;

	// Start timing the whole process: scheduling + production
	double totalTime = omp_get_wtime();
	// Phase 1: Job scheduling, enqueue jobs' ID to jobQ
	// 8 production tasks to be done and their job ID is from 0 to 7
	// Hint: We need 7 windows and 4 tires to make a car,
	// so when i equal to WINDOW and TIRE we need to enqueue
	// 7 and 4 times respectively. Otherwise only enqueue once
	//TODO enqueue jobID to make a car
	int part;
	int order[8] = {3,0,1,2,7,4,5,6};
	for(k =0; k < num_cars; k++){
		for ( i = 0; i < 8; i++)
		{
			if (i==5)
			{
				for ( j = 0; j < 7; j++)
				{
					part = order[i];
					queueEnqueue(wpack.jobQ, &part);
				}
			}
			else if (i==6)
			{
				for ( j = 0; j < 4; j++)
				{
					part = order[i];
					queueEnqueue(wpack.jobQ, &part);
				}
			}
			else
			{
				part = order[i];
				queueEnqueue(wpack.jobQ, &part);
			}
		}
	}
	if(wpack.jobQ->capacity < 20){
		queuePrint(wpack.jobQ);
		}

	//Phase 2: Production
	
	for (i = 0; i < (x); i++)
	{	
		int w = 1;
		sem_wait(&sem_worker);
		while(w==1){
			int *sem_v = malloc(sizeof(int));
			sem_getvalue(&sem_space, sem_v);
			if(*sem_v >0){
				w = 0;
			}
		}
		
		wpack.tid = (i%num_workers);
		fflush(stdout);
		// workerThreadFuc(&wpack);
		if ((rc = pthread_create(&threads[i], NULL,*workerThreadFuc ,&wpack ))){
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return EXIT_FAILURE;
		}
	}
	for (i = 0; i < (x); i++)
	{		
		pthread_join(threads[i], NULL); 
	}

	totalTime = omp_get_wtime() - totalTime;
	reportResults(totalTime);

	destroySem();
	queueDestroy(wpack.jobQ);
	free(rpack);
	return EXIT_SUCCESS;
}

void reportResults(double production_time) {
	int *sem_value = malloc(sizeof(int));
	printf("=====Production report=====\n");
	printf("Number of workers: %d, Storage space size: %d\n", num_workers, num_spaces);
	sem_getvalue(&sem_skeleton, sem_value);
	printf("Unused Skeleton: %d\n",   *sem_value);
	sem_getvalue(&sem_engine,   sem_value);
	printf("Unused Engine: %d\n",     *sem_value);
	sem_getvalue(&sem_chassis,  sem_value);
	printf("Unused Chassis: %d\n",    *sem_value);
	sem_getvalue(&sem_body,     sem_value);
	printf("Unused Body: %d\n",       *sem_value);
	sem_getvalue(&sem_window,   sem_value);
	printf("Unused Window: %d\n",     *sem_value);
	sem_getvalue(&sem_tire,     sem_value);
	printf("Unused Tire: %d\n",       *sem_value);
	sem_getvalue(&sem_battery,  sem_value);
	printf("Unused Battery: %d\n",    *sem_value);

	sem_getvalue(&sem_space, sem_value);
	if (*sem_value < num_spaces) {
		printf("There are waste car parts!\n");
	}
	sem_getvalue(&sem_car, sem_value);
	printf("Production of %d %s done, production time: %f sec\n", 
			*sem_value,
			*sem_value > 1 ? "cars" : "car",	       
			production_time);
	printf("==========\n");
	free(sem_value);
}

void initResourcePack(struct resource_pack *pack,
		int space_limit, int num_workers) {
	initSem();
	pack->space_limit  = space_limit;
	pack->num_workers  = num_workers;
	pack->sem_space    = &sem_space   ;
	pack->sem_worker   = &sem_worker  ;

	pack->sem_skeleton = &sem_skeleton;
	pack->sem_engine   = &sem_engine  ;
	pack->sem_chassis  = &sem_chassis ;
	pack->sem_body     = &sem_body    ;

	pack->sem_window   = &sem_window  ;
	pack->sem_tire     = &sem_tire    ;
	pack->sem_battery  = &sem_battery ;
	pack->sem_car      = &sem_car     ;
}

int destroySem(){
#if DEBUG
	printf("Destroying semaphores...\n");
#endif
	sem_destroy(&sem_worker);
	sem_destroy(&sem_space);

	sem_destroy(&sem_skeleton);
	sem_destroy(&sem_engine);
	sem_destroy(&sem_chassis);
	sem_destroy(&sem_body);

	sem_destroy(&sem_window);
	sem_destroy(&sem_tire);
	sem_destroy(&sem_battery);
	sem_destroy(&sem_car);
#if DEBUG
	printf("Semaphores destroyed\n");
#endif
	return 0;
}

int initSem(){
#if DEBUG
	printf("Initiating semaphores...\n");
#endif
	printf("num_worker %d \n", num_workers);
	sem_init(&sem_worker, 0, num_workers);
	sem_init(&sem_space,   0, num_spaces);

	sem_init(&sem_skeleton, 0, 0);
	sem_init(&sem_engine,   0, 0);
	sem_init(&sem_chassis,  0, 0);
	sem_init(&sem_body,     0, 0);

	sem_init(&sem_window,   0, 0);
	sem_init(&sem_tire,     0, 0);
	sem_init(&sem_battery,  0, 0);
	sem_init(&sem_car,      0, 0);
#if DEBUG
	printf("Init semaphores done!\n");
#endif
	return 0;
}

