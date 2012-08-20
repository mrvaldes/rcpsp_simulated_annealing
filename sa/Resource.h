#ifndef RESOURCE_H
#define RESOURCE_H
#include <iostream>
#include <vector>
#include "stdio.h"
#include "Activ.h"
#include "debug.h"


using namespace std;

class Activ;

class Resource
{
public:
	int id, max;
	vector< vector<int> > use; // rep. del uso del recurso en el tiempo
	Resource();
	Resource(int id, int max);
	int tiempoParaActividad(Activ &j);
	int available(int t);	// q disponible en tiempo t
	int findSpace(int t, int r, int d);	// buscar t con espacio para el req.
										// con r = r_jk y duración = d_j, desde t en adelante
	void assign(int t, int id, int req, int d); // asigna desde t, requ de activ. id, con req r y duracion d
	bool assignUnit(int t, int id); // intenta asignar 1 ud. de actividad id en t, retorna falso si falla
	int remove(int id);	// remueve acitividad id
	void ResetUse(int tMax);
};

ostream &operator <<(ostream &os, const Resource &r);

#endif // RESOURCE_H
