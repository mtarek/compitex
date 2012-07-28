#include "parameter.h"

int Parameter_eq(void *d1, void *d2) {
	Parameter *p1 = (Parameter *)d1;
	Parameter *p2 = (Parameter *)d2;
	
	if(!p1 || !p2)
		return 0;
	
	if(!strcmp(p1->identifier, p2->identifier))
		return 1;
	
	return 0;
}

Parameter * Parameter_build(unsigned int type, char *id) {
	Parameter *p;
	p = calloc(1, sizeof(Parameter));
	p->identifier = strdup(id);
	p->type = type;
	
	return p;
}