set( rpg_SRC
M2optPoly.c      M2optPolyII.c     MCPolygon.c     MbouncePoly.c
MgrowingPoly.c   MrejectPoly.c     MsearchPoly.c   MspPoly.c
MstarPoly.c      MtriangPoly.c     MxmonoPoly.c    analysis.c
analysisII.c     argEval.c         basicAVL.c      basicDoubleArray.c
basicGeom.c      basicHeap.c       basicInits.c    basicIntArray.c
basicIo.c        basicLine.c       basicPoly.c     basicPolyLine.c
basicStack.c     basicVoidArray.c  binArray.c      calcPoly.c
convexHull.c     edgeIncArray.c    elapsed.c       eralloc.c
error.c          forcedEdge.c      geom.c          geom2.c
geomHash.c       global.c          incConvexHull.c
indexAVL.c       intIncArray.c     io.c            ipeWrite.c
isectArray.c     lineHeap.c        lineInfo.c      linkPoly.c
main.c           mem.c             merge.c         nodeList.c
outWrite.c        pathTree.c      pointArray.c     pointOps.c
pointIncArray.c  pointList.c       poly.c          polyOps.c
poly2.c          polyAVL.c         polyArray.c     polyIncArray.c
qhull.c           quadEdge.c      set.c            stateLine.c
smoothPoly.c     sortPoly.c        starKernel.c    starPoly.c
stat.c           sweepArrange.c  triangAVL.c
triangArray.c    uLongArray.c      user.c         voidIncArray.c
"${PROJECT_BINARY_DIR}/src/gitversion.h"
)

include(CheckIncludeFile)
if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
	message("Building debug version")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(rpg ${rpg_SRC} main.c)

find_library(M_LIB m)
target_link_libraries(rpg ${M_LIB})
target_link_libraries(rpg ${rpg_LIBS})
set(rpg_flags "")	

set_target_properties(rpg PROPERTIES COMPILE_FLAGS "${rpg_flags}")

add_custom_command(
  OUTPUT "${PROJECT_BINARY_DIR}/src/gitversion.h"
  DEPENDS "${CMAKE_SOURCE_DIR}/.git/logs/HEAD"
  COMMAND ${CMAKE_COMMAND}
    -Dlocal_dir="${CMAKE_SOURCE_DIR}"
    -Doutput_dir="${PROJECT_BINARY_DIR}/src"
    -P "${CMAKE_SOURCE_DIR}/cmake/gitversion.cmake"
)
