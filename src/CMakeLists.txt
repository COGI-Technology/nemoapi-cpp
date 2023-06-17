file(GLOB SOURCES *.cpp)
file(GLOB SOURCES_ASYNC *_async.cpp uv_*.cpp)
list(REMOVE_ITEM SOURCES cli.cpp)

if (NOT ASYNC)
    foreach (s ${SOURCES_ASYNC})
        list(REMOVE_ITEM SOURCES ${s})
    endforeach (s ${SOURCES_ASYNC})
endif (NOT ASYNC)

add_library(nemoapi SHARED ${SOURCES})
target_link_libraries(nemoapi ${DEPENDENT_LIBRARIES} ${LIBS})

set(LIBRARIES nemoapi ${DEPENDENT_LIBRARIES} ${LIBS})
add_executable(cli cli.cpp)
target_link_libraries(cli ${LIBRARIES})