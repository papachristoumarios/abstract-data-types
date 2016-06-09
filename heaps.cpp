#include <cstdio>
#include <functional>
#include <vector>
#include <exception>
#include <math.h>

using namespace std;

template <class T> inline T min(T A, T B) {
	return A <= B ? A : B;
} 

template <class T> inline T max(T A, T B) {
	return A >= B ? A : B;
}

template <class T> class BaseHeap {
	protected:
		vector<T*> A;
		void swap(int i, int j) {
			T* tmp = A[i];
			A[i] = A[j];
			A[j] = tmp;
		}
		
	public:
		BaseHeap();
		BaseHeap(vector<T*> X) { A = X; }
		virtual ~BaseHeap() {
			for (T* x : A) {
				T* tmp = x;
				x = NULL;
				delete tmp;
			}
		}
		int depth(int i) { return (int)log2((double)i); }
		T* pop_last() {
			T* tmp = A[size() - 1];
			A.pop_back();
			return tmp;
		}
		T* key(unsigned int i) {
			try {
				return A[i];
			} catch (exception &e) {
				return NULL;
			}
		}
		void set_key(unsigned int i,T* k) {
			try {
				A[i] = k;
			} catch (exception &e) {
				A.push_back(k);
			}
				
		}
		int size() { return (int)A.size(); }
		int parent(int i) { return i/2; }
		int left(int i) { 
			if (2*i < size()-1) return 2*i;
			else return -1;
			}
		int right(int i) { 
			if (2*i+1 < size()-1) return 2*i+1;
			else return -1;
			}
		int max_child(int i) {
			if (*left_key(i) >= *right_key(i)) return left(i);
			else return right(i);
		}	
			
		T* left_key(int i) { return key(left(i)); }	
		T* right_key(int i) { return key(right(i)); }
		int null_path(int i) {
			if (is_leaf(i)) return 0;
			else return 1 + min<int>(null_path(left(i)), null_path(right(i)));
		}	
		bool is_leaf(int i) {
			return left(i) > size() && right(i) > size();
		}
		bool is_internal_node(int i) { return !is_leaf(i); }
		T* operator[] (int i) { return A[i]; }
};

template <class T> class PropertyHeap : public BaseHeap<T> {
	protected:
		function<bool(T*,T*,T*)> trenary_opertator;
	public:
		PropertyHeap(function<bool(T*,T*,T*)> f) : BaseHeap<T>() { //operator key,left_key,right_key -> {0,1}
			trenary_opertator = f;
		}
		PropertyHeap(vector<T*> X, function<bool(T*,T*,T*)> f) : BaseHeap<T>(X) { //operator key,left_key,right_key -> {0,1}
			trenary_opertator = f;
		}
		virtual bool satisfies_property(int i) {
			return trenary_opertator(BaseHeap<T>::key(i), BaseHeap<T>::left_key(i), BaseHeap<T>::right_key(i));
		}
		virtual bool satisfies_property_recursively(int i) {
			if (!satisfies_property(i)) return false;
			else return satisfies_property_recursively(BaseHeap<T>::left(i)) && satisfies_property_recursively(BaseHeap<T>::right(i));
		} 
		virtual bool heap_satisfies_property() { return satisfies_property_recursively(0); }
}; 

template <class T> class MaxHeap: public PropertyHeap<T> {
	private:
		bool comp(T* p, T* l, T* r) {
			return (*p >= *l) && (*p >= *r); 
		}
	public:
		MaxHeap(vector<T*> X) : PropertyHeap<T>(X, this->comp) {
			MaxHeap<T>::max_heapify(this, 0);
		}
		MaxHeap() : PropertyHeap<T>(this->comp) {}	
		T* max() { return BaseHeap<T>::A[0]; }
		void sift_up (int i) {
			if (i > BaseHeap<T>::size() - 1) return;
			if (i == 0) return;
			else if (BaseHeap<T>::A[i] >= BaseHeap<T>::A[i/2]) {
				BaseHeap<T>::swap(i, i/2);
				sift_up(i/2);
			}
		}
		void insert (T* item) {
			BaseHeap<T>::A.push_back(item);
			sift_up(BaseHeap<T>::size() - 1);
		}
		void combine(int i) {
			if (i == BaseHeap<T>::size() - 1) return;
			else if (PropertyHeap<T>::satisfies_property(i) == false) {
				int j = BaseHeap<T>::max_child(i);
				swap(i, j);
				combine(j);
			}
		}
		
		static MaxHeap* build_max_heap(vector<T*>& v) {
			BaseHeap<T> h = *new BaseHeap<T>(v);
			for (int i = (int)v.size() / 2; i>=0; --i) {
				MaxHeap<T>::max_heapify(h,i);
			}
			return (MaxHeap*)(&h);
		} 
		
		static void max_heapify(BaseHeap<T>& H, int i) {
			int l = H.left(i); int r = H.right(i);
			int largest;
			if (l <= H.size()-1 && H.left_key(i) >= H.key(i)) {
				largest = l;
			}
			else largest = i;
			if (r <= H.size() -1 && H.right_key(i) > H.key(largest)) {
				largest = r;
			}
			if (largest != i) {
				H.swap(i, largest);
				MaxHeap<T>::max_heapify(H, largest);
			}
		}
		static vector<T*> heapsort(BaseHeap<T>& H) {
			vector<T*> v;
			int n = H.size();
			for (int i = n-1; i >=0 ; i--) {
				H.swap(0,i);
				v.push_back(H.pop_last());
				MaxHeap<T>::max_heapify(H, i);
			}
			return v;
		} 
		
};

template <class T> class MinHeap : public PropertyHeap<T> {
	protected:
		vector<T*> A;
	private:
		bool comp(T* p, T* l, T* r) {
			return (*p <= *l) && (*p <= *r);
		}
	public:
		MinHeap(vector<T*> X) : PropertyHeap<T>(X, this->comp) {}
		MinHeap() : PropertyHeap<T>(this->comp) {}
		T* min() { return A[0]; }
};

template <class T> class LeftistHeap : public MinHeap<T>, virtual PropertyHeap<T> {
	
};

template <class T> class RightistHeap : public MinHeap<T>, virtual PropertyHeap<T> {
	
};


int main(void) {	
	return 0;
}
