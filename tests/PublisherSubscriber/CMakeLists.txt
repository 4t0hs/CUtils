set(TARGET PublisherSubscriberTest)

add_executable(${TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_compile_features(${TARGET} PUBLIC cxx_std_20)

target_include_directories(${TARGET} PRIVATE
	${INCLUDE_DIRECTROY}
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET} PRIVATE
	${LIBRARY_NAME}
	${EXTERNAL_LIBRARY}
	${TESTING_FRAMEWORK}
	$<$<CONFIG:Coverage>:gcov>
	$<$<CONFIG:Address>:asan>
)

target_compile_options(${TARGET} PRIVATE
	${WARNING_OPTIONS}
	$<$<CONFIG:Debug>:${DEBUG_OPTIONS}>
	$<$<CONFIG:Release>:${NODEBUG_OPTIONS}>
	$<$<CONFIG:Coverage>:${DEBUG_OPTIONS} --coverage>
	$<$<CONFIG:Address>:${DEBUG_OPTIONS} -fsanitize=address>
)
add_test(NAME ${TARGET} COMMAND ${TARGET}-googletest)

enable_testing()
