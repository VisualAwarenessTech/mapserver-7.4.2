# - Find LibSVG and LibSVG-Cairo
#
# Following variables are provided:
# SVG_FOUND
#     True if libsvg has been found
# SVG_INCLUDE_DIR
#     The include directories of libsvg
# SVG_LIBRARY
#     libsvg library list
# Following variables are provided:
# SVGCAIRO_FOUND
#     True if libsvg-cairo has been found
# SVGCAIRO_INCLUDE_DIR
#     The include directories of libsvg-cairo
# SVGCAIRO_LIBRARY
#     libsvg-cairo library list

set(SVG_INCLUDE_DIR "k:/Development/op3d_active/libsvg-0.1.4/src")
set(SVG_LIBRARY "k:/Development/op3d_active/libsvg-0.1.4/vc141/x64/vs2017_Release/libsvg.lib")
set (SVG_FOUND TRUE)

set(SVGCAIRO_INCLUDE_DIR "k:/Development/op3d_active/libsvg-cairo-0.1.6/src")
set(SVGCAIRO_LIBRARY "k:/Development/op3d_active/libsvg-cairo-0.1.6/msvc/vc141/x64/vs2017_Release")
set(SVGCAIRO_FOUND TRUE)

#find_package(PkgConfig)
#pkg_check_modules(PC_SVG QUIET libsvg>=0.1.4)
#pkg_check_modules(PC_SVGCAIRO QUIET libsvg-cairo>=0.1.6)

#find_path(SVG_INCLUDE_DIR
#   NAMES svg.h
#   HINTS ${PC_SVG_INCLUDE_DIR} ${PC_SVG_INCLUDE_DIRS}
#)

#find_library(SVG_LIBRARY
#   NAME svg
#   HINTS ${PC_SVG_LIBDIR} ${PC_SVG_LIBRARY_DIRS}
#)

#find_path(SVGCAIRO_INCLUDE_DIR
#   NAMES svg-cairo.h
#   HINTS ${PC_SVGCAIRO_INCLUDE_DIR} ${PC_SVGCAIRO_INCLUDE_DIRS}
#)

#find_library(SVGCAIRO_LIBRARY
#   NAME svg-cairo
#   HINTS ${PC_SVGCAIRO_LIBDIR} ${PC_SVGCAIRO_LIBRARY_DIRS}
#)

#include(FindPackageHandleStandardArgs)

set(SVG_INCLUDE_DIRS ${SVG_INCLUDE_DIR})
set(SVG_LIBRARIES ${SVG_LIBRARY})
#find_package_handle_standard_args(SVG DEFAULT_MSG SVG_LIBRARY SVG_INCLUDE_DIR)
mark_as_advanced(SVG_LIBRARY SVG_INCLUDE_DIR)
set(SVGCAIRO_INCLUDE_DIRS ${SVGCAIRO_INCLUDE_DIR})
set(SVGCAIRO_LIBRARIES ${SVGCAIRO_LIBRARY})
#find_package_handle_standard_args(SVGCAIRO DEFAULT_MSG SVGCAIRO_LIBRARY SVGCAIRO_INCLUDE_DIR)
mark_as_advanced(SVGCAIRO_LIBRARY SVGCAIRO_INCLUDE_DIR)
