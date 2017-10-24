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
typedef enum { typeCon, typeId, typeOpr } nodeEnum;

/* constants */
typedef struct {
	double value; /* value of constant */
} conNodeType;

/* identifiers */
typedef struct {
	char* s;
} idNodeType;

/* operators */
typedef struct {
	int oper; /* operator */
	int nops; /* number of operands */
	struct nodeTypeTag *op[1]; /* operands (expandable) */
} oprNodeType;

typedef struct nodeTypeTag {
	nodeEnum type; /* type of node */
	/* union must be last entry in nodeType */
	/* because operNodeType may dynamically increase */
	union {
		conNodeType con; /* constants */
		idNodeType id; /* identifiers */
		oprNodeType opr; /* operators */
	};
} nodeType;
extern int sym[26];
extern int lineno;