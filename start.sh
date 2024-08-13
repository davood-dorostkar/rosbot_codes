#!/bin/bash

is_running=$(docker ps --filter "name=rosbot_xl" --format "{{.ID}}")
is_stopped=$(docker ps -a --filter "name=rosbot_xl" --filter "status=exited" --format "{{.ID}}")

if [ -n "$is_running" ]; then
    echo "rosbot container is running..."

elif [ -n $is_stopped ]; then
    echo "Starting rosbot container..."
    docker-compose start

else    
    echo "Creating rosbot container..."
    docker-compose up -d
fi
 
xhost local:root && docker-compose exec rosbot_sim bash
