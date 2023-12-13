add_executable(UpdateCableType
	${CMAKE_CURRENT_SOURCE_DIR}/UpdateCableType.cpp
)
target_compile_options(UpdateCableType
	PUBLIC
  -g
	-fPIC 
	-Wall 
	-Werror
	-Wextra
	-Wpedantic
)
target_include_directories(UpdateCableType PRIVATE
	${Qt6Core_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/mocks
)
target_link_libraries(UpdateCableType PRIVATE
    MockApiServer
    Qt6::Test
)
add_dependencies(UpdateCableType
    MockApiServer
)

add_test(NAME UpdateCableType COMMAND UpdateCableType WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}) 
