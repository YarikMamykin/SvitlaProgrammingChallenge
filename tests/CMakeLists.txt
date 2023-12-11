enable_testing()

file(GLOB subdirectories RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
set(dirlist "")
foreach(subdir ${subdirectories})
	if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
		add_subdirectory(${subdir})
	endif()
endforeach()
