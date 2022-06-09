#ifndef FACES_H
#define FACES_H

#include "fortune.h"

face_list* create_face_list( vertex* sites, int size, vertex ll, vertex tr );

face* peek_next_face( face_list* list );

face* pop_next_face( face_list* list );

edge* create_edge( face* parent );

void bound_faces( face_list* list, vertex ll, vertex tr );

#endif