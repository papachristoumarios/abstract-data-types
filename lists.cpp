#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct interval_t {
	int left, right;
	bool cleft, cright;
} interval;


//type definitions
typedef struct node_t {
	interval intrv;
	int data;
	struct  node_t *next;
	struct  node_t *previous;	
} node;

typedef struct list_t {
	node *head;	
	node *rear;
	unsigned int size;
} list;

//adjacency list
typedef struct adjacency_node_t {
	int data;
	vector<node*> adj;
} adjacency_node; 

typedef struct adjacency_list_t {
	adjacency_node *head;	
} adjacency_list;

//list methods

//constructors
list create() {
	list l;
	l.head = NULL;
	l.rear = NULL;	
	l.size = 0;
	return l;
}

list init(int x) {
	list l;
	node *p = new node;
	p->data = x;
	l.head = p;
	l.rear = p;
	l.size = 1;
	p->next = NULL;
	p->previous = NULL;
	return l;
}

//returns size
unsigned int size(list &l) {
	return l.size;
}

//checks if list is empty
bool isEmpty(list &l) {
	if (l.head == NULL && l.size == 0) return true;
	return false;
}

void add(list &l, int k, int x) {
	node *q = new node;
	q->data = x;
	l.size++;
	if (k == 1) { //case when inserted in front of the list
		q->next = l.head;
		l.head = q;
		q->previous = NULL;
		if (q->next->next == NULL) l.rear = q->next;
		return;
	}
	node *current = l.head;
	for (int i = 0; i < k-2; i++) 	current = current->next;
	q->next = current->next;
	q->previous = current;
	current->next = q;
	if (q->next == NULL) {
		l.rear = q;
	}
}

//returns element at position k 
int get(list &l, int k) {
	node *current = l.head;
	for (int i = 0; i < k-1; i++) current = current -> next;
	return current->data;
}

//returns element at position k (backwards)
int getBakcwards(list &l, int k) {
	return get(l, l.size - k - 1);
}

//deletes element at position k and reconnects the list 
bool del(list &l, int k) {
	if(isEmpty(l)) return false;
	node *tmp = l.head;
	l.size--;
	if (k==1) {
		l.head =  tmp->next;
		delete tmp;
		return true;
	}
	for(int i = 0; i < k-2; i++) tmp = tmp->next;
	node *tmp2 = tmp->next;
	tmp->next = tmp->next->next;
	if (tmp->next == NULL) {
		l.rear = tmp;
	}
	delete tmp2;
	return true;
}

//deletes backwards
bool delBack(list &l, int k) {
	return del(l, l.size - k - 1);
}

//inserts element in front of the list
void insertFront(list &l, int x) {
	add(l,1,x);
}

//prints a connected list (via a node pointer) recursively
void print(node *p) {
	 if (p != NULL) {
		printf("%d ", p->data);
		print(p->next); 
	 }	
}

//prints a connected list (via a node pointer) recursively (reverse)
void printReversed(node *p) {
	 if (p != NULL) {
		print(p->next); 
		printf("%d ", p->data);	
	 }	
}

//prints a list
void printList(list &l) {
	print(l.head);
	printf("\n");
}

//searches for a query key
inline node* search(list &l, int query, int &pos) {
	node *p;
	pos = 1;
	for (p = l.head; p != NULL; p = p->next) {
		if (p->data == query) return p;
		pos++;
	}
	return NULL;
}

//deletes and reconnects a list at a given node in O(1)
bool deleteAfter(node *p) {
	node *q;
	if (p == NULL || p->next == NULL) return false;
	q = p->next;
	p->next = q->next;
	p->next->previous = p; 
	delete q;
	return true;	
}

//searches for x and moves it in front of the list
inline int searchMF(list &l, int x) {
	node *current,*p; int k = 1;
	if (l.head->data == x) return 1;

	for (current = l.head; current != NULL; current = current->next) {
		if (!(current == NULL || current->next == NULL)  && current->next->data == x) {
			p = current->next;
			current->next = p->next;
			p->next = l.head;
			l.head = p;
			return k+1;			 
		}
		k++;
	}
	return 0;
}

void copy(list &l1, list &l2) {
	node *p; int k;
	for (p = l1.head, k = 1; p != NULL; p = p->next, k++) add(l2,k,p->data);	
}

list concatenate(list &l1, list &l2) {
	if (l1.head == NULL) return l2;
	if (l2.head == NULL) return l1;
	
	list l =  init(l1.head->data); 
	int k=1; node *p;
	for (p = l1.head->next, k = 2; p!= NULL; p = p->next, k++) add(l,k,p->data);
 	for (p = l2.head, k = l1.size + 1; p != NULL; p = p->next,k++) add(l,k,p->data);
	return l;
}

void concat_to(list &l1, list &l2) {
	node *p;
	if (l2.head == NULL) return;
	if (l1.head == NULL) l1 = l2;
	else {
		p = l1.head;
		while (p->next != NULL) p = p->next;
		p->next = l2.head;
	}	
}

//reverses a list iteratively
void reverse (list &l) {
	node *p, *q = NULL;
	while (l.head != NULL) {
		p = l.head;
		l.head = p->next;
		p->next = q;
		q = p;	
	}
	l.head = q; return;
}

//destroy a list
void destroy (list &l) {
	node *q, *p;
	for (q = l.head; q != NULL && l.head != NULL; q = q->next) {
		l.head = q->next;
		p = q;
		delete p;
	}
}

//quicksort 
node * partition (node *f, node *l) {
	int x = f->previous->data;
	node *i = f, *j = l;
	
	while (true) {
		while (i->data < x) i = i->next;
		while (j->data > x) j = j->previous;
		if (i->previous == j) break;
		int tmp = i->data;
		i->data = j->data;
		j->data = tmp;
		i = i->next;
		j = j->previous;		
	}
	return j;	
}

void quicksort(node *f, node *l) {
	if (f->previous == l) return;
	node *p = partition(f,l); 
	quicksort(f,p); quicksort(p->next,l);
}

//mergesort
node * get_mid(list &l) {
	node *p, *q = l.head;
	for (p = l.head; p != NULL && p->next != NULL; p = p->next->next) q = q->next;
	return q;
}

list oddeven(int N) {
	list l,m;
	int x;
	l.head = m.head = NULL;
	node *p,*q,*r,*s;
	
	for (int i = 0; i < N; i++) {
		scanf("%d", &x);
		q = new node;
		q->data = x;
		q->next = NULL;
		if (x % 2 == 0) {
			if (!l.head) { r = q; l.head = r;}
			else { r->next = q; r = r->next; }
		} else {
			if (!m.head) { s = q; m.head = s;}
			else {s->next = q; s = s->next;}
		}
	}
	
	s = m.head;
	if (m.head->next) {
		p = NULL;
		
		while (s != NULL) {
			q = s->next;
			s->next = p;
			p = s;
			s = q;
			
		}
		m.head = p;
	}
	r->next = m.head;
	//delete  &m;
	return l;
}


/*
list merge (list &l1, list &l2) {
	node *p = l1.head;
	node *q = l2.head;
	node *r;
	list l;
	if (p -> data < q->data) {
		r = p;
		p = p->next;
	}
	else {
		r = q;
		q = q->next;
	}
	l.head = r;
	l.size = 0;
	
	while (1 == 1) {
		if (p == NULL && q == NULL) break;
		if (p ->data < q->data) {
			r->next = p;	
			p = p->next;
		}
		else {
			r->next = q;
			q = q->next;
		}
		r = r->next;
		l.size++;
	}
	
	return l;
}

void mergesort(list &l) {
	if (l.head == NULL || l.head->next == NULL) return;
	node *mid = get_mid(l);
	list m;
	m.head = mid;
	mid->previous->next = NULL;
	mergesort(l); mergesort(m);
	merge(l,m);
}
*/

void _reverse_recursive (node *c, node *p, list &l) {
	if (!c->next) {
		l.head = c;
		c->next = p;
		return;
	}
	
	node *n = c->next;
	c->next = p;
	_reverse_recursive(n,c,l);
}

void reverse_recursive(list &l) {
	_reverse_recursive(l.head, NULL, l);
}

list minLen(list &l1, list &l2) {
	node *p = l1.head;
	node *q = l2.head;
	while (true) {
		p = p->next;
		q = q->next;
		if (p && !q) return l1;
		if (!p && q) return l2;
		if (!p && !q) return l1;
	}
	return l1;
}

list minLen3(list &l1, list &l2, list &l3) {
	list u = minLen(l1,l2);
	return minLen(u, l3);
}

list sortedIntersect (list &l1, list &l2) {
	list l; l.head = NULL; 
	node *p = l1.head;
	node *q = l2.head;
		
	while (p && q) {
		if (p->data > q->data) q = q->next;
		else if (p->data < q->data) p = p->next;
		else {
			if (!l.head) init(q->data);
			else add(l,l.size,q->data);	
		}
		q = q->next;
		p = p->next;	
	}
	return l;
}

void frontBackSplit(list &l, list &m) { //pass an empty list m as argument
	if (l.size == 1) return;
	if (l.size == 2) {
		m.head = l.head->next;
		m.size++;
		l.head->next = NULL;
		return;
	}
	
	node *p, *q = l.head;
	
	for (p = l.head; p && p->next; p = p->next->next) q = q->next;
	m.head = q->next;
	m.size = size(m);
	q->next = NULL; //delink lists
	
}

int main(void) {
	list l = oddeven(10);
	printList(l);
	printReversed(l.head);
	return 0;
}

//unit testing
void test() {
	list l = init(2);
	add(l, 1,6);
	add(l, 2, 5);
	printList(l);
	printf("%d %d\n", l.size, get(l,2));
	printList(l);
	exit(0);
}


void example1() {
		int K,N,M,x,i; list l;
	
	scanf("%d", &N);


	for (i=0; i < N; i++) {
		scanf("%d %d", &K, &x);
		if (i == 0) {
			l = init(x); continue;
		}
		add(l,K,x);
		
	}
	//printList(l);
	scanf("%d", &M);
	
	for (i=0; i<M; i++) {
		scanf("%d", &K);
		del(l,K);
		//printList(l);
	} 
	scanf("%d", &K);
	printf("%d %d\n", l.size, get(l,K));
	exit(0);
}
