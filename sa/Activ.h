#ifndef ACTIV_H
#define ACTIV_H

#include <iostream>
#include <vector>
#include <map>
#include "defs.h"
#include "Mode.h"

using namespace  std;

class Mode;

class Activ{
public:
	int id;							// id
	int nsucc;						// # de sucesores
	vector<Activ*> succ, pred;		// punteros a sucesores y predecesores
	int nmodes;						// # de modos
//	map<int, Mode> modes;			// cont. de modos (MM)
	Mode sm;						// cont. de modo (SP)
//	int duration, ES, EF, LS, LF;	// info del único modo (para SM)
	bool selected;					// si está seleccionada en una representación de la solución

	int StartTime;							// t de inicio
	int FinishTime;							// t de termino

    Activ();

	bool isPrecedenceElegible();
	int predMaxFj() const;					// obtiene el tiempo Fj mayor de los predecesores
	int succMinSj() const;					// obtiene el tiempo Sj menor de los sucesores
	int predMaxFj_id() const;					// obtiene el tiempo Fj mayor de los predecesores
	int succMinSj_id() const;					// obtiene el tiempo Sj menor de los sucesores

	void ResetTimes();
};

ostream &operator <<(ostream &os, const Activ &a);

#endif // ACTIV_H
