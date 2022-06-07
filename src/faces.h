#ifndef FACES_H
#define FACES_H

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

face_list* create_face_list( vertex* sites, int size, vertex ll, vertex tr );

face* peek_next_face( face_list* list );

face* pop_next_face( face_list* list );

void destroy_face_list( face_list* list );

void remove_face_edges( face* f );

edge* create_edge( face* parent );

void bound_faces( face_list* list, vertex ll, vertex tr );

#endif