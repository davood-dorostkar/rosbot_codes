#!/bin/bash

search_path=/ros2_ws/src
exclude_dirs=".git rosbot_xl_controller rosbot_hardware_interfaces ros_components_description rosbot_xl rosbot_controllers \
.github rosbot_xl_description husarion rosbot_xl_gazebo rosbot_xl_bringup gazebosim log build install"

all_dirs=$(find "$search_path" -maxdepth 1 -type d)
filtered_dirs=$(echo "$all_dirs" | grep -v "^$search_path$")
filtered_dirs=$(echo "$filtered_dirs" | grep -vE "$(echo "$exclude_dirs" | sed 's/ /|/g')")

for dir in $filtered_dirs; do
    rsync -aP "$dir" /codes/
done
