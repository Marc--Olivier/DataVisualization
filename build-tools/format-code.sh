#!/bin/sh

pushd $(dirname "$0") > /dev/null
ROOT_DIR=`pwd -P`/..
#find $ROOT_DIR \( -name "*.cpp" -o -name "*.h" \) -exec clang-format -style=file -i {} \;

filetypes="*.cpp *.h"
for filetype in $(echo $filetypes)
do
  find $ROOT_DIR -name $filetype -exec clang-format -style=file -i {} \;
done

popd > /dev/null