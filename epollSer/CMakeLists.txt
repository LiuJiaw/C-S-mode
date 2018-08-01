cmake_minimum_required(VERSION 3.5)

project(epollSer)

set(SOURCE_FILES 
	task.h
	threadpool.h
	threadpool.cpp
	epoll.h	
	epoll.cpp
	Main.cpp
        )


add_executable(epollSer ${SOURCE_FILES})

find_package(Threads)

target_link_libraries(epollSer ${CMAKE_THREAD_LIBS_INIT})
