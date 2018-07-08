#include <iostream>
#include <utility>
#include <vector>
using namespace std;

#ifndef POINT
#define POINT
template<class T>
class Point {
    public:
        Point(T arr[], int n) : data(vector<T>(arr, arr+n)) {}
        Point(vector<T> v) : data(v) {} 
        T operator[](size_t d) const {
            return data[d];
        }
        size_t dim() {
            return data.size();
        }
        Point<T>& operator +=(const Point<T> &p2) {
            for (int i = 0; i < (int)data.size(); i++)
                data[i]+=p2[i];
            return *this;
        }
        Point<T>& operator -=(const Point<T> &p2) {
            for (int i = 0; i < (int)data.size(); i++)
                data[i]-=p2[i];
            return *this;
        }
        /*Point<double> operator / (T v) {
            vector<double> x(data);
            for (int i = 0; i < (int)data.size(); i++)
                x[i] /= v;
            return Point(x);
        }*/
        friend Point<T> operator +(Point<T> p1, const Point<T> &p2) {
            p1 += p2;
            return p1;
        }
        friend Point<T> operator -(Point<T> p1, const Point<T> &p2) {
            p1 -= p2;
            return p1;
        }
        bool inRange(vector<pair<T,T>> &range) { 
            for (int i = 0; i < (int)range.size(); i++) {
                if (range[i].first > data[i] || data[i] > range[i].second)
                    return false;
            }
            return true;
        }
        void print() {
            cout << "( ";
            for (T v : data)
                cout << v << " ";
            cout << " )";
        }
    private:
        vector<T> data;
};

template <class T>
bool cmpD(const Point<T> &p1, const Point<T> &p2, size_t d) {
    return p1[d] < p2[d];
}
#endif 


