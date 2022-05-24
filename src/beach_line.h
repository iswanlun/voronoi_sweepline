#ifndef BEACH_LINE_H
#define BEACH_LINE_H

#include "events.h"

typedef struct _arc {

    face* parent; // vertex source
    edge* reverse;
    
    struct _arc* prev;
    struct _arc* next;

    vertex_event* pinch;

} arc;

typedef struct _line {

    arc* head;

} line;

line* create_line( void );

void destroy_line( line* l );

void site_event( line* l, face* parent, vertex_list* vlist );

void circle_event( line* l, vertex_event* v_event );

arc* create_arc( face* parent );

#endif