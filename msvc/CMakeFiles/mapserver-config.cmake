# - Config file for the mapserver package
# It defines the following variables
#  MAPSERVER_INCLUDE_DIRS - include directories for mapserver
#  MAPSERVER_LIBRARIES    - libraries to link against
#  MAPSERVER_EXECUTABLES  - executables build for mapserver

# Compute paths
get_filename_component(MAPSERVER_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(MAPSERVER_INCLUDE_DIRS "${MAPSERVER_CMAKE_DIR}/K:/Development/op3d_active/protobuf-3.7.1;K:/Development/op3d_active/libpng-1.4.3/;K:/Development/op3d_active/jpeg-9/;K:/Development/op3d_active/freetype-2.6.3/include;K:/Development/op3d_active/Proj-4.9.0/src;C:/Development/op3d_active/pixman-0.32.2/pixman;K:/Development/op3d_active/fribidi-0.19.6/lib;K:/Development/op3d_active/harfbuzz-2.4.0/src;K:/Development/op3d_active/libiconv-1.12/source/include;K:/Development/op3d_active/cairo-1.12.16/src;K:/Development/op3d_active/fcgi-2.4.1/include;K:/Development/op3d_active/geos-3.5.0/include;K:/Development/op3d_active/postgresql-9.3.2/src/include;K:/Development/op3d_active/gdal-2.4.0/frmts;C:/Development/op3d_active/gdal-2.4.0/alg;C:/Development/op3d_active/gdal-2.4.0/gcore;C:/Development/op3d_active/gdal-2.4.0/port;C:/Development/op3d_active/gdal-2.4.0/ogr;C:/Development/op3d_active/gdal-2.4.0/ogr/ogrsf_frmts;K:/Development/op3d_active/curl-7.63.0/include;K:/Development/op3d_active/libxml2-2.7.7/Include;K:/Development/op3d_active/libxslt-1.1.28/;K:/Development/op3d_active/giflib-5.0.4/include;C:/Program Files (x86)/MapServer/include;C:/Program Files (x86)/MapServer/include/mapserver")

# Our library dependencies (contains definitions for IMPORTED targets)
if(NOT TARGET mapserver AND NOT MapServer_BINARY_DIR)
  include("${MAPSERVER_CMAKE_DIR}/mapserverTargets.cmake")
endif()

# These are IMPORTED targets created by mapserverTargets.cmake
set(MAPSERVER_LIBRARIES mapserver)
set(MAPSERVER_EXECUTABLES sortshp shptree shptreevis msencrypt legend scalebar tile4ms shptreetst shp2img mapserv)
