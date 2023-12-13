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
	${CMAKE_SOURCE_DIR}/tests/utils
)
target_link_libraries(UpdateCableType PRIVATE
    MockApiServer
		utils
    Qt6::Test
)
add_dependencies(UpdateCableType
    MockApiServer
		utils
)

add_test(NAME UpdateCableType COMMAND UpdateCableType WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}) 
