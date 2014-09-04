#!/bin/bash

if (( $# != 1 ))
then
  echo "Usage: ..."
  echo $0 "bd"
  exit 1
fi

echo Making Directory $1
mkdir $1

echo Setting Environment Variables
export FLYCAPTURE2_ROOT_PATH=/usr

echo Generating Make Files
cd $1
cmake -DALVAR_USE_FLYCAPTURE2:BOOL=TRUE ../

echo Making Source
make -j8

echo Copying Plugins
mkdir sample/alvarplugins
cd sample/alvarplugins
cp ../../src/platform/capture_plugin_*/libalvarcaptureplugin*.so .

echo Ready
