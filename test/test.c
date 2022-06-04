#include "test.h"
#include "../src/beach_line.h"
#include "../src/fortune.h"
#include <stdio.h>
#include <stdlib.h>

int test_face_creation( void ) {

    vertex ll;
    vertex tr;

    ll.x = -5;
    ll.y = -40;

    tr.x = 5;
    tr.y = 20;
    
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
    
    face_list* list = create_face_list( points, list_size, ll, tr );

    float prev = list->collection[0].site.y;
    int pass = 1;

    printf("  Face list:\n");

    for (int i = 0; i < list->size; i++ ) {

        printf("    x: %f y: %f \n", list->collection[i].site.x, list->collection[i].site.y );
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

    printf("  Top value (41) -> %f \n", peek_vertex_event(list)->sweep_y);

    vertex_event* index = list->head;

    int pass = 1;
    float prev = index->sweep_y;
    while( index != NULL ) {

        printf("    val: %f\n", index->sweep_y );

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

        printf("    val: %f\n", index->sweep_y );

        pass &= (index->sweep_y <= prev) ? 1 : 0;
        prev = index->sweep_y;
        index = index->next;
    }

    dispose_vertex_list( list );
    return pass;
}

void print_face( face* f ) {

    printf("  FACE - x: %f y: %f \n", f->site.x, f->site.y );

    edge* index = f->top_edge;

    do {

        printf("    POINT: - x: %f y: %f \n", index->origin.x, index->origin.y );
        index = index->next;

    } while ( index != NULL && index != f->top_edge );

    if ( index == NULL ) {
        printf("\n -- fail to connect, null next error -- \n");
    }
     
}

int test_site_event_insertion( void ) {
    
    vertex ll;
    vertex tr;

    ll.x = -10;
    ll.y = -10;

    tr.x = 10;
    tr.y = 10;

    vertex points[1];

    points[0].x = -1;
    points[0].y = 6;

    line* ln = create_line( ll, tr );
    vertex_list* vlist = create_vertex_list();

    face_list* flist = create_face_list( points, 1, ll, tr );

    printf("\n  Staring face stack: \n");

    for ( int i = 0; i < 5; i++ ) {

        print_face( &(flist->collection[i]) );
    }
    
    site_event( ln, pop_next_face( flist ), vlist );

    printf("\n  After single site event: \n");

    for ( int i = 0; i < 5; i++ ) {

        print_face( &(flist->collection[i]) );
    }

    site_event( ln, pop_next_face( flist ), vlist );

    printf("\n  After second site event: \n");

    for ( int i = 0; i < 5; i++ ) {

        print_face( &(flist->collection[i]) );
    }

    for ( int i = 2; i < 4; i++ ) {

        site_event( ln, pop_next_face( flist ), vlist );
    }

    printf("\n  Next face in stack: \n");
    print_face( peek_next_face( flist ) );

    printf("\n  Vertex event count: %d \n", vlist->length );

    vertex_event* ve_next1 = next_vertex_event( vlist );
    vertex_event* ve_next2 = next_vertex_event( vlist );

    printf("  Vertex event: x: %f y: %f s: %f \n",  ve_next1->v_site.x, ve_next1->v_site.y, ve_next1->sweep_y );
    printf("  Vertex event: x: %f y: %f s: %f \n",  ve_next2->v_site.x, ve_next2->v_site.y, ve_next2->sweep_y );

    circle_event( ln, ve_next1, vlist );
    circle_event( ln, ve_next2, vlist );

    free(ve_next1);
    free(ve_next2);


    for ( int i = 0; i < 4; i++ ) {

        print_face( &(flist->collection[i]) );
    }

    destroy_line( ln );
    dispose_vertex_list( vlist );

    return 1;
}

int test_fortune( void ) {

    vertex sites[3];

    vertex ll;
    vertex tr;

    ll.x = 0;
    ll.y = 0;

    tr.x = 30;
    tr.y = 30;

    sites[0].x = 8;
    sites[0].y = 14;
    sites[1].x = 15;
    sites[1].y = 14;
    sites[3].x = 23;
    sites[3].y = 14;

    face_list* f_list = fortunes_sweep_line( sites, 3, ll, tr );

    for ( int i = 0; i < f_list->size; i++ ) {

        edge* index = f_list->collection[i].top_edge;

        printf("  face - x: %f y: %f ", index->home->site.x, index->home->site.y );

        do {

            printf("    point - x: %f y: %f ", index->origin.x, index->origin.y );

        } while ( index != f_list->collection[i].top_edge );
    }    

    return 1;
}
