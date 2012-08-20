#include "Activ.h"
Activ::Activ()
{
}

bool Activ::isPrecedenceElegible(){
	for (vector<Activ*>::iterator p = pred.begin(); p != pred.end(); ++p) {
		if (!((*p)->selected)) return false;
	}
	return true;
}

int Activ::predMaxFj() const {
	int max = 0;
	for (vector<Activ*>::const_iterator p = pred.begin(); p != pred.end(); ++p) {
		if ((*p)->FinishTime > max) max = (*p)->FinishTime;
	}
	return max;
}

int Activ::succMinSj() const {
	int min = LMAX;
	for (vector<Activ*>::const_iterator p = succ.begin(); p != succ.end(); ++p) {
		if ((*p)->StartTime < min) min = (*p)->StartTime;
	}
	return min;
}

int Activ::predMaxFj_id() const {
	int max = 0;
	int id = 0;
	for (vector<Activ*>::const_iterator p = pred.begin(); p != pred.end(); ++p) {
		if ((*p)->FinishTime > max) {
			max = (*p)->FinishTime;
			id = (*p)->id;
		}
	}
	return id;
}

int Activ::succMinSj_id() const {
	int min = LMAX;
	int id = 0;
	for (vector<Activ*>::const_iterator p = succ.begin(); p != succ.end(); ++p) {
		if ((*p)->StartTime < min) {
			min = (*p)->StartTime;
			id = (*p)->id;
		}
	}
	return id;
}

void Activ::ResetTimes() {
	this->StartTime = 0;
	this->FinishTime = 0;
}

ostream &operator <<(ostream &os, const Activ &a) {
	os << "Activ["<< a.id << "]:"
	   << "\n\tselected: " << a.selected
	   << "\n\tSj: " << a.StartTime
	   << "\n\tFj: " << a.FinishTime
	   << "\n\tsucc:(" << a.succ.size() << "):";
	for (vector<Activ*>::const_iterator i = a.succ.begin(); i != a.succ.end(); ++i) os << (*i)->id << " ";
	os << "\n\tpred:(" << a.pred.size() << "):";
	for (vector<Activ*>::const_iterator i = a.pred.begin(); i != a.pred.end(); ++i) os << (*i)->id << " ";
	os << "\n\tmode:" << a.sm;
//	os << "\n\tmodes:(" << a.modes.size() << "):";
//	for (map<int, Mode>::const_iterator i = a.modes.begin(); i != a.modes.end(); ++i) os << i->second << endl;
	return os;
}
