FROM husarion/rosbot-xl-gazebo:humble
RUN apt-get update && apt-get install -y rsync
RUN useradd -m -s /bin/bash -G sudo davood
RUN echo "davood:1234" | chpasswd 
RUN chown davood:davood -R /ros2_ws /ros_entrypoint.sh
USER davood
WORKDIR /ros2_ws/src/
RUN echo "source /ros_entrypoint.sh" >> ~/.bashrc