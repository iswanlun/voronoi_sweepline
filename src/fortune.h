#ifndef FORTUNE_H
#define FORTUNE_H

typedef struct vertex {

    float x, y;

} vertex;

struct face;

typedef struct edge {   /* This is really a half edge. */

    vertex origin;      /* edges are connected one to the next _clockwise_. */

    struct edge* next;  /* The next edge in this face. */
    struct edge* twin;  /* The edge of the face adjacent to this edge, pointing in the opposite direction. Will be NULL for perimeter edges.*/

    struct face* home;  /* The face this edge belongs to. */

} edge;

typedef struct face {

    vertex site;        /* The coordinates of this faces site. */
    edge* top_edge;     /* The first edge of the edges defining this face. */

} face;

typedef struct face_list {

    face** collection;  /* This is the doubly connected edge list. */
    int size;           /* The number of faces in collection. */

    int index;          /* Invalid paramerter after return, do not use. */
    face* bounds[4];    /* Invalid paramerter after return, do not use. */

} face_list;

/* Creates a doubly connected edge list */
face_list* fortunes_sweep_line( vertex* sites, int size, vertex ll, vertex tr );

/* Frees all memory allocated by fortunes_sweep_line. */
void destroy_face_list( face_list* list );

#endif