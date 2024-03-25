#!/bin/bash

curl -H "Content-Type: application/json" http://localhost:8080/api/new -d '{"title": "do dinner","description": "Pesto Pasta"}' -i -v

