set(FBSTAB_MATLAB_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(FBSTAB_MEXUTILS ${FBSTAB_MATLAB_ROOT}/mexutils)

#add_subdirectory(mexutils)
# Look for MATLAB API library paths
find_package(Matlab REQUIRED COMPONENTS MX_LIBRARY)
# Libmex library included by default.
# This demo target only requires MX_LIBRARY (libmx) in addition
# add other components as needed. See Modules/FindMatlab.cmake or it 
# documentation for other supported components. 

#set(MATLAB_ADDITIONAL_VERSIONS "MATLAB_R2019a.app=9.6")
#get_filename_component(Matlab_RELEASE ${Matlab_ROOT_DIR} NAME)
#matlab_get_version_from_release_name(${Matlab_RELEASE} Matlab_VERSION)
#if (Matlab_VERSION AND Matlab_VERSION VERSION_LESS "8.5")
#  add_compile_definitions(MATLAB_PRE_R2015A)
#endif()

# turn off new Matlab class support
set(Matlab_HAS_CPP_API 0)

# interface library for mex related files
add_library(libmex INTERFACE)
target_include_directories(libmex INTERFACE ${Matlab_INCLUDE_DIRS})
target_link_libraries(libmex INTERFACE ${Matlab_MEX_LIBRARY} ${Matlab_MX_LIBRARY})

# header-only interface mexutils library
add_library(libmexutils INTERFACE)
target_include_directories(libmexutils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(libmexutils INTERFACE libmex)

add_subdirectory(@FBstabMpc)