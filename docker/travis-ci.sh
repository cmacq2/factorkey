#!/bin/sh
set -e

echo_eval ()
{
    echo $@
    eval $@
}

echo "SANITY CHECK: image is: '$image'"
if [ -n "$image" ]
then
    echo "Number of cores available on the build host: $(nproc)"
    echo_eval mkdir -p build
    echo_eval docker pull \"$image\"
    echo_eval docker run -t -v \"$(pwd):/factorkey/src\" -v \"$(pwd)/build:/factorkey/build\" \"$image\"
else
    echo "Image required!"
    exit 1
fi
