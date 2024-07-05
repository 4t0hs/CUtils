set(TARGET ${LIBRARY_NAME})

file(GLOB_RECURSE SOURCES ${SOURCE_DIRECTROY}/*.c)
message(STATUS [${TARGET}] ": C source files=" ${SOURCES})

# build target
add_library(${TARGET} STATIC ${SOURCES})
# set features
target_compile_features(${TARGET} PUBLIC c_std_17)

# set options
target_compile_options(${TARGET} PRIVATE
	${WARNING_OPTIONS}
	$<$<CONFIG:Debug>:${DEBUG_OPTIONS}>
	$<$<CONFIG:Release>:${NODEBUG_OPTIONS}>
	$<$<CONFIG:Coverage>:${DEBUG_OPTIONS} --coverage>
	$<$<CONFIG:Address>:${DEBUG_OPTIONS} -fsanitize=address>
)

target_link_libraries(${TARGET} PRIVATE
	${EXTERNAL_LIBRARY}
	$<$<CONFIG:Coverage>:gcov>
	$<$<CONFIG:Address>:asan>
)

target_include_directories(${TARGET} PRIVATE
	${INCLUDE_DIRECTROY}
	${EXTERNAL_INCLUDE}
	${CMAKE_CURRENT_SOURCE_DIR}/Utilities
)

target_compile_definitions(${TARGET} PRIVATE
	_GNU_SOURCE
)

add_subdirectory(Sample)
add_subdirectory(Csv)
add_subdirectory(ExtendedTypes)
add_subdirectory(PublisherSubscriber)
add_subdirectory(Thread)
