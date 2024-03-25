#!/bin/bash

set -xe

for d in `docker ps -aq`; do docker container rm $d; done
