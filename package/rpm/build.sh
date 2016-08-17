#!/bin/bash

IN=$(cat ../VERSION)
set -- "$IN"
IFS="."; declare -a Array=($*)

VERSION=${Array[0]}
RELEASE=${Array[1]}

if [[ -z "$RELEASE" ]]; then 
RELEASE=0
fi;

mkdir -p SOURCES
tar -C ../ -czvf SOURCES/websock_rtsp_proxy-$VERSION.$RELEASE.tar.gz src/ install/ CMakeLists.txt

rpmbuild SPECS/websock_rtsp_proxy.spec --verbose -ba --define "_topdir $PWD" --define "_version $VERSION" --define "_release $RELEASE"

