include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/examples)
link_directories(${LIBRARY_OUTPUT_PATH}/lib)

set(LIBRARIES nemoapi ${DEPENDENT_LIBRARIES} ${LIBS})

add_executable(example_sync synchronous/main.cpp)
target_link_libraries(example_sync ${LIBRARIES})

if (ASYNC)
    add_executable(example_async asynchronous/main.cpp)
    target_link_libraries(example_async ${LIBRARIES})
endif (ASYNC)