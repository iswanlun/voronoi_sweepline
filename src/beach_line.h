#ifndef BEACH_LINE_H
#define BEACH_LINE_H

#include "events.h"

typedef struct _arc {

    face* parent; // vertex source
    edge* reverse;
    
    struct _arc* prev;
    struct _arc* next;

    vertex_event* pinch;

    vertex (*solve) (struct _arc*, float);              // self, s
    float (*eval) (struct _arc*, float, float );        // self, s, x
    float (*diff) (struct _arc*, float, float, float);  // self, x, s, y

} arc;

typedef struct _line {

    arc* head;

    vertex bottom_left_corner;
    vertex top_right_corner;

    face* bounds[4];

} line;

line* create_line( vertex ll, vertex tr );

void destroy_line( line* l );

void insert_segment( line* l, face* parent, vertex_list* vlist, float s );

void pinch_out_segment( line* l, vertex_event* v_event );

arc* create_arc( face* parent );

#endif