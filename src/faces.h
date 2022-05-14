#ifndef FACES_H
#define FACES_H

typedef struct _vertex {

    float x, y;

} vertex;

typedef struct _edge {

    vertex vp;
    vertex vn;

    struct _edge* prev;
    struct _edge* next;

} edge;

typedef struct _face {

    float x, y;
    
    edge top_edge;
    edge* left;
    edge* right;

} face;

#endif