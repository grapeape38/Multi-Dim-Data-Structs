#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include "Rangetree.h"
#include "RangetreeQueries.cpp"
#include "LinkedList.cpp"
#include "Point.cpp"

using namespace std;

template <class T>
void buildRangeTree(Node<T>* curr, Rangetree<T>* rt, int dim) {
    curr->rt = rt;
    curr->ll = rt->leaves;
    if (!curr->isLeaf()) {
        auto split = rt->split(curr->val, dim);
        buildRangeTree(curr->left, split.first, dim);
        buildRangeTree(curr->right, split.second, dim);
    }
}

template <class T>
void buildRangeTree(Node<T>* curr, LinkedList<T> *ll) {
    curr->ll = ll;
    if (!curr->isLeaf()) {
        auto split = ll->split2D(curr->val);
        buildRangeTree(curr->left, split.first);
        buildRangeTree(curr->right, split.second);
    }
}

template <class T>
pair<Rangetree<T>*, Rangetree<T>*> Rangetree2D<T>::split(T x, int d) {
    auto s = this->leaves->split(x,d);
    auto rs = this->root->rt->split(x,d);
    Rangetree<T>* l = new Rangetree2D<T>(s.first, rs.first);
    Rangetree<T>* r =  new Rangetree2D<T>(s.second, rs.second);
    return make_pair(l,r);
}

template <class T>
bool movePointers(Node<T> *&curr, Node<T> *&B, Node<T> *&E, bool left) {
    if (left) {
        curr = curr->left;
        B = B->cleftB;
        E = E->cleftE;
    }
    else {
        curr = curr->right;
        B = B->crightB;
        E = E->crightE;
    }
    return (B != nullptr && E != nullptr);
}

template <class T>
void Rangetree2D<T>::query(Query<T> &q) {
    auto &root = this->root;
    auto &dim = this->dim;
    auto range = q.getRange();
    if (root->isLeaf()) {
        q.query(this->leaves);
        return;
    }
    T L = range[dim].first, R = range[dim].second, B = range[dim+1].first, E = range[dim+1].second;
    auto B_ptr = root->rt->find(B);
    auto E_ptr = root->rt->find(E);
    if (B_ptr->val < B)
        B_ptr = B_ptr->next;
    if (E_ptr->val > E)
        E_ptr = E_ptr->prev;
    //start and end pointers must point to something
    if (B_ptr == nullptr || E_ptr == nullptr) return;
    Node<T>* Q = root;
    while (!Q->isLeaf() && (Q->val < L || Q->val > R)) {
        if (Q->val < L)  {
            if (!movePointers(Q,B_ptr,E_ptr,false)) return;
        }
        else  {
            if (!movePointers(Q,B_ptr,E_ptr,true)) return;
        }
    }
    if (Q->isLeaf()) {
        q.query(Q);
        return;
    }
    Node<T> *curr = Q->left, *currB = B_ptr->cleftB, *currE = E_ptr->cleftE;
    while (!curr->isLeaf() && currB != nullptr && currE != nullptr) {
        if (L <= curr->val) {
            q.query(currB->crightB, currE->crightE);
            movePointers(curr, currB, currE, true);
        }
        else {
            movePointers(curr, currB, currE, false);
        }
    }
    q.query(curr->ll);
    curr = Q->right; currB = B_ptr->crightB, currE = E_ptr->crightE;
    while (!curr->isLeaf() && currB != nullptr && currE != nullptr) {
        if (R > curr->val) {
            q.query(currB->cleftB, currE->cleftE);
            movePointers(curr, currB, currE, false);
        }
        else {
            movePointers(curr, currB, currE, true);
        }
    }
    q.query(curr->ll);
}

template <class T>
Rangetree<T>::Rangetree(vector<Point<T>*> &pts, int d) : dim(d) {
    leaves = new LinkedList<T>(pts, d);
    buildTree();
    buildRangeTree(pts);
}

template <class T>
Rangetree<T>::Rangetree(LinkedList<T> *ll, Rangetree<T> *rt) : leaves(ll) {
    dim = ll->getDim();
    buildTree();
    buildRangeTree(rt);
}

template <class T>
void Rangetree<T>::buildTree() {
    root = leaves->toTree();
    maxDim = root->maxDim;
}

template <class T>
void Rangetree<T>::buildRangeTree(vector<Point<T>*> &pts) {
    if (dim < maxDim) {
        Rangetree<T> *rt = (dim == maxDim - 2)
            ? new Rangetree2D<T>(pts, dim+1) 
            : new Rangetree<T>(pts,dim+1);
       buildRangeTree(rt); 
    }
}

template <class T>
void Rangetree<T>::buildRangeTree(Rangetree<T>* rt) {
    if (dim == maxDim - 1) {
        root->rt = rt;
        if (!root->isLeaf()) {
            auto split = rt->leaves->split2D(root->val);
            ::buildRangeTree(root->left, split.first);
            ::buildRangeTree(root->right, split.second);
        }
    }
    else if (dim < maxDim)
        ::buildRangeTree(root, rt, dim);
}

template <class T>
pair<Rangetree<T>*, Rangetree<T>*> Rangetree<T>::split(T x, int d) {
    auto s = leaves->split(x,d);
    Rangetree<T>* l, *r, *rtl, *rtr;
    l = r = rtl = rtr = nullptr;
    if (dim < maxDim) {
        auto rs = root->rt->split(x,d);
        rtl = rs.first;
        rtr = rs.second;
    }
    l = new Rangetree<T>(s.first, rtl);
    r = new Rangetree<T>(s.second, rtr);
    return make_pair(l,r);
}

template <class T>
Node<T>* Rangetree<T>::find(T x) {
    Node<T>* curr = root;
    while (!curr->isLeaf()) {
        if (x > curr->val)
            curr = curr->right;
        else
            curr = curr->left;
    }
    return curr;
}

template <class T>
void Rangetree<T>::print() {
    string tabs = "";
    for (int i = 0; i < dim; i++) tabs+="\t";
    if (dim < maxDim) {
        queue<Node<T>*> q;
        q.push(root);
        while (!q.empty()) {
            Node<T>* u = q.front();
            q.pop();
            if (!u->isLeaf()) {
                cout << tabs << "Non-leaf " << u->val << ": " << endl;
                if (u != root && dim == maxDim - 1) {
                    cout << tabs + "\t" + "Linked List: ";
                    u->ll->print();
                    cout << endl;
                }
                else
                    u->rt->print();
                q.push(u->left);
                q.push(u->right);
            }
        }
    }
    cout << tabs;
    leaves->print();
    cout << endl;
}


template<class T>
void Rangetree<T>::query(Query<T> &q) {
    auto range = q.getRange();
    T L = range[dim].first, R = range[dim].second;
    if (root->isLeaf()) {
        q.query(leaves);
        return;
    }
    Node<T>* Q = root;
    while (!Q->isLeaf() && (Q->val < L || Q->val > R)) {
        if (Q->val < L) 
            Q = Q->right;
        else 
            Q = Q->left;
    }
    if (Q->isLeaf()) {
        q.query(Q);
        return;
    }
    Node<T>* curr = Q->left;
    while (!curr->isLeaf()) {
        if (L <= curr->val) {
            q.query(curr->right->rt);
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }
    q.query(curr->ll);
    curr = Q->right;
    while (!curr->isLeaf()) {
        if (R > curr->val) {
            q.query(curr->left->rt);
            curr = curr->right;
        }
        else {
            curr = curr->left;
        }
    }
    q.query(curr->ll);
}


template <class T>
Rangetree<T>* createRangeTree(vector<Point<T>*> &pts) {
    if (!pts.size()) {
        cout << "Error, empty points vector.\n";
        return nullptr;
    }
    if (pts[0]->dim() == 2)
        return new Rangetree2D<T>(pts, 0);
    return new Rangetree<T>(pts, 0);
}


