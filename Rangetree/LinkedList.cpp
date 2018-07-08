#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include "Point.cpp"
#include "RangetreeQueries.cpp"

using namespace std;

template <typename T>
class Rangetree;

template <typename T>
class LinkedList;

template <typename T>
struct Node {
    Point<T> *pt, *sum;
    Node<T>* left, *right, *next, *prev, *cleftB, *cleftE, *crightB, *crightE;
    Node<T>** pointers[];
    Rangetree<T> *rt;
    LinkedList<T> *ll;
    T val;
    int count, tag, maxDim;
    Node(Point<T> *p, int dim) : pt(p) {
        val = (*p)[dim];
        sum = new Point<T>(*pt);
        tag = 0;
        count = 1;
        left = right = next = nullptr;
        maxDim = p->dim() - 1;
        rt = nullptr;
        ll = nullptr;
    }
    Node(Node *l, Node *r, T v) : left(l), right(r), val(v) {
        tag = 1;
        maxDim = l->maxDim;
        rt = nullptr;
        ll = nullptr;
    }
    ~Node() {
        delete sum;
        delete pt;
        delete rt;
    }
    bool isLeaf() { return tag == 0; }
    T dim(int d) { return (*pt)[d]; }
    void print() { pt->print(); }
    bool inRange(vector<pair<T,T>> &range) { return pt->inRange(range); }
};

template <typename T>
class LinkedList {
    private:
        Node<T>* head, *tail;
        int size, dim;
        Node<T>* toTree(int n, Node<T> **currhead) {
            if (n <= 0) return nullptr;
            if (n == 1) return *currhead;
            Node<T>* left = toTree((n+1) / 2, currhead);
            int val  = ((*currhead)->val);
            //no duplicate tree entries
            while ((*currhead)->val == val)
                *currhead = (*currhead)->next;
            Node<T>* right = toTree(n / 2, currhead);
            return new Node<T>(left, right, val);
        }
    public:
        LinkedList(int d) : dim(d) {
            size = 0;
            head = tail = nullptr;
        }
        LinkedList(vector<Point<T>*> &v, int d) : dim(d)  {
            size = 0;
            head = tail = nullptr;
            auto cmp = [&](Point<T> *p1, Point<T> *p2) { 
                return cmpD<T>(*p1,*p2,dim); };
            sort(v.begin(), v.end(), cmp);
            for (auto &p : v) {
                Node<T> *n = new Node<T>(p,dim);
                add(n);
            }
        }
        ~LinkedList() {
            delete head;
            delete tail;
        }

        int getDim() { return dim; }

        void add(Node<T> *n) {
            if (size == 0) {
                head = tail = n; 
                size++;
            }
            else  {
                //don't inc size for duplicates
                if (tail->val != n->val)
                    size++;
                tail->next = n;
                n->prev = tail;
                n->count = tail->count+1;
                *(n->sum) += *(tail->sum);
                tail = n;
            }
        }

        void query(Query<T> &q) {
            for (Node<T>* curr = head; curr != nullptr; curr = curr->next)
                q.query(curr);
        }

        pair<LinkedList<T>*,LinkedList<T>*> split(T x, int d) {
            LinkedList<T> *l1 = new LinkedList<T>(dim),
                          *l2 = new LinkedList<T>(dim);
            for (Node<T>* curr = head; curr != nullptr; curr = curr->next) {
                Node<T> *n = new Node<T>(curr->pt, dim);
                if (curr->dim(d) <= x)
                    l1->add(n);
                else
                    l2->add(n);
            }
            return make_pair(l1, l2);
        }

        pair<LinkedList<T>*,LinkedList<T>*> split2D(T x) {
            int d = dim - 1;
            LinkedList<T> *l1 = new LinkedList<T>(dim),
                          *l2 = new LinkedList<T>(dim);
            vector<Node<T>*> firstGreater, firstLess;
            if (!size) return make_pair(l1,l2);
            bool flag = head->dim(d) <= x;
            Node<T> *n, *prev = nullptr;
            for (Node<T>* curr = head; curr != nullptr; curr = curr->next) {
                bool left = curr->dim(d) <= x;
                Node<T> *n = new Node<T>(curr->pt, dim);
                if (left) {
                    curr->cleftB = n;
                    curr->cleftE = n;
                    l1->add(n);
                }
                else {
                    curr->crightB = n;
                    curr->crightE = n;
                    l2->add(n);
                }
                if (flag != left) {
                    firstGreater.push_back(n);
                    firstLess.push_back(prev);
                    flag = !flag;
                }
                prev = n;
            }
            int c = 0;
            flag = head->dim(d) <= x;
            for (Node<T>* curr = head; curr != nullptr; curr = curr->next) {
                bool left = curr->dim(d) <= x;
                if (flag != left) {
                    c++;
                    flag = !flag;
                }
                Node<T> *B = c < firstGreater.size() ? 
                    firstGreater[c] : nullptr;
                Node<T> *E = c > 0 ?
                    firstLess[c-1] : nullptr;
                if (left) {
                    curr->crightB = B; 
                    curr->crightE = E;
                }
                else  {
                    curr->cleftB = B; 
                    curr->cleftE = E; 
                }
            }
            return make_pair(l1, l2);
        }

        void print() {
            for (Node<T>* curr = head; curr != nullptr; curr = curr->next)
                cout << curr->val << " ";
            cout << endl;
        }

        Node<T>* toTree() {
            Node<T>* curr = head;
            return toTree(size, &curr);
        }
        void inOrder(Node<T>* n, int depth) {
            string tabs = "";
            for (int i = 0; i < depth; i++) tabs += "     ";
            if (n == nullptr) return;
            inOrder(n->left, depth+1);
            cout << tabs << n->val << endl;
            inOrder(n->right, depth+1);
        }
        void inOrder() {
            inOrder(toTree(), 0);
        }
};


