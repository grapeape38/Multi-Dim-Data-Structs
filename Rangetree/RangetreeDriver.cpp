#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Rangetree.cpp"
#include "Point.cpp"
#include "RangetreeQueries.cpp"
#include "time.h"
using namespace std;

template <class T>
vector<Point<T>*> loadPoints(const char* fname, int maxPoints) {
    ifstream ifs(fname);
    string line;
    vector<Point<T>*> points;
    int i = 0;
    while(getline(ifs, line) && i < maxPoints) {
        stringstream ss(line);
        vector<T> v;
        T x;
        while (ss >> x) 
            v.push_back(x);
        points.push_back(new Point<T>(v));
        i++;
    }
    ifs.close();
    return points;
}

template <class T>
void prompt(char* fname, int maxPoints) {
    auto points = loadPoints<T>(fname, maxPoints);
    time_t starttime = time(NULL);
    Rangetree<T> *rt = createRangeTree(points);
    double buildTime = difftime(time(NULL), starttime);
    vector<pair<int,int>> range(rt->maxDim + 1, make_pair(-3000, 3000));
    cout << points.size() << " points built in " << buildTime << " seconds." << endl;
    int t = 0;
    while (t < 5) {
        cout << "What type of query would you like to execute: \n";
        cout << "0) Display Points\n";
        cout << "1) Print Range Tree\n";
        cout << "2) Search\n";
        cout << "3) Count\n";
        cout << "4) Sum \n";
        cout << "5) Exit\n";
        cin >> t;
        if (t == 0) {
            for (auto &p : points)
                p->print();
            cout << "\n---------\n\n";
        }
        else if (t == 1) {
            rt->print();
        }
        else if (t < 5) {
            if (!range.size()) {
                for (int i = 1; i <= points[0]->dim(); i++) {
                    T B, E;
                    cout << "Range for dimension " << i << ": \n";
                    cin.ignore();
                    cin >> B >> E;
                    range.push_back(make_pair(B, E));
                }
            }
            Query<T> *q = createQuery(t-1, range);
            if (q == nullptr)
                cout << "Invalid query type." << endl;
            else {
                TimeQuery<T> tq(q);
                tq.timeQ(rt);
                tq.printResults();
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: filename" << endl;
    }
    else {
        int maxCount = 500000;
        if (argc > 2) {
            istringstream iss(argv[2]);
            iss >> maxCount;
        }
        prompt<int>(argv[1], maxCount);
    }
}


