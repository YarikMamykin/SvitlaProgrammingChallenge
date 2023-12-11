add_executable(${PROJECT_NAME}
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MockApiServer.cpp
)
target_compile_options(${PROJECT_NAME}
	PUBLIC
	$<$<CONFIG:Debug>:-g>
	$<$<CONFIG:Release>:-O3>
	-fPIC 
	-Wall 
	-Werror
	-Wextra
	-Wpedantic
)
target_include_directories(${PROJECT_NAME}
	PRIVATE
	${Qt6Core_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME}
	PRIVATE
	Qt6::Core
	Qt6::HttpServer
)

install(TARGETS ${PROJECT_NAME})
install(FILES ${CMAKE_BINARY_DIR}/compile_commands.json DESTINATION ${CMAKE_SOURCE_DIR})
