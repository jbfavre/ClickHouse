#!/bin/bash

set -o errexit
set -o pipefail

# Start a local clickhouse server which will be used to run tests
PWD=$(pwd)
cd debian/tmp
PATH=$PATH:/home/dev/Debian/clickhouse/debian/tmp/usr/bin \
  ./usr/bin/clickhouse-server --config-file=../clickhouse-server-config-tests.xml
CH_PID=$!
cd ${PWD}

# Define needed stuff to kill test clickhouse server after tests completion
function finish {
  kill $CH_PID
  wait
}
trap finish EXIT

# Do tests
PATH=$PATH:/home/dev/Debian/clickhouse/debian/tmp/usr/bin \
  ./dbms/tests/clickhouse-test -c debian/tmp/usr/bin/clickhouse-client
