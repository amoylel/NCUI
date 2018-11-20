#!/bin/bash
PRJ=`pwd`
echo "当前位置:"$PRJ

if [ $1 ];then
    tmp=$1
else
    tmp="Debug"
fi

cd -
echo "当前位置:"`pwd`

if [ "$tmp" = "Debug" ];then
    gyp -d --depth=. amo.gyp
else
    gyp --depth=. amo.gyp
fi

make -j 4 BUILDTYPE=$tmp




#cp $PRJ/out/Release/obj.target/*.so $PRJ/out/Release/plugins
