#!/bin/bash

curl -X PUT -H "Content-Type: application/json" 'http://localhost:8080/complete' -d '{"task_id": 0}' -i -v

