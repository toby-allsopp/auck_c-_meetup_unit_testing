cmake_minimum_required(VERSION 3.0)

add_compile_options(-std=c++17)

add_executable(unit_test
  main.cpp
  #NameEntity.cpp
  #NameEntity2.cpp
  NameEntity_virtual_mocks.cpp
  #NameEntity_virtual_mocks2.cpp
  )
