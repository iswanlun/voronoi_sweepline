#ifndef BEACH_LINE_H
#define BEACH_LINE_H

#include "events.h"

typedef struct arc {

    face* parent; // vertex source
    edge* reverse;
    
    struct arc* prev;
    struct arc* next;

    vertex_event* pinch;

} arc;

typedef struct line {

    arc* head;
    int bias;

    vertex top_right;
    vertex lower_left;

} line;

line* create_line( vertex ll, vertex tr );

void destroy_line( line* ln );

void site_event( line* ln, face* parent, vertex_list* vlist );

void circle_event( line* ln, vertex_event* v_event, vertex_list* vlist );

#endif