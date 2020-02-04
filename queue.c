#include "queue.h"
#include <semaphore.h>
sem_t sem1;

queue* queueCreate(unsigned capacity){ 
	sem_init(&sem1,0,1); 
	queue* q = malloc(sizeof(queue));
	q->capacity = capacity;
	q->size = 0;
	q->front = 0;
	q->rear = 0;
	q->array = (int*) malloc(q->capacity * sizeof(int));
	return q;
}

void queueDestroy(queue* q){
	free(q -> array);//free array in q
	free(q);//free q
}

int queueIsFull(queue* q){
	return (q->size == q->capacity);
}

int queueIsEmpty(queue* q){
	return (q->size == 0);
}

int queueEnqueue(queue* q, int* value){
	sem_wait(&sem1);
	if (q->size == q->capacity){
		sem_post(&sem1);
		return 0;
	}
	if (q->size == 0){
		q->array[q->rear] = *value;
		q->size++;
	} else{
		q->rear = ((q->rear + 1) % q->capacity);
		q->array[q->rear] = *value;
		q->size++;
	}
	sem_post(&sem1);
	return 1;
}

int queueDequeueFront(queue* queue, int* container){ 
	sem_wait(&sem1);
	if (queueIsEmpty(queue)){
		sem_post(&sem1);	
       return 0;}
    *container = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
	sem_post(&sem1);
    return 1;
}

int queueDequeueRear(queue* queue, int* container){
	sem_wait(&sem1);
	if (queueIsEmpty(queue)){
		sem_post(&sem1);
        return 0;}
    *container = queue->array[queue->rear];
    queue->rear = (queue->rear -1)%queue->capacity;
    queue->size = queue->size - 1;
	sem_post(&sem1);
    return 1;
}

int* queueFront(queue* queue){
	if (queueIsEmpty(queue))
        return NULL;
    return &queue->array[queue->front];
}

int* queueRear(queue* queue){ 
	if (queueIsEmpty(queue))
        return NULL;
    return &queue->array[queue->rear];
}

void queuePrint(queue* q){
	printf("queue(^: front, *: rear): [");
	int i;
	for(i = 0; i < q->capacity; i++){
		printf("%d", q->array[i]);
		if(i == q->front) printf("^");
		if(i == q->rear) printf("*");
		i == q->capacity - 1 ? printf("] ") : printf(", ");
	}
	printf("size=%d, capacity=%d\n", q->size, q->capacity);
}

