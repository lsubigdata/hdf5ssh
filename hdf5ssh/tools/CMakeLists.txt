# Auto-generated CMakeLists.txt
# Remove this header to avoid further auto-generation
# File derived from Makefile in 'Makefile'

# Includes (3)
include_directories(${hdf5_FLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/../../hdf5 ${CMAKE_CURRENT_SOURCE_DIR}/../../toolslib)

# Libraries libraries (4)
add_inheritedLibraries(${hdf5_LIB} hdf5tools ${zlib123_LIB} ${zlib1_LIB})

# Subdirectories (8)
add_subdirectories(h5ls h5dump h5diff h5import h5stat h5repack h5copy h5jam)
