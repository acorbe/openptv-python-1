#!/bin/bash

INCLUDE_FOLDER_=/usr/local/include
LIB_FOLDER_=/usr/local/lib

cd ..

python setup.py  build_ext --inplace -I $INCLUDE_FOLDER_  -L $LIB_FOLDER_

#is this necessary?
python setup.py  install 
