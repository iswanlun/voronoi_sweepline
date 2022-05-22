#include "test.h"
#include "../src/beach_line.h"
#include <stdio.h>

#define VLIST_SIZE 5

int test_face_creation( void ) {

    vertex points[VLIST_SIZE];

    points[0].x = 1;
    points[0].y = 2;
    points[1].x = 3;
    points[1].y = 5;
    points[2].x = 5;
    points[2].y = 1;
    points[3].x = 7;
    points[3].y = 9;
    points[4].x = 9;
    points[4].y = 11;

    face_list* list = create_face_list( points, VLIST_SIZE );

    float prev = list->collection[0].site.y;
    int pass = 1;

    for (int i = 0; i < VLIST_SIZE; i++ ) {

        printf("  x: %f y: %f \n", list->collection[i].site.x, list->collection[i].site.y );
        pass &= (prev >= list->collection[i].site.y) ? 1 : 0;
        prev = list->collection[i].site.y;
    }

    destroy_face_list( list );

    return pass;
}
