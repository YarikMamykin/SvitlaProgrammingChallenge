add_executable(CreateCableType
	${CMAKE_CURRENT_SOURCE_DIR}/CreateCableType.cpp
)
target_compile_options(CreateCableType
	PUBLIC
  -g
	-fPIC 
	-Wall 
	-Werror
	-Wextra
	-Wpedantic
)
target_include_directories(CreateCableType PRIVATE
	${Qt6Core_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/mocks
	${CMAKE_SOURCE_DIR}/tests/utils
)
target_link_libraries(CreateCableType PRIVATE
    MockApiServer
		utils
    Qt6::Test
)
add_dependencies(CreateCableType
    MockApiServer
		utils
)

add_test(NAME CreateCableType COMMAND CreateCableType WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}) 
