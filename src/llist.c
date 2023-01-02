#include <stdio.h>
#include <string.h>

typedef struct node{
    void* value;
    struct node* next;
} Node;


/**
 * value null
 * return head of linked list
 * */
struct node* init_list(){

    struct node* head = (struct node*)malloc(sizeof(struct node));
    memset(head,0,sizeof(struct node));
    head->next = NULL;

    return head;
}
/**
 * adds a new node to linked list with value value
 * */
void pushstr(struct node* nodes,void* value){
    struct node* pNode = nodes;
    /* printf("push %s\n",value); */
    while(pNode->next != NULL){
        pNode = pNode->next;
    }
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    /* memset(newNode,0,sizeof(struct node)); */
    newNode->next = NULL;
    
    newNode->value = malloc(sizeof(value)+1);
    memset(newNode->value,0,sizeof(value)+1);
    
    strcpy(newNode->value,value);

    pNode->next = newNode;
}
void push(struct node* nodes,void* value){
    struct node* pNode = nodes;
    /* printf("push %s\n",value); */
    while(pNode->next != NULL){
        pNode = pNode->next;
    }
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    /* memset(newNode,0,sizeof(struct node)); */
    newNode->next = NULL;
    
    /* newNode->value = malloc(sizeof(value)+1); */

    newNode->value = value;

    pNode->next = newNode;
}
void insert(struct node* nodes, void* value, int index){
    struct node* pNode = nodes;
    /* printf("push %s\n",value); */
    int i =0;
    struct node* prev=NULL;
    while(pNode->next != NULL){
        if(i == index) break;
        prev = pNode;
        pNode = pNode->next;
        i++;
    }
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    /* memset(newNode,0,sizeof(struct node)); */
    newNode->next = pNode->next;
    
    /* newNode->value = malloc(sizeof(value)+1); */

    newNode->value = value;
    pNode->next = newNode;


}
void printList(struct node *head,const char* sep){

    struct node* pNode = head;
    while(pNode->next != NULL){
        pNode = pNode->next;
        printf("%s",(char*)pNode->value);
        printf("%s",sep);
    }
    printf("\n");
}
void printListf(struct node *head,const char* sep){

    struct node* pNode = head;
    while(pNode->next != NULL){
        pNode = pNode->next;
        printf("%f",*(float*)pNode->value);
        printf("%s",sep);
    }
    printf("\n");
}
int listLen(struct node* head){
    int len = 0;
    struct node* pNode = head;
    while(pNode->next != NULL){
        pNode = pNode->next;
        len ++;
    }
    return len;
}
struct node* get(struct node* head, int i){
    int n = 0; 
    struct node* pNode = head;
    while(pNode->next != NULL){
        pNode = pNode->next;
        if(n == i) return pNode;
        n++;
    }
    return NULL;
}
void removeNode(struct node* head, int i){
    int n = 0; 
    struct node* pNode = head;
    struct node* prev = NULL;
    while(pNode->next != NULL){
        prev = pNode;
        pNode = pNode->next;
        if(n == i) {
            prev->next = pNode->next;
            pNode = NULL;
            return;
        };
        n++;
    }
}
/**
 * return last element
 **/
struct node* peek(struct node* head){
    struct node* pNode = head;
    struct node* prev = NULL;
    while(pNode->next != NULL){
        prev = pNode;
        pNode = pNode->next;
    }
    return pNode;
}
/**
 * return last element and removed is
 **/
struct node* pop(struct node* head){
    struct node* pNode = head;
    struct node* prev = NULL;
    while(pNode->next != NULL){
        prev = pNode;
        pNode = pNode->next;
    }
    if(prev != NULL){
        //free(prev->next);
        prev->next = NULL;
    }
    return pNode;

}
