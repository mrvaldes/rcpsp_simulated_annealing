#include "Resource.h"

Resource::Resource() : id(0), max(0) {
}

Resource::Resource(int id, int max) {
	id = id; max = max;
}

int Resource::tiempoParaActividad(Activ &a) {
	int res = 0;
	if (a.pred.size() == 0) return res;

	int id_recurso = this->id;
	int req = a.sm.req[id_recurso];	// req. r_jk
	int d = a.sm.duration;			// duracion
	int tPred = a.predMaxFj();		// t en q finalizo ultimo de sus predecesores
	tPred++;						// puede empezar en el t sgte
	if (req <= 0 || d <= 0) {
		return tPred;	// el req. ya cabe, solo debe cumplir precedencia
	}

	int t = findSpace(tPred, req, d);
	return t;
}

int Resource::available(int t) {
	int count = 0;
	for(int i = 0; i < max; i++) { // debe contar hasta max
		if (use[t][i] == 0) count++;
	}
	return count;
}

/*! buscar un espacio en que quepa req. de r*d a partir del tiempo t,
se asume d >= 1
*/
int Resource::findSpace(int t, int req, int d) {
	if (t < 0) return -1;
	for(unsigned int i=t; i < use.size(); i++) { // iniciar i desde t
		bool cabe = false;
		if (req <= available(i)) { // si hay disponible en i, verificar en sgtes i+d
			cabe = true;
			for (int j = 1; j < d; j++) { // iniciar j en 1, hasta d-1
				if (req > available(i+j)) { // no cabe
					cabe = false;
					break;
//					i += (d-1);	// adelantar los (d-1) en q ya no cabe
				}
			}
			if (cabe) return i;
		}
	}
	return -1; // retornar si no encontró espacio
}

void Resource::assign(int t, int id, int req, int d){
	for(int i = 0; i < d; i++) {
		for (int j = 0; j < req; j++) {
			assignUnit(t+i, id);
		}
	}
}

bool Resource::assignUnit(int t, int id) {
	for(int i = 0; i < max; i++) {
		if (this->use[t][i] == 0) {
			this->use[t][i] = id;
			return true;
		}
	}
	return false;
}

void Resource::ResetUse(int tMax) {
	int rows = this->max;
	int cols = tMax;
	vector< vector<int> > temp(cols, vector<int>(rows, 0)); // en 0
	this->use = temp;
}

ostream &operator <<(ostream &os, const Resource &r) {
	os << "Resource"
	   << "\n\tid: " << r.id
	   << "\n\tmax: " << r.max
	   << "\n\tuse: \n" << r.use
	   << '\n';
	return os;
}
