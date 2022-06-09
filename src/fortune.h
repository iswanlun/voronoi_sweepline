#ifndef FORTUNE_H
#define FORTUNE_H

typedef struct vertex {

    float x, y;

} vertex;

struct face;

typedef struct edge {

    vertex origin; // clockwise

    struct edge* next;
    struct edge* twin;

    struct face* home;

} edge;

typedef struct face {

    vertex site;
    edge* top_edge;

} face;

typedef struct face_list {

    face** collection;

    int size;
    int index;

    face* bounds[4];

} face_list;

face_list* fortunes_sweep_line( vertex* sites, int size, vertex ll, vertex tr );

void destroy_face_list( face_list* list );

#endif