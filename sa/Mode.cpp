#include "Mode.h"

Mode::Mode() {
}

Mode::Mode(int duration, int ES, int EF, int LS, int LF) {
	this->duration = duration;
	this->ES = ES;
	this->EF = EF;
	this->LS = LS;
	this->LF = LF;
}

int Mode::Slack() const {
	return (LS - ES);
}

ostream &operator <<(ostream &os, const Mode &m) {
	os << "Mode duration:" << m.duration << " ES:" << m.ES << " EF:" << m.EF
	   << " LS:" << m.LS << " LF:" << m.LF << " SLK:" << m.Slack();
	for (map<int, int>::const_iterator j = m.req.begin(); j != m.req.end(); ++j) {
		os << "\n\t\tR " << j->first << ": " << j->second;
	}
	return os;
}
