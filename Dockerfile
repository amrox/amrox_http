FROM conanio/clang9:1.21.1

COPY . /home/conan/src
WORKDIR /home/conan/build
RUN conan install ../src --build=missing
RUN cmake ../src
RUN cmake --build .
CMD ctest

# The build works locally but I get the following errors in the container
# and I'm just tired of build errors.
#
# conanfile.txt: Generator cmake_find_package created Finddocopt.cpp.cmake
# WARN: Remotes registry file missing, creating default one in /home/conan/.conan/remotes.json
# conanfile.txt: ERROR: Generator cmake_find_package(file:None) failed
# [Errno 13] Permission denied: '/home/conan/build/Finddocopt.cpp.cmake'
# ERROR: [Errno 13] Permission denied: '/home/conan/build/Finddocopt.cpp.cmake'
# The command '/bin/sh -c conan install ../src --build=missing' returned a non-zero code: 1
