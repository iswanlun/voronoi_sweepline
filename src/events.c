#include <stdlib.h>
#include "events.h"
#include "stddef.h"

vertex_event* merge_list( vertex_event* left, vertex_event* right ) {

    vertex_event* head;

    if ( left->sweep_y > right->sweep_y ) {
        head = left;
        left = left->next;
    } else {
        head = right;
        right = right->next;
    }

    vertex_event* tail = head;

    while ( left != NULL && right != NULL ) {

        if ( left->sweep_y > right->sweep_y ) {          
            tail->next = left;
            tail = left;
            left = left->next;

        } else {
            tail->next = right;
            tail = right;
            right = right->next;
        }
    }

    if ( left != NULL ) {
        tail->next = left;

    } else {
        tail->next = right;
    }

    return head;
}

vertex_event* sort_vertex_list( vertex_event* head, int length ) {

    if ( length < 2 ) {
        return head;

    } else {
        vertex_event* left = head;
        int len = (length/2);

        for (int i = 1; i < len; i++ ) {
            head = head->next;
        }

        vertex_event* right = head->next;
        head->next = NULL;

        left = sort_vertex_list(left, len);
        right = sort_vertex_list(right, length - len);

        return merge_list( left, right );
    }
}

void insert_vertex_event( vertex_list* list, vertex_event** event, float v_x, float v_y, float sweep_y ) {

    if ( (*event) == NULL ) {
        (*event) = (vertex_event*) malloc( sizeof(vertex_event) );
        (*event)->next = list->head;
        list->head = (*event);
        list->length++;
    }

    (*event)->sweep_y = sweep_y;
    (*event)->v_site.x = v_x;
    (*event)->v_site.y = v_y;
    
    list->head = sort_vertex_list( list->head, list->length );
}

vertex_event* next_vertex_event( vertex_list* list ) {

    vertex_event* tmp = list->head;
    list->head = list->head->next;
    list->length--;
    return tmp;
}

vertex_event* peek_vertex_event( vertex_list* list ) {

    return list->head;
}

void null_vertex_event( vertex_list* list, vertex_event** dest ) {

    if ( *dest != NULL ) {

        vertex_event* prev = NULL;
        vertex_event* index = list->head;

        while ( index != *(dest) ) {
            prev = index;
            index = index->next;
        }

        if (prev != NULL) {
            prev->next = index->next;
        } else {
            list->head = index->next;
        }

        free(index);
        *dest = NULL;
        list->length--;
    }
}

vertex_list* create_vertex_list( void ) {

    vertex_list* list = (vertex_list*) malloc( sizeof(vertex_list) );

    list->head = NULL;
    list->length = 0;

    return list;
}

void dispose_vertex_list( vertex_list* list ) {

    while( list->head != NULL ) {
        vertex_event* tmp = list->head->next;
        free(list->head);
        list->head = tmp;
    }

    free(list);
}