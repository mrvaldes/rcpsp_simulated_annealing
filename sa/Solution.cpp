#include "Solution.h"

Solution::Solution()
{
}

Solution::Solution(vector<int> aa, vector<int> tt, vector<Resource> rr)
{
	a = aa; t = tt; r = rr;
}

ostream &operator <<(ostream &os, const Solution &s) {
	os << "Solution:";
	os << "\n\tActivities:" << s.a << endl;
	os << "\n\tTimes:" << s.t << endl;
	for (vector<Resource>::const_iterator j = s.r.begin(); j != s.r.end(); ++j) {
		os << (*j) << endl;
	}
	return os;
}
