#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <iostream>
#include "Resource.h"
#include "debug.h"

class Solution
{
public:
    Solution();
	Solution(vector<int> a, vector<int> t, vector<Resource> r);
	vector<int> a;
	vector<int> t;
	vector<Resource> r;
};

ostream &operator <<(ostream &os, const Solution &i);

#endif // SOLUTION_H
