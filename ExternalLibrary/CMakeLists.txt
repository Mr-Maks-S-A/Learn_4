cmake_minimum_required(VERSION 3.16)
project(ExternalLibrary CXX)


# doctest
set(DOCTEST_NO_INSTALL ON CACHE BOOL "" FORCE)
set(DOCTEST_WITH_TESTS OFF CACHE BOOL "" FORCE)


add_subdirectory(doctest)

