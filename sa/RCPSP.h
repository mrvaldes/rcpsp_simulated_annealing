#ifndef RCPSP_H
#define RCPSP_H

#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "stdlib.h"
#include "defs.h"
#include "Activ.h"
#include "Solution.h"
#include "debug.h"

using namespace std;

class RCPSP
{
public:
	int G_PROJECTS,			// info del dataset
	G_JOBS,
	G_HORIZON,
	G_RESOURCES_R,
	G_RESOURCES_N,
	G_RESOURCES_D;

	int P_NUM,				// info del proyecto (1)
	P_JOBS,
	P_REL_DATE,
	P_DUE_DATE,
	P_TARDCOST,
	P_MPM_DATE;

	int N;					// # de actividades
	map<int,Activ> J;		// conjunto de actividades
	int K;					// # de recursos
	vector<Resource> R;		// conjunto de recursos

	RCPSP();							// ctor
	bool readFile(char *fileName);		// leer dataset
	Solution feasibleSolution();		// obtener sol. inicial
	Solution neighbour(vector<int> Sg);	// generar vecino
	int eval(Solution &Sn);				// evaluar sol.
	void setCPM(int UPPER_BOUND = -1);
	vector<int> StartTimeList(vector<int> &Sg);	// lista con tiempos de inicio
	vector<int> FinishTimeList(vector<int> &Sg); // idem de fin

private:
	int searchLineNumber(vector<string> &lines, string line);
	void addArc(Activ * a, Activ * b);

	void setHeaderData(string s, int &var);
	void setProjectInfo(vector<string> &lines, int nline);
	void setPrecedenceRestrictions(vector<string> &lines);
	void setMode(stringstream &stream, Activ &a);
	void setRequestsRestrictions(vector<string> &lines);
	void setResourceAvailabilities(vector<string> &lines);

	void setTimesForwardRecursion(Activ * a);
	void setTimesBackwardRecursion(Activ * a, int UPPER_BOUND = -1);

	void setElegiblesByPrecedence(vector<int> &Dg, vector<int> &Sg);
	void precedenceSolution(Activ &a0, vector<int> &Sg);
	void scheduleFromList(vector<int> &Sg);
	void resetResources();
	void resetActivities();
};

ostream &operator <<(ostream &os, const RCPSP &i);

ostream &operator <<(ostream &os, const map<int,Activ> &J);

ostream &operator <<(ostream &os, const vector<Resource> &R);

#endif // RCPSP_H
