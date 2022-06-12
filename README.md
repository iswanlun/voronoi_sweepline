
# Fortune's Sweepline Algorithm for Voronoi Diagrams #

## About ##

This library is an implementation of Fortune's sweepline algorithm for creating Voronoi diagrams, with a few modifications.

## Building and Using ##

Clone the repo and switch into the src directory. Running make with the `so` option will create a shared object while using the `a` option will create an archive. 

The fortune.h header file in the /src/ directory contains all the nessecary declarations for including in a project and using the library.

## Modifications to Algorithm ##

The algorithm this library contains is distinct from the original sweepline algorithm. As described originally Fortune's algorithm does not produce a graph where all edges are defined, some may extend off into infinity. This library solves this and produces a doubly connected edge list (DHCL) in which every face is fully constrained. The bounds in which the DHCL will be created are passed to the `fortunes_sweep_line` function in the form of two vertices, ll (lower left) and tr (top right), creating a rectangular bounds. The libary does not check if all site points are within the bounds, as this being an issue depends on usage. As a rule of thumb all face sites should be within a valid set of bounds. 

## Code Guide ##

The header file fortune.h contains the declaration of two functions, `fortunes_sweep_line` and `destroy_face_list`. In short the first function creates a DHCL and the second frees all associated memory when finished. View the comments in fortune.h to understand the structures that are returned. 