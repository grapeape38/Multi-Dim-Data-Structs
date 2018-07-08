#include <vector> 
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Rangetree.cpp"
#include "Point.cpp"
#include "RangetreeQueries.cpp"
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

int main() {
    auto points = loadPoints<int>("../input/points3D.txt", 200);
    Rangetree<int> *rt = createRangeTree(points);
    vector<pair<int,int>> range(rt->maxDim + 1, make_pair(0, 100000));
    
    auto notInRange = [&](Point<int>* p) { return !p->inRange(range); };
    vector<Point<int>*> testSearch(points.begin(), 
            remove_if(points.begin(), points.end(), notInRange));

    Point<int> testSum = Point<int>{vector<int>(range.size())};
    for (auto &p : testSearch)
        testSum += (*p);

    SearchQuery<int> sq(range);
    sq.Query::query(rt);
    sq.printResults();
    cout << endl;
    sq.testResults(testSearch);

    CountQuery<int> cq(range);
    cq.Query::query(rt);
    cq.printResults();

    SumQuery<int> sumq(range);
    sumq.Query::query(rt);
    sumq.printResults();
}


