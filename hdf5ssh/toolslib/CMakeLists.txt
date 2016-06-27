#tools lib

include_directories(../)
add_definitions(-DWITH_hdf5)
file(GLOB src "*.c")
add_vishlib(hdf5tools ${src})
target_link_vishlib(hdf5tools hdf5)