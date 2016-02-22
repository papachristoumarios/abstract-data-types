#include <cstdio>
#include <cstdlib>

using namespace std;

struct node_t {
	int data;
	struct node_t *next;	
};

typedef struct node_t node;

struct queue_t {
	node *front,*rear;	
};

typedef struct queue_t queue;

queue create() {
	queue q;
	q.front = NULL;
	q.rear = NULL;	
	return q;
}

void enqueue(queue &q, int x) {
	node *p = new node;	
	p->data = x;
	p->next = NULL;
	if (q.front == NULL) q.front = p;
	else q.rear->next = p;
	q.rear = p;
}

bool dequeue(queue &q, int &x) {
	node *p;
	if (q.front == NULL) {
		return false;
	} else {
		p = q.front;
		x = q.front->data; 
		if (q.front == q.rear) {
			q.rear = NULL;
		}
		q.front = q.front->next;
		delete p;
		return true;
	}	
}

bool isEmpty(queue &q) {
	if (q.front == NULL && q.rear == NULL) {
		return true; 
	}
	return false;
}


int peek(queue &q) {
	return q.front->data;	
}

int main(void) {
	int i,x,cp=0,cn=0;
	queue qp = create(); //queue for positives
	queue qn = create(); //queue for negatives 
	
	while (scanf("%d", &x) == 1) 	 
	{
		if (x >= 0) {
			enqueue(qp,x);
			cp++;
		} else {
			enqueue(qn,x);
			cn++;
		}
	//printf ("%d", x);	
	}	
	
	if (cp != cn) {
		printf("no\n");
		return 0;
	}		
	
	int x1=0,x2=0; //data to be returned by dequeue
	
	for (i=0; i < cp; i++) {
		dequeue(qp, x1);
		dequeue(qn, x2);
		if (x1 != -x2) {
			printf("no\n");
			return 0;	
		}			
	}
	
	printf("yes\n");
	return 0;
}
