#include "list.h"

void insert(LIST *l,char input []){
  NODE *p = (NODE*)malloc(sizeof(NODE));
  char *value  = (char*)malloc(sizeof(char)*(strlen(input)+1));
  strcpy(value,input);
  p-> val = value;
  p-> mac = 0;
  p-> ext= 0; 
  p-> ent= 0;
  p-> labDec = 0;
  p-> next = l-> head;
  l-> head = p;
  return;
}

int has(LIST *l ,char input []){
  NODE *p = l->head;
  while(p != NULL){
    if(!strcmp(p->val,input)){
      return 1;
    }
    p = p->next;
  }
  return 0;
}

LIST* newList(){
  LIST *l = (LIST*)malloc(sizeof(LIST));
  return l;
}
void printList(LIST *l){
  NODE *p = l->head;
  //printf("\nThe list %s is:",name);
  while(p != NULL){
    printf(" name: %s, mac: %d, ext: %d,  ent: %d, labDec: %d\n",p->val,p->mac,p->ext,p->ent,p->labDec);
    p = p->next;
  }
  printf("\n");
}

NODE* getNode(LIST *l, char name[]){
  NODE *p = l->head;
  while(p != NULL){
    //printf("%s\n",p->val);
    if(!strcmp(p->val,name)){
      return p;
    }
    p = p->next;
  }
  return p;
}


/*
int getMac(LIST *l,char name []){
  NODE *p = l->head;
  while(p != NULL){
    //printf("%s\n",p->val);
    if(!strcmp(p->val,name)){
      return p->mac;
    }
    p = p->next;
  }
  return 0;
}

int getExt(LIST *l,char name []){
  NODE *p = l->head;
  while(p != NULL){
    //printf("%s\n",p->val);
    if(!strcmp(p->val,name)){
      return p->mac;
    }
    p = p->ext;
  }
  return 0;
}

int getEnt(LIST *l,char name []){
  NODE *p = l->head;
  while(p != NULL){
    //printf("%s\n",p->val);
    if(!strcmp(p->val,name)){
      return p->mac;
    }
    p = p->ent;
  }
  return 0;
}

int getLabDec(LIST *l,char name []){
  NODE *p = l->head;
  while(p != NULL){
    //printf("%s\n",p->val);
    if(!strcmp(p->val,name)){
      return p->labDec;
    }
    p = p->ext;
  }
  return 0;
}
*/

/* void freeList(LIST*) */
