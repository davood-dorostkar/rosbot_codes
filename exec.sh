#!/bin/bash

docker-compose exec rosbot_sim bash -c "pgrep -f gazebo | xargs kill -9"
xhost local:root && docker-compose exec rosbot_sim bash
