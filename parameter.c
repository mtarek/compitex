/***************************************************************************
 *  
 *                    ___                _ _____   __  __
 *                   / __|___ _ __  _ __(_)_   _|__\ \/ /
 *                  | (__/ _ \ '  \| '_ \ | | |/ -_)>  < 
 *                   \___\___/_|_|_| .__/_| |_|\___/_/\_\
 *                                 |_|                  
 *
 * Copyright (C) 2012 - 2017, Mohamed Tarek El-Haddad <mtarek16@gmail.com>.
 *
 * This software is licensed as described in the LICENSE file, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
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