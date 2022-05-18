#ifndef FACES_H
#define FACES_H

typedef struct _vertex {

    float x, y;

} vertex;

typedef struct _edge {

    vertex origin; // clockwise

    struct _edge* next;
    struct _edge* twin;

} edge;

typedef struct _face {

    vertex site;
    edge top_edge;

} face;

typedef struct _face_list {

    face* collection;

    int size;
    int index

} face_list;

face_list* create_face_list( vertex* sites, int size );

face* peek_next_face( face_list* list );

face* pop_next_face( face_list* list );

void destroy_face_list( face_list* list );

#endif