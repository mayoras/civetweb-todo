#!/bin/bash

set -xe

docker run -dt -p8080:8080 --name civetweb civetweb:1.0
