#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<assert.h>
#define NBUCKETS 5
#include "mydatabase.h"

typedef struct Tuple {
  int nAttr;
  char **attr;
  char *key;
  Tuple *next;
} Tuple;
typedef struct Tuple* List;
typedef List Hashtable[NBUCKETS];

typedef struct Node* Tree;

struct Node {
  char name[30];
  List toTuple; /*really a pointer to a tuple*/
  Tree lc;
  Tree rc;
} Node;

Tuple tuple_new(char *key, int nAttr, ...) {
  va_list valist;
  va_start(valist, nAttr); // initialize valist for nAttr number of arguments

  Tuple *t = (Tuple*)malloc(sizeof(Tuple));

  t->nAttr = nAttr;
  t->key = key;
  t->next = NULL;
  t->attr = malloc(sizeof(char*) * nAttr);

  for(int i = 0; i< nAttr; i++) {
    t->attr[i] = va_arg(valist, char*);
  }

  va_end(valist);
  return *t;

}

int matchSpecs(Tuple ct, Tuple lt) {
	if(ct.nAttr != lt.nAttr) {
		printf("Tuples have different length so cannot compare specs\n");
	} else {
		for(int i=0; i<ct.nAttr; i++) {
			if(strcmp(ct.attr[i], "*") && strcmp(ct.attr[i], lt.attr[i])) {
				printf("The specs don't match\n");
				return 0;
			}
		}
		return 1;
	}
}

int hash(char* key) {

  int sum = 0;
  for (int i = 0; key[i] != '\0'; i++) {
    sum += key[i];
  }

  return sum % NBUCKETS;
}

int keyExists(char* key, List *l) {
	Tuple *curr = *l;
	while(curr != NULL) {
		if(!strcmp(key, (*l)->key))
			return 1;
		curr = curr->next;
	}

	return 0;
}



void bucketInsert(Tuple t, List *l) {

  if(*l == NULL) {
    *l = (List) malloc(sizeof(Tuple));
    *l[0] = t;
    (*l)->next = NULL;
  } else {
      bucketInsert(t, &((*l)->next));
  }
}

void insert(Tuple t, Hashtable h) {
  //printf("Inserting tuple with key %s into this index: %d\n", t.key, hash(t.key));
  if(keyExists(t.key, &h[hash(t.key)])) {
	  printf("cannot insert because key already exists\n");
  } else {
	  printf("inserting...\n");
	  bucketInsert(t, &(h[hash(t.key)]));
  }

}

/*int cmpTuple(Tuple a, Tuple b) {
	if(a.nAttr == b.nAttr && a.key == b.key) {
		for(int i=0; i< a.nAttr; i++) {
			//returns false if any of the attributes do not match
			if(a.attr[i] != b.attr[i]) {
				return 0;
			}
		}
		//if the above condition is
		//never satisfied in the for loop, we return 1
		return 1;
	} else {
		printf("Number of attributes or keys don't match\n");
	}

	return 0;

}*/

void printTuple(Tuple t){
	printf("tuple(%s, %d, %s, %s, %s)\n",t.key, t.nAttr, t.attr[0], t.attr[1], t.attr[2]);
}

/*
void filePrintTuple(Tuple t, char *filename){
    FILE *fp;
    fp = fopen(filename, "a");
    fseek(fp, 0, SEEK_END); // goto end of file
 {
     if (fp == NULL) {
        perror("Can't open file");
    } else if (ftell(fp) == 0)
      {
 fprintf(fp, "tuple(%s, %d, %s, %s, %s)\n",t.key, t.nAttr, t.attr[0], t.attr[1], t.attr[2]);}
 fclose(fp);
 }}*/

void printTable(Hashtable h) {
	for(int i=0; i<NBUCKETS; i++) {
		Tuple *curr = h[i];
		while(curr != NULL) {
			printTuple(*curr);
			curr = curr->next;
		}
	}
}

//fixed
void filePrintTable(Hashtable h, char *filename) {
    FILE *fp;
    fp = fopen(filename, "a");
    fseek(fp, 0, SEEK_END); // go to end of file
    if (ftell(fp) == 0){//if file is empty
	for(int i=0; i<NBUCKETS; i++) {
		Tuple *curr = h[i];
		while(curr != NULL) {
			//filePrintTuple(*curr, filename);
 {
     if (fp == NULL) {
        perror("Can't open file");
    } else
      {
 fprintf(fp, "tuple(%s, %d, %s, %s, %s)\n", curr->key, curr->nAttr, curr->attr[0], curr->attr[1], curr->attr[2]);}



void bucketDelete(Tuple t, List *head) {
	if(*head == NULL) {
		printf("Cannot delete from empty list\n");
	} else {
		Tuple *curr = *head;
		Tuple *trail = NULL;

		while(curr != NULL) {
			if(matchSpecs(t, *curr)) {
				break;
			} else {
				trail = curr;
				curr = curr->next;
			}
		}
		//case 2 not found tuple
		if(curr == NULL) {
			printf("tuple was not found\n");
		} else {
			//case 3 delete from head node
			if(*head == curr) {
				*head = (*head)->next;

		} else {//case 4 delete beyond head
			trail->next = curr->next;

		}
		free(curr);
		}

	}
}


void bucketLookup(Tuple t, List *head) {
	if(*head == NULL) {
		printf("Cannot lookup from empty list\n");
	} else {
		Tuple *curr = *head;
		Tuple *trail = NULL;

		while(curr != NULL) {
			if(matchSpecs(t, *curr)) {
				printTuple(t);
				break;
			} else {
				trail = curr;
				curr = curr->next;
			}
		}
		//case 2 not found tuple
		if(curr == NULL) {
			printf("tuple was not found\n");
		}
		free(curr);
		}

	}


void tupleDelete(Tuple t, Hashtable h ) {
	printf("deleting...\n");
	bucketDelete(t, &h[hash(t.key)]);
}

void lookup(Tuple t, Hashtable h){
    printf("finding tuple...\n");
    bucketLookup(t, &h[hash(t.key)]);
}


int main(void) {
  Hashtable csg;

  for(int i = 0; i< NBUCKETS; i++) {

    csg[i] = NULL;
  }


  Tuple c = tuple_new("John Lennon", 3, "CSC282", "John Lennon", "A-");
  insert(c, csg);

  Tuple a = tuple_new("Luke Skywalker", 3,"CSC173", "Luke Skywalker", "A");
  insert(a, csg);

  /*Tuple x = tuple_new("Luke Skywalker", 3,"*", "*", "*");
  insert(x, csg);*/

  /*Tuple b = tuple_new("Luke Skywalker", 3,"MTH201", "Luke Skywalker", "C-");
  insert(b, csg);*/

  Tuple d = tuple_new("George Eastman", 3, "MUR110", "George Eastman", "A+");
  insert(d, csg);

  Tuple e = tuple_new("George East", 3, "*", "*", "*");
    insert(e,csg);

  Tuple f = tuple_new("George Eastman", 3, "*", "*", "*");

  //printf("a ?= d %d", matchSpecs(e,a));
  printTable(csg);
  //lookup(f,csg);
  //tupleDelete(e,csg);
  //lookup(e,csg);
  //lookup(f,csg);
  filePrintTable(csg,"Test0.txt");
  tupleDelete(f,csg);
  //lookup(d,csg);
  //lookup(f,csg);
  //lookup(e,csg);
  //printTable(csg);

  filePrintTable(csg,"Test.txt");//working now


  return 0;
}
