#!/bin/sh

set -e

echo_eval ()
{
    echo $@
    eval $@
}

echo "Number of cores available for the build: $(nproc)"
echo_eval cd /factorkey/build
echo_eval cmake /factorkey/src -GNinja -DINCLUDE_SAMPLE_APPS=1 -DCMAKE_INSTALL_PREFIX=/factorkey/dist $@
echo_eval ninja
echo_eval ninja test

