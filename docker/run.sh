#!/bin/bash

set -xe

docker run -dt -p8080:80 -v $(pwd)/data:/app/data --name civetweb civetweb:1.0
