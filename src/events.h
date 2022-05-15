#ifndef EVENTS_H
#define EVENTS_H

typedef struct _vertex_event {

    float sweep_y;

    struct _vertex_event* next;    

} vertex_event;

typedef struct _vertex_list {

    vertex_event* head;

} vertex_list;

vertex_event* vertex_insert_event( vertex_list* list, vertex_event* current, float sweep_y );

void vertex_next_event( vertex_list* list, vertex_event* destination );

vertex_list* vertex_create_list( void );

void vertex_dispose_list( vertex_list* list );

#endif