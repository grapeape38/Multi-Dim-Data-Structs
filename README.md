# Multi-Dim-Data-Structs
# Multi-Dim-Data-Structs
This will be C++ implementations for a couple popular data structures intended for use with multi-dimensional sets of data. Most algorithms are adapted from Hanan Samet's [Foundations of Multidimensional and Metric Data Structures](https://www.amazon.com/Foundations-Multidimensional-Structures-Kaufmann-Computer/dp/0123694469), and slides from 
Marc van Kreveld's [Geoemetric Algorithms course](http://www.cs.uu.nl/docs/vakken/ga/)
# Range trees
First data structure, trades highly efficient range queries for expensive construction and substantial space requirements. This implementation uses a technique known as fractional cascading to reduce the complexity of a query from O(log^d(n)) to O(log^(d-1)(n)). In order to use on your own data, create a file f points with one column representing a dimension. Then make the executable in the rangetree directory, and call it: `rangetree (points file) (max number of points to read)`. Then you can experiment with various queries, such as range searching, counting, and summing.
