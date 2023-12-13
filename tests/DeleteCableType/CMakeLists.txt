add_executable(DeleteCableType
	${CMAKE_CURRENT_SOURCE_DIR}/DeleteCableType.cpp
)
target_compile_options(DeleteCableType
	PUBLIC
  -g
	-fPIC 
	-Wall 
	-Werror
	-Wextra
	-Wpedantic
)
target_include_directories(DeleteCableType PRIVATE
	${Qt6Core_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/mocks
)
target_link_libraries(DeleteCableType PRIVATE
    MockApiServer
    Qt6::Test
)
add_dependencies(DeleteCableType
    MockApiServer
)

add_test(NAME DeleteCableType COMMAND DeleteCableType WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}) 
