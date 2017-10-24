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
typedef struct List List;
typedef struct Node Node;

struct Node
{
	void *data;
	Node *next;
};

struct List
{
	Node *head;
	Node *tail;

	unsigned int size;
};

void List_init(List *l, int (*eq) (void *d1, void *d2));
void List_add(List *l, void *data);
void List_addonce(List *l, void *data);
void List_rm(List *l, void *data);
void List_destroy(List *l);