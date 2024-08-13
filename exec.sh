#!/bin/bash
container_id=$(docker ps -a --filter "name=rosbot_xl" --format "{{.ID}}")

if [ -z "$container_id" ]; then
    echo "Creating rosbot container..."
    docker-compose up -d
    container_id=$(docker ps -a --filter "name=rosbot_xl" --format "{{.ID}}")
fi

is_stopped=$(docker ps --filter "id=$container_id" --format '{{.ID}}')

if [ -z "$is_stopped" ]; then
    echo "Starting rosbot container..."
    docker-compose start
else
    echo "Container is running."
fi

xhost local:root && docker-compose exec rosbot_sim bash 