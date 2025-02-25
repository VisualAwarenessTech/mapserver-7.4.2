# - Find PHP
# This module finds if PHP is installed and determines where the include files
# and libraries are. It also determines what the name of the library is. This
# code sets the following variables:
#
#  PHP_INCLUDE_PATH       = path to where php.h can be found
#  PHP_EXECUTABLE         = full path to the php binary
#

SET(PHP_FOUND "NO")

SET(PHP_POSSIBLE_INCLUDE_PATHS
  /usr/include/php
  /usr/local/include/php
  /usr/include/php
  /usr/local/include/php
  /usr/local/apache/php
  ${PHP_INCLUDES}
)

#find_library(PHP_LIBRARY NAMES php5ts php5 php7ts php7 PATHS /sw /opt/local)

set(PHP_LIBRARY_NAMES "K:/Development/op3d_active/php-7.2.22_dev/lib/php7ts.lib")
#if(WIN32)
#  string(SUBSTRING $ENV{VSCMD_VER} 0 2 VSCMD_VER)
#  SET(PHP_INCLUDE_PATH "C:/php-sdk/phpmaster/vc${VSCMD_VER}/$ENV{VSCMD_ARG_TGT_ARCH}/php-src")
#  SET(PHP_POSSIBLE_INCLUDE_PATHS ${PHP_INCLUDE_PATH})
#  if(CMAKE_CL_64)
#    SET(PHP_INCLUDE_PATH "${PHP_INCLUDE_PATH}/$ENV{PLATFORM}")
#  endif(CMAKE_CL_64)
#  if(WITH_THREAD_SAFETY)
#    SET(PHP_INCLUDE_PATH "${PHP_INCLUDE_PATH}/Release_TS")
#    SET(PHP_LIBRARY_NAMES php5ts php7ts)
#  else(WITH_THREAD_SAFETY)
#    SET(PHP_INCLUDE_PATH "${PHP_INCLUDE_PATH}/Release")
#    SET(PHP_LIBRARY_NAMES php5 php7)
#  endif(WITH_THREAD_SAFETY)
#    find_library(PHP_LIBRARY NAMES ${PHP_LIBRARY_NAMES} PATHS ${PHP_INCLUDE_PATH})
#endif(WIN32)


#FIND_PATH(PHP_FOUND_INCLUDE_PATH main/php.h ${PHP_POSSIBLE_INCLUDE_PATHS})

set (PHP_FOUND_INCLUDE_PATH "K:/Development/op3d_active/php-7.2.22_dev/include")
IF(PHP_FOUND_INCLUDE_PATH)
  SET(php_paths "${PHP_POSSIBLE_INCLUDE_PATHS}")
  FOREACH(php_path Zend main TSRM)
    SET(php_paths ${php_paths} "${PHP_FOUND_INCLUDE_PATH}/${php_path}")
  ENDFOREACH(php_path Zend main TSRM)
  SET(PHP_INCLUDE_PATH "${php_paths}" INTERNAL "PHP include paths")
ENDIF(PHP_FOUND_INCLUDE_PATH)

#FIND_PROGRAM(PHP_EXECUTABLE
#  NAMES php
#  PATHS
#  /usr/local/bin
#  )
set (PHP_EXECUTABLE "C:/Program Files/Visual Awareness Technologies and Consulting Inc/bin/php.exe")

MARK_AS_ADVANCED(
  PHP_EXECUTABLE
  PHP_FOUND_INCLUDE_PATH
  )

#IF( NOT PHP_CONFIG_EXECUTABLE )
#FIND_PROGRAM(PHP_CONFIG_EXECUTABLE
#  NAMES php-config
#  )
#ENDIF( NOT PHP_CONFIG_EXECUTABLE )

#IF(PHP_CONFIG_EXECUTABLE)
#  EXECUTE_PROCESS(COMMAND ${PHP_CONFIG_EXECUTABLE} --version
#    OUTPUT_VARIABLE PHP_VERSION)
#  STRING(REPLACE "\n" "" PHP_VERSION "${PHP_VERSION}")
  set(PHP_VERSION "7.2.22")
#  EXECUTE_PROCESS(COMMAND ${PHP_CONFIG_EXECUTABLE} --extension-dir
#    OUTPUT_VARIABLE PHP_EXTENSION_DIR)
#  STRING(REPLACE "\n" "" PHP_EXTENSION_DIR "${PHP_EXTENSION_DIR}")

#  EXECUTE_PROCESS(COMMAND ${PHP_CONFIG_EXECUTABLE} --includes
#    OUTPUT_VARIABLE PHP_INCLUDES)
#  STRING(REPLACE "-I" "" PHP_INCLUDES "${PHP_INCLUDES}")
#  STRING(REPLACE " " ";" PHP_INCLUDES "${PHP_INCLUDES}")
#  STRING(REPLACE "\n" "" PHP_INCLUDES "${PHP_INCLUDES}")
#  LIST(GET PHP_INCLUDES 0 PHP_INCLUDE_DIR)

  set(PHP_INCLUDE_DIR "K:/Development/op3d_active/php-7.2.22_dev/include")
  set(PHP_MAIN_INCLUDE_DIR ${PHP_INCLUDE_DIR}/main)
  set(PHP_TSRM_INCLUDE_DIR ${PHP_INCLUDE_DIR}/TSRM)
  set(PHP_ZEND_INCLUDE_DIR ${PHP_INCLUDE_DIR}/Zend)
  set(PHP_REGEX_INCLUDE_DIR ${PHP_INCLUDE_DIR}/regex)
  set(PHP_EXT_INCLUDE_DIR ${PHP_INCLUDE_DIR}/ext)
  set(PHP_DATE_INCLUDE_DIR ${PHP_INCLUDE_DIR}/ext/date/lib)
  set(PHP_STANDARD_INCLUDE_DIR ${PHP_INCLUDE_DIR}/ext/standard)

  MESSAGE(STATUS ${PHP_MAIN_INCLUDE_DIR})

  IF(NOT PHP_INCLUDE_PATH)
    set(PHP_INCLUDE_PATH ${PHP_INCLUDES})
  ENDIF(NOT PHP_INCLUDE_PATH)

  IF(PHP_VERSION LESS 5)
    MESSAGE(FATAL_ERROR "PHP version is not 5 or later")
  ENDIF(PHP_VERSION LESS 5)

  IF(PHP_EXECUTABLE AND PHP_INCLUDES)
    set(PHP_FOUND "yes")
    MESSAGE(STATUS "Found PHP-Version ${PHP_VERSION} (using ${PHP_CONFIG_EXECUTABLE})")
  ENDIF(PHP_EXECUTABLE AND PHP_INCLUDES)

  FIND_PROGRAM(PHPUNIT_EXECUTABLE
    NAMES phpunit phpunit2
    PATHS
    /usr/local/bin
  )

  IF(PHPUNIT_EXECUTABLE)
    MESSAGE(STATUS "Found phpunit: ${PHPUNIT_EXECUTABLE}")
  ENDIF(PHPUNIT_EXECUTABLE)

#ENDIF(PHP_CONFIG_EXECUTABLE)
