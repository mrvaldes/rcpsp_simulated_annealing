#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

/*! funciones y operadores para debuguear */
//template <class T>
//extern void pr(const T& myvar);

// declarar Y DEFINIR aqui
template <class T> void pr(const T& myvar) {
	cout << myvar << endl;
}


ostream &operator <<(ostream &os, const vector< vector<int> > vec);

ostream &operator <<(ostream &os, const vector<int> vec);

ostream &operator <<(ostream &os, const map<int, int> map);

#endif // DEBUG_H
