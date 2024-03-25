#!/bin/bash

set -xe

if [[ $# -ne 1 ]]; then
    TAG=1.0
else
    TAG=$1
fi;


docker build -t civetweb:$TAG .
