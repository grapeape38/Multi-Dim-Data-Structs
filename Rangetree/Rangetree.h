#include <vector>
#include <algorithm>
using namespace std;

#ifndef RANGETREE_H
#define RANGETREE_H

template <class T>
class Point;

template <class T>
struct Node;

template <class T>
class LinkedList;

template <class T>
class Query;

template <class T>
class Rangetree {
    protected:
        Node<T>* root;
    public:
        int dim, maxDim;
        LinkedList<T> *leaves;
        Rangetree(vector<Point<T>*> &pts, int d);
        Rangetree(LinkedList<T> *ll, Rangetree<T> *rt);
        void buildTree();
        void buildRangeTree(vector<Point<T>*> &pts);
        void buildRangeTree(Rangetree<T>* rt);
        Node<T>* find(T x);
        virtual pair<Rangetree<T>*, Rangetree<T>*> split(T x, int d);
        virtual void query(Query<T> &q);

        ~Rangetree() { 
            delete root;
            delete leaves;
        }
        void print();
};

template <class T>
class Rangetree2D : public Rangetree<T> {
    public:
        Rangetree2D(vector<Point<T>*> &pts, int d) : Rangetree<T>(pts, d) {}
        Rangetree2D(LinkedList<T> *ll, Rangetree<T> *rt) : Rangetree<T>(ll, rt) {}
        pair<Rangetree<T>*, Rangetree<T>*> split(T x, int d);
        void query(Query<T> &q);
};
#endif
