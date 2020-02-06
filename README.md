# Random Polygon Generator (RPG)

The code `rpg` generates (simply-connected and multiply-connected) polygons on
a set of user-specified or randomly generated polygons. RPG 3.0 is a revived
and clean-up version of C code fragments that were implemented during the
course of the last 25 years.

# Run-time options

    --random <number>   generates <number> random points within the unit square.
    --circle <number>   generates <number> random points within the unit circle.
    --cluster <number>  generates <number> clustered points within the unit 
                        circle.
    --seed <number>     initializes the random number generator with <number>,
                        which has to be a non-negative integer.
    --algo <algo>       specifies the algorithm used to generate the polygon.
                        Valid parameters are as follows:
                          "xmono", "star", "growth", "2opt", "space", "triangle".
    --smooth <number>   calls the smoothing routine <number> times, where
                        <number> has to be a positive integer.
    --holes <number>    generates (up to) <number> holes, where <number> has to
                        be a positive integer.
    --format <format>   specifies the format of the output file: "ipe" or "line".
    --output <file>     directs the output to file <file>.
    --input <file>      the input points are read from <file>.
    --count <number>    specifies the number of polygons to generate.

# Sample use

    rpg --cluster 30 --algo 2opt --format line --output rpg --seed 10 --holes 2

This command will cause a multiply-connected polygon with 30 vertices and with
two holes to be written to the output file "rpg.line".

Permissible input files contain the number of input points in the first
line. All other lines contain pairs of x-coordinates and y-coordinates, with
one pair of coordinates per input point. The output format "ipe" tells rpg to
generate output suitable for the drawing editor Ipe (http://ipe.otfried.org/),
while the format "line" results in a sequence of polygons, where each polygon
is encoded by the number of its vertices followed by pairs of x-coordinates
and y-coordinates, with one pair of coordinates per vertex. Please note that
the first vertex of each polygon is counted and out twice, once at the
beginning of the vertex list and once at the end of the vertex list.

Please note that the code might end up with fewer holes than requested by the
user. Furthermore, please be warned that it might generate degenerate
(zero-area) holes if the input contains multiple collinear points.

Please see the following publication for details on the algorithms:

   Thomas Auer and Martin Held.
   Heuristics for the generation of random polygons.
   Proc. 8th Canadian Conference on Computational Geometry (CCCG'96), pages 38-43, 1996.
   http://www.cccg.ca/proceedings/1996/cccg1996_0007.pdf.

We have the following correspondence to the algorithms described in this paper:

* "star" implements "Quick Star" 
* "growth" implements "Steady Growth"
* "space" implements "Space Partitioning"
* "2opt" implements "2-opt Moves"

In addition, "xmono" implements the algorithm by Zhu, Sundaram, Snoeyink and 
Mitchell (CGTA 1996) for generating x-monotone polygons. Please be warned that
it will work for only very small numbers of vertices; otherwise, an integer
overflow will occur. Furthermore, "triangle" generates polygons by searching
for feasible edges within a random triangulation of the vertices.



Please direct bug reports or suggestions to Martin Held at held@cs.sbg.ac.at.

# Contact

    E-Mail:      held@cs.sbg.ac.at
    Snail Mail:  Martin Held
                 Universität Salzburg
                 FB Computerwissenschaften
                 Computational Geometry and Applications Lab
                 A-5020 Salzburg, Austria

# Copyright/License

Copyright T. Auer, M. Gschwandtner, M. Heimlich, M. Held  1994-2020, see files COPYING.txt.

Copyright C.B. Barber, see file COPYING_Qhull.txt.
