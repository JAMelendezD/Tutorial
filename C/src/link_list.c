#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};

typedef struct node node_l;

void printlist(node_l *head) {
    node_l *tmp = head;
    int counter = 1;
    while (tmp != NULL) {
        printf("%5d (%p) -> ", tmp->value, tmp->next);
        tmp = tmp->next;
        if (counter % 4 == 0) {
            printf("\n");
        }
        counter++;
    }
    printf("\n");
}

node_l *create_new_node(int value) {
    node_l *result = malloc(sizeof(node_l));
    result->value = value;
    result->next = NULL;
    return result;
}

void insert_at_head(node_l **head, node_l *node_to_insert) {
    node_to_insert->next = *head;
    *head = node_to_insert;
}

node_l *find_node(node_l *head, int value) {
    node_l *tmp = head;
    while (tmp != NULL) {
        if (tmp->value == value) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

void insert_after_node(node_l *node_to_insert_after, node_l *new_node) {
    new_node->next = node_to_insert_after->next;
    node_to_insert_after->next = new_node;
}

void remove_node(node_l **head, node_l *node_to_remove) {
    node_l *tmp = *head;
    node_l *prev = *head;
    if (*head == node_to_remove) {
        *head = node_to_remove->next;
        free(node_to_remove);
        return;
    }

    while (tmp != NULL && tmp != node_to_remove) {
        prev = tmp;
        tmp = tmp->next;
    }

    if (tmp == NULL) {
        return;
    }

    prev->next = tmp->next;
    free(node_to_remove);
}

int main(int argc, char **argv) {
    node_l *head = NULL;
    node_l *tmp;

    for (int i = 0; i < 10; i++) {
        tmp = create_new_node(i);
        insert_at_head(&head, tmp);
    }

    tmp = find_node(head, 5);
    printf("Found node with value %p\n", tmp);
    printlist(head);
    insert_after_node(tmp, create_new_node(6000));
    printlist(head);
    remove_node(&head, tmp);
    printlist(head);
}