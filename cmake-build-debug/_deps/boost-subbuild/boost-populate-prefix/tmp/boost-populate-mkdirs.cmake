# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "F:/Users/User/Documents/СPPProjects/WeeklyScript/cmake-build-debug/_deps/boost-src"
  "F:/Users/User/Documents/СPPProjects/WeeklyScript/cmake-build-debug/_deps/boost-build"
  "F:/Users/User/Documents/СPPProjects/WeeklyScript/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix"
  "F:/Users/User/Documents/СPPProjects/WeeklyScript/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix/tmp"
  "F:/Users/User/Documents/СPPProjects/WeeklyScript/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix/src/boost-populate-stamp"
  "F:/Users/User/Documents/СPPProjects/WeeklyScript/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix/src"
  "F:/Users/User/Documents/СPPProjects/WeeklyScript/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix/src/boost-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/Users/User/Documents/СPPProjects/WeeklyScript/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix/src/boost-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "F:/Users/User/Documents/СPPProjects/WeeklyScript/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix/src/boost-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
