/*
 * Description: A small library that handles some types of trees (BST, AVL, splay) written in C++
 * Author: Marios Papachristou (papachristoumarios@gmail.com)
 * Year: 2015
 * License: The content is licensed under the MIT License 
 * Note: This library also contains some functions that were proposed by Stanford CS Education Library 
 */ 

#include <cstdio>
#include <algorithm>
#include <climits>
#include <cmath>
#include "tree.h"

using namespace std;

//for red black trees
enum colour_t {RED, BLACK};
typedef enum colour_t colour;

/*
 * Basic struct definitions for binary trees:
 * A tree node contains integer data, left and right children and a parent
 * Refer to them as t->left, t->right, t->parent
 * A binary tree contains a root node pointer
 */ 

typedef struct interval_t {
	int left, right;
	bool cleft, cright;
} interval;


typedef struct node_t {
	int data; node_t *left, *right, *parent;
	interval intrv;
	colour color;
} node;

typedef struct tree_t {
	node *root;
	unsigned int height;
} tree;

/*
 * Initializes a binary tree with a value and returns it
 */
tree init (int x) {
	node *p = new node;
	p->data = x;  
	p->left = p->right = NULL;
	tree t;
	t.root = p;
	return t;	
}

/*
 * Checks if a binary tree is empty, namely if its root node is NULL
 */ 
bool empty(tree &t) {
	return t.root == NULL;
}

/*
 * Inserts a node to a binary search tree (BST)
 */ 
void _insert(node *t, node *item) {
	if(!t) { t = item; return;}
	if(item->data < t->data) _insert(t->left, item);
	else if(item->data > t->data) _insert(t->right, item);	
}

void insert_to_bt(tree &t, int x) {
	node *p = new node;
	p->data = x;
	p->left = p->right = NULL;
	_insert(t.root, p);
}

/*
 * Preorder, Inorder, Postorder and Threading Traversals 
 * TODO add general visit functions
 */
void preorder(node *t) {
	if (t == NULL) return;
	printf("%d", t->data);
	preorder(t->left);
	preorder(t->right);	
}

void preorder_bt(tree &t) {
	preorder(t.root);
}

void inorder(node *t) {
	if (t == NULL) return;
	inorder(t->left);
	printf("%d", t->data);
	inorder(t->right);
}

void postorder(node *t) {
	if (t == NULL) return;
	postorder(t->left);
	postorder(t->right);
	printf("%d", t->data);
}

int height (node *t) {
	if (t == NULL) return -1;
	return 1 + max(height(t->left), height(t->right));
}

int height_bt(tree &t) {
	return height(t.root);
}

node* search(node *t, int x) {
	if (t->data == x || t == NULL) return t;
	if (t->data < x) search(t->left,x);
	else search(t->right, x);
	return NULL;
} 

node* get_min(node *t) {
	node *current = t;
	while (current->left) current = current->left;
	return current;	
}

node* get_min_bt(tree &t) {return get_min(t.root);}

void exchange_children(node *parent) {
	node *l = parent->left;
	parent->left = parent->right;
	parent->right = l;
}

void reverse(node *t) {
	exchange_children(t);
	if (t->left != NULL) reverse(t->left);
	if (t->right != NULL) reverse(t->right);	
}

void reverse_bt(tree &t) {reverse(t.root);}

node* search_bt(tree &t, int x) {
	return search(t.root, x);
}

bool is_bst(node *t, int minKey, int maxKey) {
	if (t == NULL) return true;
	if (t->data < minKey || t->data > maxKey) return false;
	return is_bst(t->left, minKey, t->data) && is_bst(t->right, t->data + 1, maxKey);	
}

bool is_bst_bt(tree &t) {
	return is_bst(t.root, INT_MIN, INT_MAX);
}

int get_larger_value_less_than(node *t, int x) {
	int val = x;
	while (t) {
		if (t->data >= x) t = t->left;	
		else {
			val = t->data;
			t = t->right;
		}
	}
	return val;	
}

int get_min_value_larger_than(node *t, int x) {
	int val = x;
	while (t) {
		if (t->data >= x) t = t->right;	
		else {
			val = t->data;
			t = t->left;
		}
	}
	return val;	
}

int get_min_val_larger_than_bt(tree &t, int x) {
	return get_min_value_larger_than(t.root, x);
}

int get_sum (node *t) {
	if (t==NULL) return 0;
	return t->data + get_sum(t->left) + get_sum(t->right);
}

int get_sum_bt(tree &t) {
	return get_sum(t.root);
}

bool has_path_sum (node *t, int S) {
	if (t == NULL) return (S == 0);
	return has_path_sum(t->left, S - t->data) || has_path_sum(t->right, S - t->data);
}

int get_num_of_nodes(node *t) {
	if (t == NULL) return 0;
	return 1 + get_num_of_nodes(t->left) + get_num_of_nodes(t->right);	
}

int get_num_of_nodes_bt(tree &t) {return get_num_of_nodes(t.root);}

bool _is_semibalanced(node *t) {
	if (t == NULL) return true;
	int L = get_num_of_nodes(t->left);
	int R = get_num_of_nodes(t->right);
	return L / 2 <= R && R <= 2*L; 	
}

bool is_semibalanced(node *t) {
	if (t == NULL) return true;
	return _is_semibalanced(t->left) && _is_semibalanced(t->right);
}

//uses postorder traversal 
int is_semibalanced2(node *t) {
	int L = 0, R = 0;
	
	if (t == NULL) return 0;
	L = is_semibalanced2(t->left);
	if (t->left != NULL) L++; //add left leaf
	R = is_semibalanced2(t->right);
	if (t->right != NULL) R++; //add right leaf
	
	if (L / 2 <= R && R <= 2*L) return L+R;
	else return 0;	
}

int max_height_difference_between_subtrees(node *t) {
	if (t == NULL) return 0;
	int d = abs(height(t->left) - height(t->right));
	return max(d, max(max_height_difference_between_subtrees(t->left), max_height_difference_between_subtrees(t->right)));	
}

void print_paths(node *t, int path[], int pathlen) {
	if (t == NULL) return;
	
	path[pathlen] = t->data;
	pathlen++;
	
	if (t->left == NULL && t->right == NULL) { //it's a leaf
		for (int i = 0; i < pathlen; i++) 	printf("%d ", path[i]);
		printf("\n");
	}
	else {
		print_paths(t->left, path, pathlen);
		print_paths(t->right, path, pathlen);
		
	}	
}

void print_paths_bt (tree &t) {
	int path[100];
	print_paths(t.root, path, 0); 
}

//splay trees

void left_rotate(tree &t, node *x) {
	node *y = x->right;
	if (y) {
		x->right = y->left;
		if (y->left) y->left->parent = x;
		y->parent = x->parent;
	}
	
	if (!x->parent) t.root = y;
	else if ( x == x->parent->left) x->parent->left = y;
	else x->parent->right = y;
	if (y) y->left = x;
	x->parent = y;	  
}

void right_rotate(tree &t,node *x) {
	node *y = x->left;
	if (y) {
		x->left = y->right;
		if (y->right) y->right->parent = x;
		y->parent = x->parent;
	}
	
	if (!x->parent) t.root= y;
	else if ( x == x->parent->left) x->parent->left = y;
	else x->parent->right = y;
	if (y) y->right = x;
	x->parent = y;	  
}

void splay(tree &t, node *x) {
	while (x->parent) {
		if (!x->parent->parent) {
			if (x->parent->left  == x) right_rotate (t,x->parent);
			else left_rotate(t,x->parent);
		}
		else if (x->parent->left == x && x->parent->right == x->parent) {
			left_rotate(t,x->parent->parent);
			left_rotate(t,x->parent);
			
		}
		else if (x->parent->left == x && x->parent->parent->right == x->parent) {
			right_rotate(t,x->parent);
			left_rotate(t,x->parent);
		}
		else {
			left_rotate(t,x->parent);
			right_rotate(t,x->parent);
		}	
	}	
}		

bool oddEvenPath(node *t, int x) {
	if (t  == NULL) return true;
	if (t->data % 2 == x % 2) return oddEvenPath( t->left ? t->left : NULL, ++x ) || oddEvenPath( t->right ? t->right : NULL, ++x);
	else return false;
}

bool oddEvenPath_bt(tree &t) {
	return oddEvenPath(t.root, 1);
}
		
void replace(tree &t, node *p, node *q) {
	if (!p->parent) t.root = q;
	else if (p == p->parent->left) p->parent->left = q;
	else p->parent->right = q;
	if (q) q->parent = p->parent;
}

//balanced trees
bool is_balanced(node *t) {
	return abs(height(t->left) - height(t->right)) <= 1 ? true : false;	
}

//build123
tree build123() {
	tree t = init(1);
	insert_to_bt(t, 2);
	insert_to_bt(t, 3);
	return t;
}

bool compare(node *t1, node *t2) {
	if (!t1 && !t2) return true;	
	if (t1 && t2) return t1->data == t2->data && compare(t1->left, t2->left) && compare(t1->right, t2->right);
	else return false;
}

bool compare_bt(tree &t1, tree &t2) {
	return compare(t1.root, t2.root);
}

void double_tree (node *t) {
	node *p;
	if (t == NULL) return;
	double_tree(t->left);
	double_tree(t->right);
	
	//add new node and copy to left 
	p = t->left;
	node *q;
	q = new node;
	q->data = t->data;
	t->left = q;
	t->left->left = p;
}

//Range minimum query for segment trees (RMQ)
//some interval functions

void order (interval &I, interval &J) {
	if (I.right >= J.right) {
		interval tmp = I;
		I = J;
		J = tmp;
	}
}

bool intersect (interval I, interval J) {
	int a = min(I.left, J.left);
	int b = max(I.right, J.right);
	return a < b;
}

bool totaloverlap (interval I, interval J) {
	return (I.left <= J.left && J.right >= I.right) || (I.left >= J.left && J.right <= I.right);
}

int rangeMinimumQuery(node *t, interval x) {

	if (intersect(t->intrv, x) == false) return INT_MAX; //something silly
	if (totaloverlap(t->intrv, x)) return t->data; //holds minimum
	
	int pL = rangeMinimumQuery(t->left, x);
	int pR = rangeMinimumQuery(t->right, x);
	
	return min (pL, pR);
}




int main(void) {
	return 0;
}
