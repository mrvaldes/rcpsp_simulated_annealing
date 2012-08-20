#include "RCPSP.h"

RCPSP::RCPSP()
{
}

int RCPSP::searchLineNumber(vector<string> &lines, string line) {
	for (unsigned int n = 0; n <= lines.size(); n++)
		if(line == lines[n]) return n;
	return 0;
}

void RCPSP::addArc(Activ * a, Activ * b){
	a->succ.push_back(b);
	b->pred.push_back(a);
}

void RCPSP::setHeaderData(string s, int &var){
	stringstream stream (s.substr(s.find(':')+1)); stream >> var;
}

void RCPSP::setProjectInfo(vector<string> &lines, int nline) {
	stringstream stream (lines[nline]);
	stream >> P_NUM >> P_JOBS >> P_REL_DATE >> P_DUE_DATE >> P_TARDCOST >> P_MPM_DATE;
	N = P_JOBS + 2;
	setPrecedenceRestrictions(lines);
	setRequestsRestrictions(lines);
}

void RCPSP::setPrecedenceRestrictions(vector<string> &lines) {
	int nline = searchLineNumber(lines, "PRECEDENCE RELATIONS:") + 2;
	for (int i = 0; i < N; i++) {
		stringstream stream (lines[nline+i]);
		Activ * a = new Activ;
		stream >> a->id >> a->nmodes >> a->nsucc;
		a->sm = Mode(0, EMIN, EMIN, LMAX, LMAX);
		a->selected = false; a->StartTime = 0; a->FinishTime = 0;
		J[a->id] = (*a);
		delete a;
	}
	// crear dependencia (arcos del grafo) entre actividades (nodos)
	for (int i = 0; i < N; i++) {
		stringstream stream (lines[nline+i]);
		int id, x;
		stream >> id >> x >> x;
		Activ * a = &J[id];
		Activ * b;
		for(int j = 0; j < a->nsucc; j++) {
			stream >> x;
			b = &J[x];
			addArc(a,b);
		}
	}
}

/*! @todo uso de otros recursos N y D */
void RCPSP::setMode(stringstream &stream, Activ &a) {
	int nmode, duration;
	Mode * m = &a.sm;
	stream >> nmode >> duration;
	m->duration = duration;
	m->EF = EMIN; m->ES = EMIN;
	m->LF = LMAX; m->LS = LMAX;
	for(int i = 0; i < G_RESOURCES_R; i++)
		stream >> m->req[i+1];
}

void RCPSP::setRequestsRestrictions(vector<string> &lines) {
	int nline = searchLineNumber(lines, "REQUESTS/DURATIONS:") + 2;
	for(int i = 1; i <= N; i++){
		stringstream stream (lines[nline+i]);
		int j; stream >> j;
		setMode(stream, J[j]); /*! @todo caso multimodo (sgtes líneas) */
	}
}

/*! @todo tirar error si G_HORIZON = 0 */
/*! @todo implementar para recursos tipo N y D */
void RCPSP::setResourceAvailabilities(vector<string> &lines) {
	int nline = searchLineNumber(lines, "RESOURCEAVAILABILITIES:") + 2;
	stringstream stream (lines[nline]);
	for(int i = 0; i < G_RESOURCES_R; i++) {
		Resource * r = new Resource;
		stream >> r->max;
		r->id = i+1;
		r->ResetUse(G_HORIZON); // matriz de G_HORIZON * r.max
		R.push_back((*r));
		delete r;
	}
	K = R.size();
}

bool RCPSP::readFile(char *fileName) {
	vector<string> lines;
	ifstream inputf(fileName);
	string line;
	if(!inputf.is_open()) {
		return false;
	} else {
		while (inputf.good()) { getline (inputf, line); lines.push_back(line);}
		inputf.close();
	}
	setHeaderData(lines[4], G_PROJECTS);
	setHeaderData(lines[5], G_JOBS);
	setHeaderData(lines[6], G_HORIZON);
	setHeaderData(lines[8], G_RESOURCES_R);
	setHeaderData(lines[9], G_RESOURCES_N);
	setHeaderData(lines[10], G_RESOURCES_D);
	setProjectInfo(lines, 14);
	setResourceAvailabilities(lines);
	return true;
}

/*! @todo funciona solo para SM, generalizar para MM.. */
void RCPSP::setTimesForwardRecursion(Activ * a) {
	if (a->pred.size() == 0) a->sm.ES = 0;
	for (vector<Activ*>::iterator pred = a->pred.begin(); pred != a->pred.end(); ++pred) {
		if ((*pred)->sm.EF > a->sm.ES) a->sm.ES = (*pred)->sm.EF;
	}
	a->sm.EF = a->sm.ES + a->sm.duration;
	for (vector<Activ*>::iterator succ = a->succ.begin(); succ != a->succ.end(); ++succ) {
		setTimesForwardRecursion((*succ));
	}
}

/*! @todo funciona solo para SM, sacar del for y generalizar para MM.. */
void RCPSP::setTimesBackwardRecursion(Activ * a, int UPPER_BOUND /* = -1*/) {
	if (UPPER_BOUND < 0) {
		if (a->succ.size()==0) a->sm.LF = a->sm.EF; // si no hay upper bound y es nodo final LF=EF
		for (vector<Activ*>::iterator succ = a->succ.begin(); succ != a->succ.end(); ++succ) {
			if (a->sm.LF == LMAX) a->sm.LF = (*succ)->sm.LS; // si no se ha seteado
			if ((*succ)->sm.LS < a->sm.LF) a->sm.LF = (*succ)->sm.LS;
		}
	} else {
		a->sm.LF = UPPER_BOUND; // con upper bound
	}
	a->sm.LS = (a->sm.LF - a->sm.duration);

	for (vector<Activ*>::iterator pred = a->pred.begin(); pred != a->pred.end(); ++pred) {
		setTimesBackwardRecursion((*pred));
	}
}

void RCPSP::setCPM(int UPPER_BOUND /*= -1*/) {
	Activ * ai = &((J.begin())->second);
	Activ * af = &((--J.end())->second);
	setTimesForwardRecursion(ai);
	setTimesBackwardRecursion(af, UPPER_BOUND);
}

vector<int> RCPSP::StartTimeList(vector<int> &Sg) {
	vector<int> res;
	map<int,Activ>::iterator it;
	for (unsigned int i = 0; i < Sg.size(); i++) {
		it = J.find(Sg[i]);
		Activ * a = &((*it).second);
		res.push_back(a->StartTime);
	}
	return res;
}

vector<int> RCPSP::FinishTimeList(vector<int> &Sg) {
	vector<int> res;
	map<int,Activ>::iterator it;
	for (unsigned int i = 0; i < Sg.size(); i++) {
		it = J.find(Sg[i]);
		Activ * a = &((*it).second);
		res.push_back(a->FinishTime);
	}
	return res;
}

/*!
Se asume cada elemento en S marcado (selected=true).
Tomar todos los elementos en J, restar elementos en S (se podría optimizar
tomando solo los sucesores de elementos en Sg, pero por definicion del paper se
busca en todo A), y por cada j verificar si predecesores P_j están seleccionados.
*/
void RCPSP::setElegiblesByPrecedence(vector<int> &Dg, vector<int> &Sg) {
	for (map<int,Activ>::iterator it = J.begin(); it != J.end(); ++it) {
		if(!it->second.selected) { // no existe en S, verificar P_j
			bool precedenceOK = true;
			for (vector<Activ*>::iterator pred = it->second.pred.begin(); pred != it->second.pred.end(); ++pred) {
				if (!(*pred)->selected) { precedenceOK = false; break;}
			}
			if (precedenceOK) {
				Dg.push_back(it->second.id); // es elegible
			}
		} else { // ya existe en S
			continue;
		}
	}
}

/*! ojo q siempre generará la misma solución dado un orden de actividades */
void RCPSP::precedenceSolution(Activ &a0, vector<int> &Sg) {
	vector<int> Dg;
	if (!a0.selected) {
		a0.selected = true;
		Sg.push_back(a0.id);
		setElegiblesByPrecedence(Dg, Sg);
		for (unsigned int i = 0; i < Dg.size(); i++) precedenceSolution(J[Dg[i]], Sg);
	}
}

/*! @todo antes de obtener lista, des-seleccionar todos los nodos en A */
Solution RCPSP::feasibleSolution() {
	vector<int> Sg, Dg, T;

	// obtener lista de actividadas q satisf. restr de precedencia
	Activ * a0 = &(*J.begin()).second;	// seleccionar nodo inicial y agregar a lista
	a0->selected = true;
	Sg.push_back(a0->id);

	// obtener Dg = ids de nodos que se pueden seleccionar segun precedencia
	setElegiblesByPrecedence(Dg, Sg);
	for (unsigned int i = 0; i < Dg.size(); i++) {
		precedenceSolution(J[Dg[i]], Sg); // por c/u recorrer recursivamente
	}

	// asignar tiempos a T y usos a R según restr. de recursos y orden de lista generada
	scheduleFromList(Sg);
//	T = StartTimeList(Sg);
	T = FinishTimeList(Sg);
	Solution Sol (Sg, T, R);
	return Sol;
}

void RCPSP::resetResources() {
	for(vector<Resource>::iterator rit = R.begin(); rit != R.end(); ++rit) {
		Resource * r = &(*rit);
		r->ResetUse(G_HORIZON);
	}
}

void RCPSP::resetActivities() {
	for(map<int,Activ>::iterator it = J.begin(); it != J.end(); ++it) {
		Activ a = (*it).second;
		a.ResetTimes();
	}
}

/*!
se asignan tiempos y uso de recursos secuencialmente en el orden dado por la lista
*/
void RCPSP::scheduleFromList(vector<int> &Sg) {
	// resetear actividades y recursos
	resetResources();
	resetActivities();

	map<int,Activ>::iterator ait;
	for (unsigned int i = 0; i < Sg.size(); i++) {
		ait = J.find(Sg[i]);
		Activ * a = &((*ait).second);

		// si es inicial asignar 0 y saltar al prox. en la lista
		if (a->pred.size() <= 0) {
			a->StartTime = 0; a->FinishTime = 0;
			continue;
		}

		// por cada recurso buscar el t mayor en que hay espacios para reqs.
		int tMayor = 0;
		for(unsigned int ir = 0; ir < R.size(); ir++){
			Resource * r = &R[ir];
			int tRecurso = r->tiempoParaActividad(*a);
			if (tMayor < tRecurso) tMayor = tRecurso;
		}

		// asignar en t mayor
		a->StartTime = tMayor;
		a->FinishTime = tMayor + a->sm.duration -1;

		// asignar recursos
		for(unsigned int ir = 0; ir < R.size(); ir++){
			Resource * r = &R[ir];
			int d = a->sm.duration;
			int req = a->sm.req[r->id];
			r->assign(tMayor, a->id, req, d);
		}
	}

	// asegurarse de tener el nodo final actualizado
	Activ * af = &((--J.end())->second);
	int tfinal = af->predMaxFj();
	af->StartTime = tfinal;
	af->FinishTime = tfinal;

}

Solution RCPSP::neighbour(vector<int> Sg) {
	vector<int> T;
	Activ * a;

	int posicion = 0;
	int idActiv = 0;
	int size = Sg.size();

	int min = 1;		// descartar el primero
	int max = size - 2;	// descartar el ultimo
	while(true) {
		// select random activity
		posicion = rand() % ((max+1) - (min)) + (min);
		idActiv = Sg[posicion];
		a = &J[idActiv];

		// obtener id pred y succ
		int idPred = a->predMaxFj_id();
		int idSucc = a->succMinSj_id();

		// obtener posiciones pred y succ en Sc.a
		unsigned int ip;
		unsigned int is;
		for(ip=0; ip<Sg.size();ip++) if (Sg[ip]==idPred) break;
		for(is=0; is<Sg.size();is++) if (Sg[is]==idSucc) break;

		int rango = is-ip-1;
		int nuevaPosicion = 0;
		if (rango > 1) {
			// seleccionar random entre 1 y distancia
			nuevaPosicion = (rand() % rango) + 1; // descarta primero y ultimos (dummy)
			nuevaPosicion += ip;
		} else {
			continue; // seleccionar otra actividad
		}
		int diff = nuevaPosicion - posicion;

		// shift ciclico
		vector<int>::iterator it = Sg.begin() + posicion;
		vector<int>::iterator temp = it;
		advance(it, diff);
		Sg.erase(temp);
		Sg.insert(it, 1, idActiv);
		break;
	}


	//	schedule(Sg);
	scheduleFromList(Sg);
	T = StartTimeList(Sg);
	//T = FinishTimeList(Sg);
	Solution Sol (Sg, T, R);
	return Sol;
}

int RCPSP::eval(Solution &Sn) {
	return Sn.t.back();
}

ostream &operator <<(ostream &os, const RCPSP &i) {
	os << "\nG_PROJECTS:" << i.G_PROJECTS
	   << "\nG_JOBS:" << i.G_JOBS
	   << "\nG_HORIZON:" << i.G_HORIZON
	   << "\nG_RESOURCES_R:" << i.G_RESOURCES_R
	   << "\nG_RESOURCES_N:" << i.G_RESOURCES_N
	   << "\nG_RESOURCES_D:" << i.G_RESOURCES_D
	   << "\nP_NUM:" << i.P_NUM
	   << "\nP_JOBS:" << i.P_JOBS
	   << "\nP_REL_DATE:" << i.P_REL_DATE
	   << "\nP_DUE_DATE:" << i.P_DUE_DATE
	   << "\nP_TARDCOST:" << i.P_TARDCOST
	   << "\nP_MPM_DATE:" << i.P_MPM_DATE << endl;
//	os << i.R << endl;
//	os << i.J << endl;
	return os;
}

ostream &operator <<(ostream &os, const map<int,Activ> &J) {
	for (map<int,Activ>::const_iterator j = J.begin(); j != J.end(); ++j) {
		os << j->second << endl;
	}
	return os;
}

ostream &operator <<(ostream &os, const vector<Resource> &R) {
	for (vector<Resource>::const_iterator r = R.begin(); r != R.end(); ++r) {
		os << *r << endl;
	}
	return os;
}


