#include "test.h"
#include "../src/beach_line.h"
#include <stdio.h>

int test_face_creation( void ) {
    
    int list_size = 10;

    vertex points[list_size];

    points[0].x = 0;
    points[0].y = 2;
    points[1].x = 0;
    points[1].y = 5;
    points[2].x = 0;
    points[2].y = 1;
    points[3].x = 0;
    points[3].y = 9;
    points[4].x = 0;
    points[4].y = 11;
    points[5].x = 0;
    points[5].y = -3;
    points[6].x = 0;
    points[6].y = -32;
    points[7].x = 0;
    points[7].y = 16;
    points[8].x = 0;
    points[8].y = -2;
    points[9].x = 0;
    points[9].y = 2;
    

    face_list* list = create_face_list( points, list_size );

    float prev = list->collection[0].site.y;
    int pass = 1;

    for (int i = 0; i < list_size; i++ ) {

        printf("  x: %f y: %f \n", list->collection[i].site.x, list->collection[i].site.y );
        pass &= (prev >= list->collection[i].site.y) ? 1 : 0;
        prev = list->collection[i].site.y;
    }

    destroy_face_list( list );

    return pass;
}


int test_vertex_events( void ) {

    vertex_list* list = create_vertex_list();

    vertex_event* dud = NULL;
    vertex_event* ext = NULL;

    insert_vertex_event( list, &(dud), 1, 0, -5);
    dud = NULL;
    insert_vertex_event( list, &(dud), 2, 0, 5);
    dud = NULL;
    insert_vertex_event( list, &(dud), 3, 0, 7);
    dud = NULL;
    insert_vertex_event( list, &(dud), 4, 0, 12);
    dud = NULL;
    insert_vertex_event( list, &(dud), 5, 0, 34);
    dud = NULL;
    insert_vertex_event( list, &(dud), 6, 0, 2);
    dud = NULL;
    insert_vertex_event( list, &(dud), 7, 0, -2);
    dud = NULL;
    insert_vertex_event( list, &(dud), 8, 0, 31);
    dud = NULL;
    insert_vertex_event( list, &(dud), 9, 0, 8);
    dud = NULL;

    insert_vertex_event( list, &(ext), 10, 0, -1);
    insert_vertex_event( list, &(dud), 11, 0, 41);

    printf("  Top value (41) -> %f \n", peek_vertex_event(list));

    vertex_event* index = list->head;

    int pass = 1;
    float prev = index->sweep_y;
    while( index != NULL ) {

        printf("  val: %f\n", index->sweep_y );

        pass &= (index->sweep_y <= prev) ? 1 : 0;
        prev = index->sweep_y;
        index = index->next;
    }

    int count = list->length;

    null_vertex_event( list, &(dud) );
    null_vertex_event( list, &(ext) );

    pass &= ( dud == NULL ) ? 1 : 0;
    pass &= ( ext == NULL ) ? 1 : 0;

    pass &= (count == (list->length + 2)) ? 1 : 0;

    printf("  old count: %d new count: %d \n", count, list->length );
    printf("  new list: \n");

    index = list->head;
    prev = index->sweep_y;
    while( index != NULL ) {

        printf("  val: %f\n", index->sweep_y );

        pass &= (index->sweep_y <= prev) ? 1 : 0;
        prev = index->sweep_y;
        index = index->next;
    }

    dispose_vertex_list( list );
    return pass;
}

int test_beach_line_creation( void ) {

    int pass = 1;

    vertex ll;
    ll.x = 0;
    ll.y = 0;

    vertex tr;
    tr.x = 100;
    tr.y = 100;

    line* l = create_line( ll, tr );

    printf("  Created bound faces sites:\n");

    for (int i = 0; i < 4; i++ ) {

        printf("  Point %d: x: %f y: %f\n", (i+1), l->bounds[i]->site.x, l->bounds[i]->site.y );

    }

    destroy_line( l );

    return pass;
}