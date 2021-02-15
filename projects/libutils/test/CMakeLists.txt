set(test_SRC test_main.cpp test_refcount.cpp test_event.cpp)

add_executable(test_${PROJECT_NAME} ${test_SRC})
target_include_directories(test_${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(test_${PROJECT_NAME} ${PROJECT_NAME})
