#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define HASHSIZE 1001

typedef struct _node{
	char *word;
	int number;
	struct _node *next;
}node;

static node* hashtab[HASHSIZE];

void inithashtab(){
	int i;
	for(i = 0; i < HASHSIZE; i++){
		hashtab[i] = NULL;
	}
}

unsigned int hash(char *word){
	unsigned int h = 0;
	for(; *word; word++){
		h = *word + h*31;
	}
	return h%HASHSIZE;
}

node* lookup(char *word){
	unsigned int hi = hash(word);
	node* np = hashtab[hi];
	for(; np != NULL; np = np->next){
		if(!strcmp(np->word, word)){
			return np;
		}
	}
	
	return NULL;
}

char* m_strdup(char *word){
	int l = strlen(word) + 1;
	char *ns = (char*)malloc(l*sizeof(char));
	strcpy(ns, word);
	if(ns == NULL)
		return NULL;
	else
		return ns;
}

int get(char *word){
	node *n = lookup(word);
	if(n == NULL)
		return 0;
	else
		return n->number;
}

int put(char *word){
	unsigned int hi;
	node *np;
	if((np = lookup(word)) == NULL){
		hi = hash(word);
		np = (node *)malloc(sizeof(node));
		if(np == NULL){
			return 0;
		}
		np->word = m_strdup(word);
		if(np->word == NULL)
			return 0;
		np->next = hashtab[hi];
		np->number = 1;
		hashtab[hi] = np;
	}else{
		np->number = np->number + 1;
	}
	return 1;
}

void displaytable(){
	int i;
	node *t;
	for(i = 0; i < HASHSIZE; i++){
		if(hashtab[i] != NULL){
			t = hashtab[i];
			printf("(");
			for(; t != NULL; t= t->next)
				printf("(%s.%d) ", t->word, t->number);
			printf(".)\n");
		}
		
	}
}

