#include <cstdio>
#include <algorithm>
#include <functional>
#include <vector>
#include <iterator>
using namespace std;

template <class T> class Tree {
	protected:
		T data;
		vector<Tree<T>*> subtrees;
	public:
		Tree();
		virtual T key() { return data; }
		virtual void set_key(T k) { data = k; }
		virtual const vector<Tree<T>*> Subtrees() { return subtrees; }	
		virtual ~Tree() { 
			for (Tree* t: subtrees) delete t;
		}
		virtual bool IsEmpty() { return this == NULL; }
		
		virtual void SwapKeys(Tree& other) { 
			T x = key();
			set_key(other.key());
			other.set_key(x);	
		}
		virtual bool IsLeaf() {
			return subtrees.size() == 0;
		}
};

template <class T, class Q> class TreeIterator {
	protected:
		Tree<T>& t;
		function < Q ( Tree<T>& ) >  visit;
		unsigned int subtree_index;
	public:
		TreeIterator(Tree<T>& _t) : t(_t), subtree_index(0) {}
		virtual function < Q (Tree<T>& ) > get_visit_function() const { return visit; }
		virtual void set_visit_function (const function < Q (Tree<T>& ) >& _visit) { visit = _visit; }
		virtual void traverse(Q&) = 0;
		virtual Tree<T>* operator[] (int i) {return t.subtrees[i]; }
		virtual Tree<T>* operator++ () { return t.subtrees[subtree_index++]; }
		virtual Tree<T>* operator-- () { return t.subtrees[subtree_index--]; }
		virtual T& operator* () { return *t.subtrees[subtree_index]; }
		friend class Tree<T>;
};

template <class T, class Q> class PreOrderIterator : public TreeIterator<T,Q> {
	public:
		PreOrderIterator(Tree<T> _t) : TreeIterator<T,Q>(_t) {}
		void traverse(Q& result) {
			if (TreeIterator<T,Q>::t == NULL) return;
			result = TreeIterator<T,Q>::visit(TreeIterator<T,Q>::t);
			for (Tree<T>* s : TreeIterator<T,Q>::t.subtrees) TreeIterator<T,Q>::visit(*s);
		}
};

template <class T, class Q> class PostOrderIterator : public TreeIterator<T,Q> {
	public:
		PostOrderIterator(Tree<T> _t) : TreeIterator<T,Q>(_t) {}
		void traverse(Q& result) {
			if (TreeIterator<T,Q>::t == NULL) return;
			for (Tree<T>* s : TreeIterator<T,Q>::t.subtrees) TreeIterator<T,Q>::visit(*s);
			result = TreeIterator<T,Q>::visit(TreeIterator<T,Q>::t);
		}
};

template <class T> class BST : public virtual Tree<T> {
	protected:
		T data;
	public:
		BST *left, *right;
		BST(T x) {
			data = x;
			left = right = NULL;
			Tree<T>::subtrees = *new vector<Tree<T>*> (2);
			Tree<T>::subtrees[0] = left, Tree<T>::subtrees[1] = right;
		} 
		virtual bool IsLeaf() {
			return left == NULL && right == NULL;
		}
		virtual int Height() {
			if (!this) return -1;
			else return 1 + max(left->Height(), right->Height()); 
		}
		BST* search (T q) {
			if (!this) return NULL;
			if (q == this->data) return this;
			else if (q > this->data) return right->search(q);
			else if (q < this->data) return left->search(q);	
		}
		BST& insert (BST<T> *q) {
			if (!this) { *this = *q; return *this; }
			if (q->data > data) right->insert(q);
			else left->insert(q);
		}
		void swap_values (BST& other) {
			T tmp = data;
			data = other.data;
			other.data = tmp;
		} 
		
		BST* delete_node(T q) {
			BST *result = search(q);
			if (result == NULL) return this;
			else if (q < this->data) left->delete_node(q);
			else if (q > this->data) right->delete_node(q);
			else {
				if (IsLeaf()) {
					delete this;
					return NULL;
				}
				else if (left == NULL) {
					BST* tmp = this;
					this = this->right;
					delete tmp;
					return this;
				}
				else if (right == NULL) {
					BST* tmp = this;
					this = this->left;
					delete tmp;
					return this;
				}
				else {
					BST *tmp = right->find_min();
					this->data = tmp->data;
					this->right = right->delete_node(q);	
				}					
			}
			return this;
		}
		
		int children() {
			return left == NULL ? 0 : 1 + right == NULL ? 0 : 1;
		}
		T key() { return data; }
		void set_key(T k) { data = k; }
		bool IsEmpty() { return this == NULL; }
		virtual BST<T>& Left() { return *left; }
		virtual BST<T>& Right() { return *right; }
};

template <class T> class AVLTree : public BST<T> 
{
	protected:
		virtual AVLTree& Left() { return *left; }
		virtual AVLTree& Right() { return *right; }
		int height, b;
		int BalanceFactor() {
			if (BST<T>::Tree::IsEmpty()) return 0;
			else return left->Height() - right->Height();
		}
		bool IsAVLBalanced() {
			return BalanceFactor() >= -b && BalanceFactor() <= b;	
		}
		
		void AdjustHeight() {
			if (BST<T>::Tree::IsEmpty()) height = -1;
			else height = max (left->height, right->height) + 1;
			
		}
		
		void LLR() { //left left rotation
			if (BST<T>::Tree::IsEmpty()) return;
			AVLTree<T>* const tmp = right; 
			right = left;
			left = Right().left;
			Right().left = Right().right;
			Right().right = tmp;
			Tree<T>::SwapKeys(Right());
			Right().AdjustHeight();
			AdjustHeight();
		}
		void LRR() {
			if (BST<T>::Tree::IsEmpty()) return;
			Left().RRR();
			LLR();
		}
		void RRR() {
			if (BST<T>::Tree::IsEmpty()) return;
			AVLTree* const tmp = right; 
			left = right;
			right = Left().right;
			Left().right = Left().left;
			Left().left = tmp;
			Tree<T>::SwapKeys(Left());
			Left().AdjustHeight();
			AdjustHeight();
		}
		void RLR() {
			if (BST<T>::Tree::IsEmpty()) return;
			Right().LLR();
			RRR();
			
		}
		void Balance() {
			AdjustHeight();
			if (abs (BalanceFactor()) > 1) {
				if (BalanceFactor() > 0) {
					if (Left().BalanceFactor() > 0) LLR();
					else LRR();
				}
				else {
					if (Right().BalanceFactor() > 0) RRR();
					else RLR();
				}
			}
		}
	public:
		AVLTree(T x, int _b) : BST<T>(x), height(-1), b(_b) {}
		int Height()  const { return height; };
		AVLTree<T> *left, *right;	
};

template <class T> unsigned int bsearch(const vector<T*>& V, const T& q) {
	unsigned int left = 1, right = V.size();
	if (right == 0) return 0;
	while (left < right) {
		unsigned int middle = (left + right + 1) / 2;
		if (q >= *V[middle]) left = middle;
		else right = middle - 1U;
	}
	return left;
}

template <class T> class MWayTree {
	protected:
		vector<T*> data;
		vector<MWayTree*> subtrees;
		int m, number_of_keys;
	public:
		MWayTree(int M) {
			subtrees = *new vector<MWayTree*>(M);
			data = *new vector<T*>(M-1);
		}
		MWayTree(int M, const vector<T*>& d) {
			subtrees = new vector<MWayTree*>(M);
		} 
		
		MWayTree& split_half() { //modifies the tree itself by splitting it to two
			int middle = m / 2 + 1;
			MWayTree *tmp = new MWayTree(2); // add M? instead of 2 ?
			tmp->data[0] = this->data[middle];
			tmp->subtrees[0] = new MWayTree(m);
			tmp->subtrees[1] = new MWayTree(m);
			for (int i = 0; i < middle; i++) tmp->subtrees[0]->data[i] = data[i];
			for (int i = 0; i < middle; i++) tmp->subtrees[1]->data[i] = data[middle + i + 1];
			*this = *tmp;
			return *this;
		}
		
		MWayTree& merge_with (MWayTree& other) {
			int index = bsearch(data,other->data[0]);
			data.insert(data.begin() + index, other->data[0]);
			subtrees.insert(subtrees().begin + index, other->subtrees[0]);
			subtrees.insert(subtrees().begin + index + 1, other->subtrees[1]);
	
			if (number_of_keys + 1 <= m - 1) {
				number_of_keys++; return *this;
			}
			else if (number_of_keys > m - 1) {
				split_half(); return *this;
			}
			
		}
	
		bool IsEmpty() { return !this; }
		MWayTree& get_subtree (int i) { return *subtrees[i]; }
		T key (int pos) { return data[pos]; }
		T* find (const T& q, MWayTree* sbtr) {
			if (IsEmpty()) return NULL;
			unsigned int i = data.size();
			while (i > 0) {
				if (*data[i] == q) {
					sbtr = subtrees[i];
					return data[i];
			}
				if (q < *data[i]) return subtrees[i]->find(q, sbtr);
				i--; 
			}
			return subtrees[i]->find(q, sbtr);
	}
	T* binary_find (const T& q, MWayTree* sbtr) {
		if (IsEmpty()) return NULL;
		unsigned int index_of_query = bsearch(data, q);
		if (*data[index_of_query] == q) return data[index_of_query];
		else return subtrees[index_of_query]->binary_find(q, sbtr);
		
	}
	MWayTree& insert(T* t) {
		if (IsEmpty()) {
			data[1] = t;
			subtrees[0] = subtrees[1] = new MWayTree(m);
			number_of_keys = 1;
			return *this;
		}
		unsigned int i = bsearch(data, *t);
		if ( i != 0 && t == data[i]) return *this;
		if (number_of_keys < m - 1U) {
			for (unsigned int j = number_of_keys; j > 0; j--) {
				data[j+1] = data[j];
				subtrees[j+1] = subtrees[j];
			}
			data [i + 1] = t;
			subtrees[i + 1] = new MWayTree(m);
			number_of_keys++;		
		}	
	}	
};


int main(void) {
	return 0;
}
