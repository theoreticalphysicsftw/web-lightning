cmake_minimum_required(VERSION 3.5) 

project(glad-es3)

add_library(${PROJECT_NAME} "glad.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC ".")