#include <functional>
#include <cstdio>
#include <math.h>
#include <vector>

using namespace std;

template <class T> class HashFunction : public function<unsigned int(T)> {
	public:
		HashFunction();
		virtual unsigned int operator () (T t) {return 0;}
};

class ModuloHashFunction : public HashFunction<unsigned int> {
	int M;
	public:
		ModuloHashFunction(int m) : HashFunction() {M = m;}
		unsigned int operator () (unsigned int t) { return t % M;}
};

class AverageSquareHashFunction : public HashFunction<unsigned int> {
	unsigned int k, w;
	public:
		AverageSquareHashFunction(unsigned int K) : HashFunction() {k = K; w = 8*sizeof(unsigned int);}
		unsigned int operator () (unsigned int t) { return (t*t) >> (w -k); }
};

class MultiplicativeHashFunction : public HashFunction<unsigned int> {
	unsigned int k, w, a;
	public:
		MultiplicativeHashFunction(unsigned int K, unsigned int A) : HashFunction() {a = A, k = K; w = 8*sizeof(unsigned int);}
		unsigned int operator () (unsigned int t) { return (a*t) >> (w -k); }
};

template <class T> class HashTable {
	int N;
	vector<vector<T*>> V;
	HashFunction<T>& h;
	public:
		HashTable(int n, HashFunction<T>& _h) {
			V = *new vector<vector<T*>> (n);
			N = n;
			h = _h;
		}
		void insert(T* x) {
			V[h(*x)].push_back(x);
		}
		bool search(T* x, int &p1, int& p2) {
			p1 = h(*x);
			for (int i = 0; i < (int) V[p1].size(); i++) {
				if (*x == *V[p1][i]) {
					p2 = i;
					return true;
				}
			}
			p1 = p2 = -1;
			return false;
		}
		bool purge(T *x) {
			int i,j;
			bool found = search(x, i, j);
			if (!found) return false;
			V[i].erase(V[i].begin() + j - 1);
			return true;
		}
};

int main (void) {
	
	return 0;
	
}

