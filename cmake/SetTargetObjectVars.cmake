#
# It would have been nice had cmake permitted target_link_libraries() to work for OBJECT type libraries.
#
# This macro acts as some kind of poor-man's subsitute/analog for target_link_libraries() to be used in conjunction with a OBJECT library target.
# It sets up compiler flags using 'cmake generator expressions' that look like $<> and which you really don't want to know about.
# It takes three arguments: the name of the object library target, the mode and a list of libraries that components of the object library may require.
# Example: 
# 
# set(my_obj_lib_SRCS "some_dbus_interface.cpp some_implementation_detail.cpp something_to_unit_test.cpp")
# add_library(my_obj_collection ${my_obj_lib_SRCS})
# set_target_object_vars(my_obj_collection PRIVATE Qt5::Core Qt5::DBus)
#
# Mode is an uppercase value: PRIVATE, PUBLIC or INTERFACE.
# Convenience versions that remove the need for _mode are available: set_private_target_object_vars, set_interface_target_object_vars, set_public_target_object_vars
#
# Rule of thumb: if the libraries are not visible in the interface (header definitions) of any object, you want PRIVATE. If they are, you probably want PUBLIC.
# Header-only things probably make no sense but those would be INTERFACE. Maybe.
#
macro(set_target_object_vars _object_lib_target _mode)
foreach(_lib_you_may_need IN ITEMS ${ARGN})
    message(STATUS "Picking up include directories & compile options for OBJECT lib: ${_object_lib_target}, mode: ${_mode}, from dependency: ${_lib_you_may_need}")
    target_include_directories("${_object_lib_target}" ${_mode} $<TARGET_PROPERTY:${_lib_you_may_need},INTERFACE_INCLUDE_DIRECTORIES>)
    target_compile_options("${_object_lib_target}" ${_mode} $<TARGET_PROPERTY:${_lib_you_may_need},INTERFACE_COMPILE_OPTIONS>)
endforeach(_lib_you_may_need)
endmacro(set_target_object_vars)

macro(set_private_target_object_vars _object_lib)
set_target_object_vars("${_object_lib}" PRIVATE ${ARGN})
endmacro(set_private_target_object_vars)

macro(set_interface_target_object_vars _object_lib)
set_target_object_vars("${_object_lib}" INTERFACE ${ARGN})
endmacro(set_interface_target_object_vars)

macro(set_public_target_object_vars _object_lib)
set_target_object_vars("${_object_lib}" PUBLIC ${ARGN})
endmacro(set_public_target_object_vars)