# Rosbot Simulator in Gazebo
it is based on the Husarion simulation in [this tutorial](https://husarion.com/tutorials/ros2-tutorials)

![](/rosbot-gazebo.png)

## Usage
run and enter container
```
./start.sh
```
to start the gazebo simulation, open a shell inside the container and run
```
sim
```
to run the second container and enter it
```
./client.sh
```
## Kinematics
the kinematics formulation in the tutorial is wrong. read this [guide](https://ucr-robotics.readthedocs.io/en/latest/tbot/moRbt.html) instead. unfortunately this very guide also has a typo. the correct formulation of a differential robot with 2 wheels is in short: 

![](/differential-robot.png)

$L$ : wheels distance<br />
$\theta$ : robot rotation in $x-y$ plate<br />
$V$ : robot velocity along its heading<br />
$\omega$ : wheel angular velocity<br />
$v_r$ , $v_l$ : right and left wheel

$$
V = \frac{v_r + v_l}{2} = \frac{r(\omega_r + \omega_l)}{2} \\
$$
$$
\omega = \frac{v_r-v_l}{L} = \frac{r(\omega_r - \omega_l)}{L}
$$
so the state space is:
$$
\dot{x} = V \cos{\theta}
$$
$$
\dot{y} = V \sin{\theta}
$$
$$
\dot{\theta} = \omega
$$
assume an initial value for each state, then start integrating over time.