#include <stdlib.h>
#include <stdio.h>

struct LinkedList {
    int value;
    struct LinkedList* nextLink;
} typedef LinkedList;

struct ArrayWithLength {
    int length;
    int* value;
} typedef ArrayWithLength;

LinkedList* linkedListFromArrayWithLength(ArrayWithLength a) {
    LinkedList* head = malloc(sizeof(LinkedList));
    head->value = 0;
    head->nextLink = NULL;
    LinkedList* current = NULL;
    if (a.length > 0) {
        head->value = a.value[0];
        current = head;
    }
    for (int i = 1; i < a.length; ++i) {
        LinkedList* next = malloc(sizeof(LinkedList));
        next->value = a.value[i];
        next->nextLink = NULL;
        current->nextLink = next;
        current = next;
    }
    return head;
}

void printLinkedList(LinkedList* ptr) {
    if (ptr == NULL) { printf("NULL list"); }
    LinkedList* current = ptr;
    while (current != NULL) {
        printf("%i ", current->value);
        current = current->nextLink;
    }
    printf("\n");
}

int accessLinkedListValue(LinkedList* ptr, int index) {
    LinkedList* current = ptr;
    for (int i = 0; i < index; i++) {
        current = current->nextLink;
    }
    return current->value;
}

void setLinkedListValue(LinkedList* ptr, int index, int value) {
    if (ptr == NULL) { return; }
    LinkedList* current = ptr;
    for (int i = 0; i < index; i++) {
        current = current->nextLink;
    }
    current->value = value;
}

void deleteLinkedList(LinkedList* ptr) {
    if (ptr->nextLink != NULL) {
        deleteLinkedList(ptr->nextLink);
    }
    free(ptr);
}

LinkedList* addHead(LinkedList* ptr, int newValue) {
    LinkedList* newHead = malloc(sizeof(LinkedList));
    newHead->value = newValue;
    newHead->nextLink = ptr;
    return newHead;
}

LinkedList* addLast(LinkedList* ptr, int newValue) {
    LinkedList* current = ptr;
    while (current->nextLink != NULL) {
        current = current->nextLink;
    }
    LinkedList* newLast = malloc(sizeof(LinkedList));
    newLast->value = newValue;
    newLast->nextLink = NULL;
    current->nextLink = newLast;
    return ptr;
}

int main() {
    int values[5];
    for (int i = 0; i < 5; i++) {
        values[i] = i + 1;
    }
    ArrayWithLength arr = {5, values};
    LinkedList* x = linkedListFromArrayWithLength(arr);
    printLinkedList(x);

    setLinkedListValue(x, 2, 100);
    printLinkedList(x);

    x = addHead(x, 0);
    printLinkedList(x);

    x = addLast(x, 6);
    printLinkedList(x);
}