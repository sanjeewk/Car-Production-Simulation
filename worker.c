#include "worker.h"

void* workerThreadFuc(void *arg){ 
	// Extract work_pack

	work_pack *wpack = (work_pack *)arg;
	int tid = wpack->tid;
	queue *jobQ = wpack->jobQ;
	// sem_wait(wpack->resource->sem_worker);
	int jobID;
	//TODO finish workerThreadFuc
	if (queueDequeueFront(jobQ, &jobID) == 1)
	{
		work(jobID, wpack->resource ,tid);
	}
	pthread_exit(NULL);
}

void work(int jobID, resource_pack *pack, int tid) {
		// sem_wait(pack->sem_worker);
	switch(jobID) {
		case SKELETON  :
			makeSkeleton(pack->sem_space, pack->sem_skeleton);
#if DEBUG	
			printf("Worker[%d]: One car skeleton made!\n", jobID);
			fflush(stdout);
#endif
			break;
		case ENGINE    :
			makeEngine(pack->sem_space, pack->sem_engine);
#if DEBUG	
			printf("Worker[%d]: One engine made!\n", tid);
			fflush(stdout);
#endif
			break;
		case CHASSIS   :
			
			makeChassis(pack->sem_space, pack->sem_chassis);
#if DEBUG	
			printf("Worker[%d]: One chassis made!\n", tid);
			fflush(stdout);
#endif
			break;
		case BODY      :
			makeBody(pack->sem_space, pack->space_limit, 
					pack->sem_body,
					pack->sem_skeleton, 
					pack->sem_engine, 
					pack->sem_chassis);
#if DEBUG	
			printf("Worker[%d]: One body made!\n", tid);
			fflush(stdout);
#endif
			break;
		case WINDOW    :
			makeWindow(pack->sem_space, pack->sem_window);
#if DEBUG	
			printf("Worker[%d]: One window made!\n", tid);
			fflush(stdout);
#endif
			break;
		case TIRE      :
			makeTire(pack->sem_space, pack->sem_tire);
#if DEBUG	
			printf("Worker[%d]: One tire made!\n", tid);
			fflush(stdout);
#endif
			break;
		case BATTERY   :
			makeBattery(pack->sem_space, pack->sem_battery );
#if DEBUG	
			printf("Worker[%d]: One battery pack made!\n", tid);
			fflush(stdout);
#endif
			break;
		case CAR       :
			makeCar(pack->sem_space, pack->space_limit, 
					pack->sem_car,
					pack->sem_window, 
					pack->sem_tire, 
					pack->sem_battery, 
					pack->sem_body);
#if DEBUG	
			printf("Worker[%d]: One car made!\n", tid);
			fflush(stdout);
#endif
			break;
		default:
			printf("Error!! Worker[%d]: No such kind of job\n", tid);
			fflush(stdout);
			break;
	}
#if DEBUG
	printf("Worker[%d]: job %d done!\n", tid, jobID);
	fflush(stdout);
#endif
reportJobDone(pack->sem_worker, pack->num_workers);
}
