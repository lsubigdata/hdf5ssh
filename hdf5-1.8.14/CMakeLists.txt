# hdf5 libs

include_directories(.)
add_definitions(-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_POSIX_SOURCE -D_BSD_SOURCE -DH5_DEBUG_API)

set(CMAKE_C_FLAGS -msse4.1)

file(GLOB hdf5_sources "*.c" "*.h")
file(GLOB hdf5_exclude "H5detect.c")
list(REMOVE_ITEM hdf5_sources ${hdf5_exclude})
file(GLOB hdf5_exclude "H5make_libsettings.c")
list(REMOVE_ITEM hdf5_sources ${hdf5_exclude})

add_tmp_executable(H5make_libsettings SOURCES "H5make_libsettings.c")
add_tmp_executable(H5detect SOURCES "H5detect.c")
get_target_location(h5make_exe H5make_libsettings)
get_target_location(h5detect_exe H5detect)
add_custom_command(OUTPUT "H5lib_settings.c"
    COMMAND ${h5make_exe} > "H5lib_settings.c" DEPENDS H5make_libsettings)

list(APPEND hdf5_sources "H5lib_settings.c")

add_custom_command(OUTPUT "H5Tinit.c"
    COMMAND ${h5detect_exe} > "H5Tinit.c" DEPENDS H5detect)

list(APPEND hdf5_sources 
    "H5lib_settings.c"
    "H5Tinit.c")

add_vishlib(hdf5 ${hdf5_sources})
target_link_vishlib(hdf5 ${M_LIB})
target_link_optional_vishlib(hdf5 zlib1 dmalloc)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    append_property(TARGET hdf5 VISH_DEFINES "linux=1")
    target_link_vishlib(hdf5 dl pthread)
endif()

add_subdirectories(toolslib tools)
