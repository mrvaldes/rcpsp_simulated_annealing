#ifndef MODE_H
#define MODE_H

#include <map>
#include <iostream>
#include "defs.h"

using namespace std;

class Mode{
public:
    Mode();
	Mode(int duration, int ES, int EF, int LS, int LF);
	int Slack() const;		// diff. entre LS y ES
	int duration,		// ud. de tiempo
	ES, EF, LS, LF;		// variables para CPM
	map<int, int> req;	// req[1] = 2 -> se requiere 2 uds. del recurso 1
};

ostream &operator <<(ostream &os, const Mode &m);

#endif // MODE_H
