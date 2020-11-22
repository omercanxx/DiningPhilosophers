#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

/* GLOBAL DEĞİŞKENLER */
int philosophersNumber;
int probabilityDouble;


struct linked_list
{
    int number;
    int count;
    struct linked_list *next;
};

typedef struct linked_list node;
node *head = NULL, *last = NULL;

typedef struct{
    bool isDouble;
    pthread_mutex_t chop;
}foo;


void *philosopher(void *);
void pickUp(int);
void eat(int);
void putDown(int);


//void print_linked_list();
void insert(int value);
void delete_item(int value);
bool search_item(int value);
void increase(int id);

foo *chopsticks;
pthread_t *philosophers;
pthread_attr_t *attributes;
pthread_t pthread_self(void);

int main(int argc, const char *argv[]) {
	philosophersNumber = atoi(argv[1]);
	probabilityDouble = atoi(argv[2]);
	chopsticks =  malloc( philosophersNumber * sizeof ( chopsticks[0]));
	philosophers =  malloc( philosophersNumber * sizeof ( philosophers[0]));
	attributes =  malloc( philosophersNumber * sizeof ( attributes[0]));
	pthread_t threads[philosophersNumber];
	
	int prob = 100 / probabilityDouble;
	bool flag = false;
	int i;
	srand(time(NULL));
	if(rand() % prob == 0)
	{
		flag = true;
	}
	for (i = 0; i < philosophersNumber; ++i) {
		pthread_mutex_init(&chopsticks[i].chop, NULL);
		if( flag == true)
		{
			chopsticks[i].isDouble = true;
		}

	}

	for (i = 0; i < philosophersNumber; ++i) {
		pthread_attr_init(&attributes[i]);
	}
	
	for (i = 0; i < philosophersNumber; ++i) {
 
		pthread_create(&philosophers[i], &attributes[i], philosopher, &i);
	}

	for (i = 0; i < philosophersNumber; ++i) {
		pthread_join(philosophers[i], NULL);
	}
	return 0;
}

void *philosopher(void *arg) {
	int philosopherId = pthread_self();
	insert(philosopherId);
	while (1) {
		// Yiyen bir daha yiyemesin diye kontrol yapılabilir.
		/*if(search_item(philosopherId) ==false)
		{
			break;
		}*/
		pickUp(philosopherId);
		eat(philosopherId);
		putDown(philosopherId);
	}
}

void pickUp(int philosopherNumber) {
	int right = (philosopherNumber + 1) % philosophersNumber;
	int left = (philosopherNumber + philosophersNumber) % philosophersNumber;

	if (philosopherNumber & 1) {
		printf("Philosopher %d is waiting to pick up chopstick %d\n", philosopherNumber, right);
		if(!chopsticks[right].isDouble)
		{
			pthread_mutex_lock(&chopsticks[right].chop);
		}
		printf("Philosopher %d picked up chopstick %d\n", philosopherNumber, right);
		printf("Philosopher %d is waiting to pick up chopstick %d\n", philosopherNumber, left);
		if(!chopsticks[left].isDouble)
		{
			pthread_mutex_lock(&chopsticks[left].chop);
		}
		printf("Philosopher %d picked up chopstick %d\n", philosopherNumber, left);
	}
	else {
		printf("Philosopher %d is waiting to pick up chopstick %d\n", philosopherNumber, left);
		if(!chopsticks[left].isDouble)
		{
			pthread_mutex_lock(&chopsticks[left].chop);
		}
		printf("Philosopher %d picked up chopstick %d\n", philosopherNumber, left);
		printf("Philosopher %d is waiting to pick up chopstick %d\n", philosopherNumber, right);
		if(!chopsticks[right].isDouble)
		{
			pthread_mutex_lock(&chopsticks[right].chop);
		}
		printf("Philosopher %d picked up chopstick %d\n", philosopherNumber, right);
	}
}

void eat(int philosopherNumber) {
	int eatTime = rand() % 3 + 1;
	printf("Philosopher %d will eat for %d seconds\n", philosopherNumber, eatTime);
	sleep(eatTime);
	printf("Philosopher %d ate\n", philosopherNumber);
	increase(philosopherNumber);
}

void putDown(int philosopherNumber) {
	
	printf("Philosopher %d will will put down her chopsticks\n", philosopherNumber);
	if(!chopsticks[(philosopherNumber + 1) % philosophersNumber].isDouble)
	{
		pthread_mutex_unlock(&chopsticks[(philosopherNumber + 1) % philosophersNumber].chop);
	}
	if(!chopsticks[(philosopherNumber + philosophersNumber) % philosophersNumber].isDouble)
	{
		pthread_mutex_unlock(&chopsticks[(philosopherNumber + philosophersNumber) % philosophersNumber].chop);
	}
	// Yemek yemiş filozofun kontrolü yapılması için yedikten sonra linked list üzerinden delete işlemi yapılabilir.
	/delete_item(philosopherNumber);/
//	print_linked_list();
	
}



void insert(int value)
{
    node *temp_node;
    temp_node = (node *) malloc(sizeof(node));

    temp_node->number=value;
    temp_node->count = 0;
    temp_node->next=NULL;

    //For the 1st element
    if(head==NULL)
    {
        head=temp_node;
        last=temp_node;
    }
    else
    {
        last->next=temp_node;
        last=temp_node;
    }
}

void delete_item(int value)
{
    node *myNode = head, *previous=NULL;

    while(myNode!=NULL)
    {
        if(myNode->number==value)
        {
            if(previous==NULL)
                head = myNode->next;
            else
                previous->next = myNode->next;

            free(myNode); 
            break;
        }

        previous = myNode;
        myNode = myNode->next;
    }
}
bool search_item(int value)
{
    node *searchNode = head;
    int flag = 0;

    while(searchNode!=NULL)
    {
        if(searchNode->number==value)
        {
            flag = 1;
            break;
        }
        else
            searchNode = searchNode->next;
    }

    if(flag==0)
    {
	return false;
    }
    return true;
}
void increase(int value)
{
    node *searchNode = head;
    int flag = 0;

    while(searchNode!=NULL)
    {
        if(searchNode->number==value)
        {
	    searchNode->count++;
	    printf("%d ate %d times\n" ,searchNode->number ,searchNode->count);
            flag = 1;
            break;
        }
        else
            searchNode = searchNode->next;
    }

    if(flag==0)
    {
	printf("ERROR INCREASE");
    }
}

/*void print_linked_list()
{
    printf("\nYour full linked list is\n");

    node *myList;
    myList = head;

    while(myList!=NULL)
    {
        printf("%d ", myList->number);

        myList = myList->next;
    }
    puts("");
}*/