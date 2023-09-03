#!/usr/bin/bash

trap exit SIGQUIT SIGINT

# arguments
while test $# != 0
do
    case "$1" in
    -i|--install) INSTALL_DEPENDENCIES=t ;;
    -w|--watch) WATCH=t ;;
    esac
    shift
done

# install package manager
if [ ! -d "vcpkg" ] || [ ! -z "${INSTALL_DEPENDENCIES}" ]; then
    echo "==Installing vcpkg=="
    git clone https://github.com/Microsoft/vcpkg.git
    chmod +x ./vcpkg/bootstrap-vcpkg.sh
    ./vcpkg/bootstrap-vcpkg.sh
    ./vcpkg/vcpkg install glfw3
    ./vcpkg/vcpkg install glad
fi

function configure() {
    echo "==Configuring project=="
    mkdir -p build
    cd build
    cmake ..
    cd ..
}

# build project
function build() {
    echo "==Building project=="
    mkdir -p build
    cd build
    cmake --build .
    cd ..
}

# perform the build
if [ -z "${WATCH}" ]
then
    configure
    build
else
    configure
    while :; do
        (watch -n1 -t -g ls -l ./src/*) > /dev/null && build
    done
fi