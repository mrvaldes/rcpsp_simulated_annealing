#include "debug.h"

#define delim ';'

ostream &operator <<(ostream &os, const vector< vector<int> > vec) {
	for(unsigned int i = 0; i < vec.size(); i++) {
		for(unsigned int j = 0; j < (vec[i]).size(); j++) {
			os.width(3); os << vec[i][j] << delim;
		}
		os << endl;
	}
	return os;
}

ostream &operator <<(ostream &os, const vector<int> vec) {
	for(unsigned int i = 0; i < vec.size(); i++) os << "\nvec[" << i << "] = " << vec[i];
	return os;
}

ostream &operator <<(ostream &os, const map<int,int> m) {
	map<int, int>::const_iterator mit;
	for (mit = m.begin(); mit != m.end(); ++mit) {
		os << (*mit).first << " = " << (*mit).second << endl;
	}
	return os;
}

//template void pr< vector<int> >();
