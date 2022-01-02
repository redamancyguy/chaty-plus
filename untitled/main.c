#include <stdio.h>
#include "stdlib.h"

struct Node {
    struct Node *next;
    int data;
};



//0 head-main
//1 headP = 0 (&head-main)
//2 head-create
//3 head-create = malloc (0x2197261937)
//4 *headP    -> 0 head 0x2197261937


void Create(struct Node **headP) {
    struct Node *head = NULL;
    while (1) {
        int temp;
        scanf("%d", &temp);
        if (temp < 0) {
            break;
        }
        struct Node *tempP = (struct Node *) malloc(sizeof(struct Node));
        tempP->next = NULL;
        tempP->data = temp;
        tempP->next = head;
        head = tempP;
    }
    *headP = head;
}

void Show(struct Node *head) {
    for (struct Node *i = head; i != NULL; i = i->next) {
        printf("%d\n", i->data);
    }
}

void Delete(struct Node *head) {
    for (struct Node *i = head; i != NULL;) {
        struct Node *temp = i->next;
        free(i);
        i = temp;
    }
}

void Sort1(struct Node **headP) {
    struct Node *OldHead = *headP;
    struct Node *NewHead = NULL;
    for (struct Node *i = OldHead; i != NULL;) {
        Show(i);
        puts("=====================");
        Show(NewHead);
        puts("**********************");
        struct Node *nextI = i->next;
        if (NewHead == NULL) {
            puts("NULL");
            NewHead = i;
            NewHead->next = NULL;
        } else {
            struct Node *temp = NewHead, *last = NewHead;
            if(temp->data > i->data){
                puts("HEAD");
                i->next = temp;
                NewHead = i;
            }else{
                puts("ELSE");
                temp = temp->next;
                while (temp != NULL) {
                    if (temp->data > i->data) {
                        i->next = temp;
                        last->next = i;
                        break;
                    }
                    last = temp;
                    temp = temp->next;
                }
                if(last->next == NULL){
                    last->next = i;
                    i->next = NULL;
                }
            }
        }
        i = nextI;
    }
    *headP =  NewHead;
}

void Sort(struct Node **headP) {
    struct Node *OldHead = *headP;
    for (struct Node *i = OldHead; i->next != NULL; i = i->next) {
        for (struct Node *j = i->next; j != NULL; j = j->next) {
            if (i->data > j->data) {
                int temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }
    struct Node *NewHead = NULL;
}

int main() {
    struct Node *head = NULL;
    Create(&head);
    Sort1(&head);
    Show(head);
    Delete(head);
    return 0;
}
//132 2  314 3 32  134 2413 4 -1