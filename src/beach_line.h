#ifndef BEACH_LINE_H
#define BEACH_LINE_H

#include "faces.h"
#include "events.h"

typedef struct _arc {

    face* parent;
    
    struct _arc* prev;
    struct _arc* next;

    vertex_event* pinch;

} arc;

#endif