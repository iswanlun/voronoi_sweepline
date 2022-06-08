#include "test.h"
#include "../src/beach_line.h"
#include "../src/fortune.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define filter_point(p) ((p == INT_MIN) ? 0 : p)
#define filter_origin_x(e) ((e == NULL) ? 0 : filter_point(e->origin.x))
#define filter_origin_y(e) ((e == NULL) ? 0 : filter_point(e->origin.y))


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

    float prev = list->collection[0]->site.y;
    int pass = 1;

    printf("  Face list:\n");

    for (int i = 0; i < list->size; i++ ) {

        printf("    x: %f y: %f \n", list->collection[i]->site.x, list->collection[i]->site.y );
        pass &= (prev >= list->collection[i]->site.y) ? 1 : 0;
        prev = list->collection[i]->site.y;
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

void print_edge( edge* e) {

    if ( e != NULL ) {

        if ( e->origin.x != INT_MIN ) {
            printf("(%f, %f)", e->origin.x, e->origin.y);
        } else {
            printf("--");
        }

    } else {
        printf("--");
    }
}

void print_twin( edge* e ) {
    
    if ( e != NULL ) {

        printf("(%f, %f)", e->home->site.x, e->home->site.y);

    } else {
        printf("--");
    }
}

void print_face( face* f ) {

    printf("  FACE - x: %f y: %f \n", f->site.x, f->site.y );

    edge* index = f->top_edge;

    do {

        printf("    POINT: ");
        print_edge(index);
        printf("    TWIN: ");
        print_twin(index->twin);
        printf("\n");
        index = index->next;

    } while ( index != NULL && index != f->top_edge );

    if ( index == NULL ) {
        printf("\n -- fail to connect, null next error -- \n");
    }
     
}

int test_site_event_insertion( void ) {

    int pass = 1;
    float sweep_line_progression;

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

    sweep_line_progression = flist->collection[0]->site.y;

    printf("\n  Staring face stack: \n");

    for ( int i = 0; i < 5; i++ ) {

        print_face( flist->collection[i] );
    }

    for ( int i = 0; i < 4; i++ ) {

        pass &= (peek_next_face( flist )->site.y <= sweep_line_progression) ? 1 : 0;
        sweep_line_progression = peek_next_face( flist )->site.y;

        site_event( ln, pop_next_face( flist ), vlist );
    }

    printf("\n  Next face in stack: \n");
    print_face( peek_next_face( flist ) );

    printf("\n  Vertex event count: %d \n", vlist->length );

    vertex_event* ve_next1 = next_vertex_event( vlist );
    vertex_event* ve_next2 = next_vertex_event( vlist );

    pass &= (sweep_line_progression >= ve_next1->sweep_y) ? 1 : 0;
    sweep_line_progression = ve_next1->sweep_y;

    pass &= (sweep_line_progression >= ve_next2->sweep_y) ? 1 : 0; // this is circumstancial to point value
    sweep_line_progression = ve_next2->sweep_y;

    printf("  Vertex event: x: %f y: %f s: %f \n",  ve_next1->v_site.x, ve_next1->v_site.y, ve_next1->sweep_y );
    printf("  Vertex event: x: %f y: %f s: %f \n",  ve_next2->v_site.x, ve_next2->v_site.y, ve_next2->sweep_y );

    circle_event( ln, ve_next1, vlist );
    circle_event( ln, ve_next2, vlist );

    site_event( ln, pop_next_face( flist ), vlist );

    printf("\n  Vertex event count: %d \n", vlist->length );

    ve_next1 = next_vertex_event( vlist );
    ve_next2 = next_vertex_event( vlist );

    pass &= (sweep_line_progression >= ve_next1->sweep_y) ? 1 : 0;
    sweep_line_progression = ve_next1->sweep_y;

    pass &= (sweep_line_progression >= ve_next2->sweep_y) ? 1 : 0; // this is circumstancial to point value
    sweep_line_progression = ve_next2->sweep_y;

    printf("  Vertex event: x: %f y: %f s: %f \n",  ve_next1->v_site.x, ve_next1->v_site.y, ve_next1->sweep_y );
    printf("  Vertex event: x: %f y: %f s: %f \n",  ve_next2->v_site.x, ve_next2->v_site.y, ve_next2->sweep_y );

    circle_event( ln, ve_next1, vlist );
    circle_event( ln, ve_next2, vlist );

    printf("\n  Final face layout: \n");

    for ( int i = 0; i < 5; i++ ) {

        print_face( flist->collection[i] );
    }

    destroy_line( ln );
    dispose_vertex_list( vlist );
    destroy_face_list( flist );

    return pass;
}

int test_simple_bounding( void ) {

    int pass = 1;

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

    for ( int i = 0; i < 4; i++ ) {
        site_event( ln, pop_next_face( flist ), vlist );
    }

    vertex_event* ve_next1 = next_vertex_event( vlist );
    vertex_event* ve_next2 = next_vertex_event( vlist );

    circle_event( ln, ve_next1, vlist );
    circle_event( ln, ve_next2, vlist );

    site_event( ln, pop_next_face( flist ), vlist );

    ve_next1 = next_vertex_event( vlist );
    ve_next2 = next_vertex_event( vlist );

    circle_event( ln, ve_next1, vlist );
    circle_event( ln, ve_next2, vlist );

    printf("\n  Begin bounding... \n");

    bound_faces( flist, ll, tr );

    printf("\n  Final face layout: \n");

    for ( int i = 0; i < flist->size; i++ ) {
        print_face( flist->collection[i] );
    }

    destroy_line( ln );
    dispose_vertex_list( vlist );
    destroy_face_list( flist );

    return pass;
}

int test_math_break_point( void ) {

    int pass = 1;


    arc* left = (arc*) malloc( sizeof(arc) );
    arc* right = (arc*) malloc( sizeof(arc) );
    
    face* left_parent = (face*) malloc( sizeof(face) );
    face* right_parent = (face*) malloc( sizeof(face) );

    left->parent = left_parent;
    right->parent = right_parent;

    left_parent->site.x = -2; 
    left_parent->site.y = 5;

    right_parent->site.x = 2;

    vertex result;

    for ( int i = 0; i < 16; i++ ) {

        right_parent->site.y = i;

        // result = break_point( left, right, -1 );

        printf("  right y val: \t    %f \n", right_parent->site.y );
        printf("  break point: \t x: %f \t y: %f \n", result.x, result.y );

    }

    left_parent->site.x = -2; 
    left_parent->site.y = 5;

    right_parent->site.x = -2;
    right_parent->site.y = 10;

    printf("\n  stacked reverse: \n");

    // result = break_point( left, right, -1 );

    printf("  break point: \t x: %f \t y: %f \n", result.x, result.y );

    // result = break_point( right, left, -1 );

    printf("  break point: \t x: %f \t y: %f \n", result.x, result.y );

    free( left );
    free( right );

    free( left_parent );
    free( right_parent );

    return pass;
}

float point_distance( vertex v1, vertex v2 ) {

    return sqrtf(powf((v2.x - v1.x), 2) + powf((v2.y - v1.y), 2));
}

int approx_equal( float f1, float f2 ) {

    return ( (((f1 + 0.01) > f2) && f1 <= f2 ) || (((f2 + 0.01) > f1) && f2 <= f1 )) ? 1 : 0;
}

int test_no_event( vertex l, vertex c, vertex r ) {

    return ( ((r.x - l.x)*(c.y - l.y)) - ((r.y - l.y)*(c.x - l.x)) );
}

int test_math_circumcenter( void ) {

    int pass = 1;


    face* left_face = (face*) malloc( sizeof(face) );
    face* center_face = (face*) malloc( sizeof(face) );
    face* right_face = (face*) malloc( sizeof(face) );

    arc* left = (arc*) malloc( sizeof(arc) );
    arc* center = (arc*) malloc( sizeof(arc) );
    arc* right = (arc*) malloc( sizeof(arc) );

    center->prev = left;
    left->next = center;

    center->next = right;
    right->prev = center;

    left->parent = left_face;
    center->parent = center_face;
    right->parent = right_face;

    left_face->site.x = -5;
    left_face->site.y = -10;

    center_face->site.x = 0;
    center_face->site.y = -11;

    right_face->site.x = 10;
    right_face->site.y = 5;

    vertex result;

    for ( int i = 0; i < 35; i++ ) {

        // printf("\n    no event: %d \n", test_no_event( left_face->site, center_face->site, right_face->site ));

        // result = circumcenter( center );

        // printf("    circumcenter \t x: %f \t y: %f \n", result.x, result.y );

        // float d1 = point_distance( result, left_face->site );
        // float d2 = point_distance( result, center_face->site );
        // float d3 = point_distance( result, right_face->site );

        // pass &= approx_equal(d1, d2) & approx_equal(d2, d3);

        // center_face->site.y++;
    }

    free( left_face);
    free( center_face);
    free( right_face);

    free( left );
    free( center );
    free( right );

    return pass;
}

void print_vertex_event_list( vertex_list* vlist ) {

    vertex_event* index = vlist->head;

    while ( index != NULL ) {

        printf("    queued vertex event \t\t s: %f \t x: %f \t y: %f \n", index->sweep_y, index->v_site.x, index->v_site.y );
        index = index->next;
    }
}

int test_multi_bounding( void ) {

    int pass = 1;

    vertex ll;
    vertex tr;

    ll.x = -10;
    ll.y = -8;

    tr.x = 10;
    tr.y = 10;
    
    int list_size = 10;

    vertex points[list_size];

    points[0].x = -3.7;
    points[0].y = 5.64;

    points[1].x = 5.94;
    points[1].y = -2.9;

    points[2].x = 7.84;
    points[2].y = 5.7;

    points[3].x = -4.5;
    points[3].y = -7.56;

    points[4].x = 2;
    points[4].y = -6;

    points[5].x = 3;
    points[5].y = -3;
    points[6].x = 7;
    points[6].y = -4;
    points[7].x = 8;
    points[7].y = 4;
    points[8].x = 9;
    points[8].y = -2;
    points[9].x = 6;
    points[9].y = 2;
    
    line* ln = create_line( ll, tr );
    
    vertex_list* vlist = create_vertex_list();
    face_list* flist = create_face_list( points, list_size, ll, tr );

    while ( peek_vertex_event( vlist ) == NULL ) {

        site_event( ln, pop_next_face( flist ), vlist );
    }

    while ( peek_next_face( flist ) != NULL ) {

        if ( peek_vertex_event( vlist ) == NULL ) {

            site_event( ln, pop_next_face( flist ), vlist );
        
        } else {

            if ( peek_next_face( flist )->site.y > peek_vertex_event( vlist )->sweep_y ) {

                site_event( ln, pop_next_face( flist ), vlist );

            } else {
                circle_event( ln, next_vertex_event( vlist ), vlist );
            }
        }
    }

    while ( peek_vertex_event( vlist ) != NULL ) {

        circle_event( ln, next_vertex_event( vlist ), vlist );
    }
    
    for ( int i = 0; i < flist->size; i++ ) {
        print_face( flist->collection[i] );
    }

    printf("\n  bounding... \n\n");

    bound_faces( flist, ll, tr );

    for ( int i = 0; i < flist->size; i++ ) {
        print_face( flist->collection[i] );
    }

    destroy_line( ln );
    dispose_vertex_list( vlist );
    destroy_face_list( flist );

    return pass;
}

void print_face_list( face_list* fl ) {

    for ( int i = 0; i < fl->size; i++ ) {

        printf("  FACE - x: %f y: %f \n", fl->collection[i]->site.x, fl->collection[i]->site.y );

        edge* index = fl->collection[i]->top_edge;

        do {

            printf("    POINT: ");
            print_edge(index);
            printf("\n");
            index = index->next;

        } while ( index != fl->collection[i]->top_edge );
    }
}

int test_fortune_line_sweep( void ) {

    int pass = 1;

    vertex ll;
    vertex tr;

    ll.x = -10;
    ll.y = -8;

    tr.x = 10;
    tr.y = 10;
    
    int list_size = 10;

    vertex points[list_size];

    points[0].x = -3.7;
    points[0].y = 5.64;
    points[1].x = 5.94;
    points[1].y = -2.9;
    points[2].x = 7.84;
    points[2].y = 5.7;
    points[3].x = -4.5;
    points[3].y = -7.56;
    points[4].x = 2;
    points[4].y = -6;
    points[5].x = 3;
    points[5].y = -3;
    points[6].x = 7;
    points[6].y = -4;
    points[7].x = 8;
    points[7].y = 4;
    points[8].x = 9;
    points[8].y = -2;
    points[9].x = 6;
    points[9].y = 2;

    face_list* flist = fortunes_sweep_line( points, list_size, ll, tr );

    print_face_list( flist );

    destroy_face_list( flist );

    return pass;
}