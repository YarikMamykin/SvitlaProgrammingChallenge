add_executable(TestLogin
	${CMAKE_CURRENT_SOURCE_DIR}/TestLogin.cpp
)
target_compile_options(TestLogin
	PUBLIC
  -g
	-fPIC 
	-Wall 
	-Werror
	-Wextra
	-Wpedantic
)
target_include_directories(TestLogin PRIVATE
	${Qt6Core_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/mocks
)
target_link_libraries(TestLogin PRIVATE
    MockApiServer
    Qt6::Test
)
add_dependencies(TestLogin
    MockApiServer
)

add_test(NAME TestLogin COMMAND TestLogin WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}) 
