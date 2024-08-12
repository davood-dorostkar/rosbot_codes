#!/bin/bash

package="$1"
clean_path="${package%/}"
rsync -aP /ros2_ws/codes/$clean_path /ros2_ws/src/

search_path=/ros2_ws/codes
all_dirs=$(find "$search_path" -maxdepth 1 -type d)

for dir in $all_dirs; do
    rsync -aP "$dir" /ros2_ws/src/
done