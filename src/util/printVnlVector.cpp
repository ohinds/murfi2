#include<vnl/vnl_matrix.h>
#include<vnl/vnl_vector.h>
#include <iostream>

using namespace std;

void printVnlVector(vnl_vector<double> v) {
  for(unsigned int i = 0; i < v.size(); i++) {
    cout << v[i] << " ";
  }
}
