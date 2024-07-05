set(LIB_LINK_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)

set(TESTING_FRAMEWORK
	GTest::gtest_main
)

add_subdirectory(Sample)
add_subdirectory(ExtendedTypes)
add_subdirectory(PublisherSubscriber)
add_subdirectory(Thread)
add_subdirectory(Csv)
add_subdirectory(Bitset)
