# Copyright (c) 2014, Ford Motor Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.


set(SMART_OBJECTS_SRC_DIR ${COMPONENTS_DIR}/smart_objects/src)

include_directories (
  include
  ${COMPONENTS_DIR}/utils/include/
)

set (SOURCES
    ${SMART_OBJECTS_SRC_DIR}/smart_object.cc
    ${SMART_OBJECTS_SRC_DIR}/smart_schema.cc
    ${SMART_OBJECTS_SRC_DIR}/schema_item.cc
    ${SMART_OBJECTS_SRC_DIR}/always_false_schema_item.cc
    ${SMART_OBJECTS_SRC_DIR}/always_true_schema_item.cc
    ${SMART_OBJECTS_SRC_DIR}/bool_schema_item.cc
    ${SMART_OBJECTS_SRC_DIR}/number_schema_item.cc
    ${SMART_OBJECTS_SRC_DIR}/string_schema_item.cc
    ${SMART_OBJECTS_SRC_DIR}/object_schema_item.cc
    ${SMART_OBJECTS_SRC_DIR}/array_schema_item.cc
)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  file (GLOB inc ${COMPONENTS_DIR}/smart_objects/include/smart_objects/*.h) 
  string( REPLACE / \\ group ${inc} )
  source_group("Header Files" FILES ${inc} )
  add_library("SmartObjects" ${inc} ${SOURCES})
  target_link_libraries("SmartObjects")
else ()
  add_library("SmartObjects" ${SOURCES})
  if(ENABLE_LOG)
    target_link_libraries("SmartObjects" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
  endif()
endif()

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

