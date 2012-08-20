#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "defs.h"
#include "RCPSP.h"
#include "Solution.h"
#include "debug.h"
#include "math.h"
#include "unistd.h"

using namespace std;

double T0 = 85; // T° inicial
double ALPHA = 0.9; // factor de enfriamiento
double MAX_ml = 10; // max. numero de soluciones antes de enfriar (markov length)
double MAX_it = 1000; // max. de iteraciones
double MAX_rh = 50;
int SEED = 1234567;

RCPSP inst;

double myrandom() {
	return (float)rand()/(float)RAND_MAX;
}

double prob(int Evbest, int Evn, double Tc) {
	return exp(-(abs(abs(Evbest)-abs(Evn)))/Tc);
}

int main(int argc, char *argv[]) {
	char *tvalue = NULL;
	char *avalue = NULL;
	char *mvalue = NULL;
	char *ivalue = NULL;
	char *rvalue = NULL;
	char *svalue = NULL;
	char *fvalue = NULL;
	int vflag = 0;

	int c;
	opterr = 0;

	while ((c = getopt (argc, argv, "t:a:m:i:r:s:f:v")) != -1)
		switch (c)
		{
		case 't':
			tvalue = optarg;
			break;
		case 'a':
			avalue = optarg;
			break;
		case 'm':
			mvalue = optarg;
			break;
		case 'i':
			ivalue = optarg;
			break;
		case 'r':
			rvalue = optarg;
			break;
		case 's':
			svalue = optarg;
			break;
		case 'f':
			fvalue = optarg;
			break;
		case 'v':
			vflag = 1;
			break;
		case '?':
			if (optopt == 't') fprintf (stderr, "-%c requiere un argumento.\n", optopt);
			else if (optopt == 'a') fprintf (stderr, "-%c requiere un argumento.\n", optopt);
			else if (optopt == 'm') fprintf (stderr, "-%c requiere un argumento.\n", optopt);
			else if (optopt == 'i') fprintf (stderr, "-%c requiere un argumento.\n", optopt);
			else if (optopt == 'r') fprintf (stderr, "-%c requiere un argumento.\n", optopt);
			else if (optopt == 's') fprintf (stderr, "-%c requiere un argumento.\n", optopt);
			else if (optopt == 'f') fprintf (stderr, "-%c requiere un argumento.\n", optopt);
			else if (isprint (optopt))
				fprintf (stderr, "Opción desconocida `-%c'.\n", optopt);
			else
				fprintf (stderr, "Opción desconocida `\\x%x'.\n", optopt);
			return 1;
		default:
			abort ();
		}

	if (tvalue != NULL) T0 = atof(tvalue);
	if (avalue != NULL) ALPHA = atof(avalue);
	if (mvalue != NULL) MAX_ml = atof(mvalue);
	if (ivalue != NULL) MAX_it = atof(ivalue);
	if (rvalue != NULL) MAX_rh = atof(rvalue);
	if (svalue != NULL) SEED = atoi(svalue);
	else SEED = time(NULL);
	srand((unsigned)SEED);

	printf("T0 ALPHA Mml Mit Mrh SEED\n");
	printf("%f %f %f %f %f %d\n", T0, ALPHA, MAX_ml, MAX_it, MAX_rh, SEED);

	if(fvalue == NULL || !inst.readFile(fvalue)) {
		printf("No se pudo leer archivo %s\n", fvalue);
		abort();
	}
	inst.setCPM();

	// initialize sbest
	Solution Sn, Sc, Sbest;
	Sbest = Sc = inst.feasibleSolution();

	// initialize evals
	int Evn, Evc, Evbest;
	Evbest = Evc = inst.eval(Sc);

	// initialize params
	double it = 0;	// iteraciones
	double ml = 0;	// largo de cadena
	double rh = 0;	// reheat
	double Tc = T0;	// temperatura
	char w = '0';	// si aceptó peor solución
	const double MAX_rhml = MAX_rh * MAX_ml;// reheat si no hay mejora en
											//MAX_rh cadenas = MAX_rh * MAX_ml iteraciones

	if (vflag) printf("it;ml;rh;Tc;w;Evn;Evc;Evbest\n");

	// mientras no se alcance max de iteraciones
	while (it < MAX_it) {
		it++;	// iteraciones totales
		ml++;	// iteraciones dentro de una temperatura (cadena de markov)
		w = '0';

		if (ml >= MAX_ml) {	// disminuir temperatura ?
			Tc = ALPHA*Tc;
			ml = 0;
		}
		if (rh > MAX_rhml) { // reheat ?
			Tc = T0;
			rh = 0;
		}

		Sn = inst.neighbour(Sc.a);	// generar nuevo vecino
		Evn = inst.eval(Sn);		// evaluar vecino
		if (Evn < Evc) {
			Sc = Sn; Evc = Evn;		// si es mejor, aceptar como current
			rh = 0;					// hay mejora, reheat vuelve a 0
		} else {
			rh++;					// no hay mejora
			if ( myrandom() < prob(Evc, Evn, Tc)) { // aceptar según prob.
				Sc = Sn; Evc = Evn;
				w = '1';
			}
		}

		if (Evc < Evbest) { // guardar el mejor de todas las it.
			Sbest = Sc;
			Evbest = Evc;
		}


		if (vflag) printf("%.0f;%.0f;%.0f;%.0f;%c;%d;%d;%d\n",it,ml,rh,Tc,w,Evn,Evc,Evbest);
	}

	cout << "Evbest: " << Evbest << endl;
	cout << "Sbest: " << Sbest << endl;

return 0;
}
