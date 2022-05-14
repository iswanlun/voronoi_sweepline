#ifndef BEACH_LINE_H
#define BEACH_LINE_H

#include "faces.h"

typedef struct _arc {

    face* parent;
    
    struct _arc* prev;
    struct _arc* next;

} arc;

#endif