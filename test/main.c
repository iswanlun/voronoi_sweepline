#include "test.h"
#include <stdio.h>

#define TEST_COUNT 4

int main( void ) {

    int (*tests[TEST_COUNT])(void);

    tests[0] = &test_face_creation;
    tests[1] = &test_vertex_events;
    tests[2] = &test_site_event_insertion;
    tests[3] = &test_fortune;
 
    for (int i = 0; i < TEST_COUNT; i++ ) {

        printf("Test %d:\n", (i+1));

        if ( tests[i]() ) {
            printf("Test %d passed.\n", (i+1) );

        } else {
            printf("Test %d failed.\n", (i+1) );
        }
    }

    return 1;
}

