add_executable(GetCableType
	${CMAKE_CURRENT_SOURCE_DIR}/GetCableType.cpp
)
target_compile_options(GetCableType
	PUBLIC
  -g
	-fPIC 
	-Wall 
	-Werror
	-Wextra
	-Wpedantic
)
target_include_directories(GetCableType PRIVATE
	${Qt6Core_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/mocks
)
target_link_libraries(GetCableType PRIVATE
    MockApiServer
    Qt6::Test
)
add_dependencies(GetCableType
    MockApiServer
)

add_test(NAME GetCableType COMMAND GetCableType WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}) 
