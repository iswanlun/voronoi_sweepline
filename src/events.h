#ifndef EVENTS_H
#define EVENTS_H

#include "faces.h"

typedef struct vertex_event {

    vertex v_site;
    float sweep_y;

    struct vertex_event* next;    

} vertex_event;

typedef struct vertex_list {

    vertex_event* head;
    int length;

} vertex_list;

void insert_vertex_event( vertex_list* list, vertex_event** event, float v_x, float v_y, float sweep_y );

vertex_event* peek_vertex_event( vertex_list* list );

vertex_event* next_vertex_event( vertex_list* list );

void null_vertex_event( vertex_list* list, vertex_event** dest );

vertex_list* create_vertex_list( void );

void dispose_vertex_list( vertex_list* list );

#endif