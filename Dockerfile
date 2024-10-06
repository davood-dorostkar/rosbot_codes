ARG ROS_DISTRO=humble
FROM husarion/rosbot-xl-gazebo:humble

SHELL ["/bin/bash", "-c"]

RUN apt-get update --fix-missing && apt-get install -y \
    python3-pip \
    python3-colcon-common-extensions \
    python3-rosdep \
    ros-$ROS_DISTRO-teleop-twist-keyboard \
    ros-$ROS_DISTRO-rqt-graph \
    ros-$ROS_DISTRO-rviz2 \
    ros-$ROS_DISTRO-plotjuggler-ros \
    ros-$ROS_DISTRO-image-view \
    ros-$ROS_DISTRO-cv-bridge \
    libopencv-dev \
    rsync \
    sudo 

RUN useradd -m -s /bin/bash -G sudo davood
RUN echo "davood:1234" | chpasswd 
RUN chown davood:davood -R /ros2_ws /ros_entrypoint.sh
USER davood
RUN echo "source /ros_entrypoint.sh" >> ~/.bashrc
RUN echo "set +e" >> ~/.bashrc
RUN echo "alias sim=\"ros2 launch rosbot_xl_gazebo simulation.launch.py mecanum:=${MECANUM:-False} camera_model:=intel_realsense_d435\"" >> ~/.bashrc
