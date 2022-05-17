#ifndef BEACH_LINE_H
#define BEACH_LINE_H

#include "faces.h"
#include "events.h"

typedef struct _arc {

    face* parent; // vertex source
    edge* reverse;
    
    struct _arc* prev;
    struct _arc* next;

    vertex_event* pinch;

} arc;

typedef struct _line {

    face* root;
    arc* head;

    vertex bottom_left_corner;
    vertex top_right_corner;

} line;

line* create_line( vertex ll, vertex tr );

void destroy_line( line* l );

void insert_segment( line* l, face* parent, vertex_list* vlist );

void pinch_off_segment( line* l, vertex_event* v_event );

arc* create_arc( face* parent );

#endif