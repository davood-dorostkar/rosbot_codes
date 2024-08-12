#!/bin/bash

search_path=/ros2_ws/codes
all_dirs=$(find "$search_path" -maxdepth 1 -type d)
filtered_dirs=$(echo "$all_dirs" | grep -v "^$search_path$")

for dir in $filtered_dirs; do
    rsync -aP "$dir" /ros2_ws/src/
done