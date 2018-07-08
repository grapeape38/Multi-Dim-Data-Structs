#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include "Rangetree.h"
#include "time.h"

#ifndef RANGETREE_QUERIES
#define RANGETREE_QUERIES
using namespace std;

template <class T>
using Range = vector<pair<T,T>>;

template <class T>
class Query {
    protected:
        Range<T> &range;
    public:
        Query(Range<T> &r) : range(r) {}
        Range<T> &getRange() { return range; }
        void query(Rangetree<T> *rt) {
            rt->query(*this);
        }
        void query(LinkedList<T> *ll) {
            ll->query(*this);
        }
        virtual void query(Node<T>* n)=0;
        virtual void query(Node<T>* s, Node<T> *e)=0;
        virtual void printResults()=0;
};

template <class T>
class TimeQuery {
    private:
        Query<T> *query;
        double diff;
    public:
        TimeQuery(Query<T> *q) : query(q) {}
        void timeQ(Rangetree<T> *rt) {
            time_t startTime = time(NULL);
            query->query(rt);
            diff = difftime(time(NULL), startTime);
        }
        void printResults() {
            query->printResults();
            cout << "Query finished in " << diff << " seconds." << endl;
        }
};

template <class T>
class SumQuery : public Query<T> {
    private: 
        Point<T> result;
    public:
        SumQuery<T>(Range<T> &r) : Query<T>(r), result{vector<T>(r.size())} {}
        void query(Node<T>* n) {
            if (n->inRange(this->range)) {
                result += *(n->pt);
            }
        }
        void query(Node<T>* s, Node<T> *e) {
            if (s != nullptr && e != nullptr) {
                result = result + *(e->sum) - (*s->sum) + *(s->pt);
            }
        }
        void printResults() {
            cout << "The sum over the range is: ";
            result.print();
            cout << endl;
        }
        Point<T> &getResult() { return result; }
};

template <class T>
class CountQuery : public Query<T> {
    private: 
        int result;
    public:
        CountQuery<T>(Range<T> &r) : Query<T>(r) {
            result = 0;
        }
        void query(Node<T>* n) {
            if (n->inRange(this->range))
                result++;
        }
        void query(Node<T>* s, Node<T> *e) {
            if (s != nullptr && e != nullptr)
                result += e->count - s->count + 1;
        }
        void printResults() {
            cout << "The number of points in the range is: " << result << endl;
        }
        int getResult() { return result; }
};

/*
template <class T>
class AverageQuery : public Query<T> {
    private:
        SumQuery<T> sq;
        CountQuery<T> cq;
    public:
        AverageQuery<T>(Range<T> &r) : Query<T>(r), sq(r), cq(r) {}
        void query(Node<T>* n) {
            sq.query(n);
            cq.query(n);
        }
        void query(Node<T>* s, Node<T> *e) {
            sq.query(s,e);
            cq.query(s,e);
        }
        void printResults() {
            if (cq.getResult() > 0) {
                Point<double> result = sq.getResult() / cq.getResult();
                cout << "The average of the points in the range is: ";
                result.print();
                cout << endl;
            }
            else
                cout << "No points were found in the range." << endl;
        }
};*/

template <class T>
class SearchQuery : public Query<T> {
    private:
        vector<Point<T>*> results;
    public:
        SearchQuery<T>(Range<T> &r) : Query<T>(r) {}
        void query(Node<T>* n) {
            if (n->inRange(this->range))
                results.push_back(n->pt);
        }
        void query(Node<T>* s, Node<T> *e) {
            if (e != nullptr) {
                for (Node<T>* curr = s;
                        curr != nullptr && curr->val <= e->val; curr = curr->next)   
                    results.push_back(curr->pt);
            }
        }
        void printResults() {
            cout << "The search results are: " << endl;
            for (auto &p : results)
                p->print();
            cout << ": " << results.size() << " points." << endl;
        }
        void testResults(vector<Point<T>*> &points) {
            set<Point<T>*> s(results.begin(), results.end());
            for (auto &p: points) {
                if (s.find(p) == s.end()) {
                    p->print();
                    cout << " not in results." << endl;
                }
            }
        }
};

namespace QueryType
{
    const int
        SEARCH = 1,
        COUNT = 2,
        SUM = 3,
        AVERAGE = 4;
};

template <class T>
Query<T>* createQuery(int type, Range<T> &r) {
    using namespace QueryType;
    switch (type) {
        case SEARCH:
            return new SearchQuery<T>(r);
            break;
        case COUNT:
            return new CountQuery<T>(r);
            break;
        case SUM:
            return new SumQuery<T>(r);
            break;
        /*
        case AVERAGE:
            return new AverageQuery<T>(r);
            break;*/
        default:
            return nullptr;
    }
}

#endif



