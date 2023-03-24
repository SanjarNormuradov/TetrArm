
# Final Project "Toy Factory"



## Overview
The initial goal of this project was to create a toy factory combining the popular game "Tetris" with a 3-DoF robotic arm to pich randmoly created figures from a conveyor and put them into a box where "Tetris" scenario happens. 

**But** due to limitations of time and methods provided by an emerging multi-everything simulator [ENVIRO](https://github.com/klavinslab/enviro), the current goal of the project has been shrinked to picking up a randomly-coloured block from the conveyor with a gripper and putting it to the box. As long as the block touches the bottom of the box, it disappears which shows how the toy factory thrives thanks to children's, and not only, endless desire to feed their imagination...

As for the control, there are 3 screen buttons (which names speak for themselves): *"Emergency Stop"*, *"Start Conveyor"* and _"Stop Conveyor"_. However, the workflow of the conveyor is a bit interesting: it has at its front a sensor, which measures a distance to the first-in-row block and stops the conveyor if the block close enough (block's inertial properties accounted for non-zero stop-distance). Only the _"Emergency Stop"_ button is capable to change the state of the conveyor (i.e. stop completely). Otherwise, the conveyor starts automatically if the first-in-row block is removed by the gripper. The gripper, whereas, is entirely dependent on the user commands: keys `←` `→` `↑` `↓` controls its movement directions, `Shift` enables turbo speed mode and `Space` once pressed turns on gripping (turns off automatically if the block touches the bottom of the box). Turning the gripper off while the block is on the air and letting it freely fall down was expected, but not achieved... Sorry for less fun :(
![](/ENVIRO_ToyFactory.jpg)

## Challenges
There were lots of challenges related to *logic* and *simulation environment-specific*.

**Simulation environment-specific**

- Agent's shape defined in its _.json_ file:
  - Could be of two contradictory types: circular or polygon-shaped. So, you cannot create a conveyor belt (oval-shaped line). 
    - _Solution_ (partly): Create polygon-shaped __one__ line (which collide with other agents) and `decorate()` it with arcs and the other line. Decorations are _noninteractive_ type objects, which means other agents can easily pass through them without collision (which is good and bad). Good, because you can simulate complex agents (a robotic arm) without colliding their parts, and bad, because if any agent by chance accesses the complex agent's inner part where there might be other agents (for example, a conveyor belt and gears inside it in this project) - the intruder would mess up everything... And `move_toward()` method could help no way, though there are more to speak about that method too...
  - Polygon-shaped agent - a square room with one wall missing, for example a "toy box" in this project, would occupy the square space... So, other agents cannot access "seemingly free" space.
    - _Solution_ (partly): Decorate it! But, again, decorations are _noninteractive_. Otherwise, if you need your agent (three-wall room) to be with physical properties (collision, friction, mass), create each part as an independent agent and attach to each other with `attach_to()`. Do not forget to `prevent_rotation()` each agent if you do not want them move around one another, because `attach_to()` keeps the agents' centers at particular distance (circle's radius). __But__ if you create a complex agent, composed of several agents attached to each other, __do not try to use `remove_agent()` method__ on them ("gripperCenter" attached to "objectGripper" which were initially meant to be removed from the simulation to tear the link with the gripped "buildingBlock" when it touches the bottom of the toy box, and added again at the same location without any link), because [ENVIRO](https://github.com/klavinslab/enviro) is not capable enough to manage complex multi-thread processes (_invisble_ type agents could `watch(Event())` but not `emit(Event())`, and even _static_ type agents doing nothing in the project could not manage multi-thread `Event()` calls)
- Methods:
  - `move_toward()` would "not move noticably" polygon-shaped agents with `prevent_rotation()` method called (which sets _rotational_ friction to infinity), even if you try to set agent's _linear_ friction to 1 and increase PID forward gain up to 100 000...
    - _Solution_ (worst case sceanrio): Try to avoid its usage with non-rotating polygon-shaped agents! But if you need to move those agents, try to `attach_to()` an omni-directional agent (attaching the polygon-shaped "objectGripper" to omni-directional "gripperCenter" in this project) which would solve the problem somehow (read more on `attach_to()` method).
  - `attach_to()` as mentioned earlier, keeps the attached agents' centers at a particular distance (circle's radius). 
    - _Solution_: Constrain agents centers' circular movement with agents' shapes of long geometry or move them syncronously (for instance in this project, including the polygon-shaped "objectGripper" of the same size as "buildingBlock" and implementing the same methods `track_velocity()` in "buildingBlock" and "objectGripper" `update()` methods respectively).
  - `track_velocity()` for polygon-shaped agents would move only in one direction unless rotation is allowed. But what if you need to keep an agent without rotation ( for example "objectGripper") and move it along X- and Y-axes? `move_toward()` method would not help you, as mentioned above, because of _rotational_ friction set to infinity.
   - _Solution_: `attach_to()` to an omni-directional agent ("gripperCenter") which provides you movement along Y-axis, but also implement `track_velocity()` with the same Vx velocity as in `omni_track_velocity()` so both agents would move syncronously.

__Logic-related__

- As the conveyor belt's sensor scans constantly and emits corresponding events when it detects whether there is a block or not, we need to highlight one specific scenario - belt is rotating with its sensor detecting no block in vicinity and new blocks are being generated at the rear end of the conveyor. As there is no blocks near the sensor, the latter would emit constantly (which is good as it should be) and the formers move, but the blocks controller should be notified only with the sensor scan change and not its every scan. Otherwise, the controller would be constantly interrupted, missing other events as attaching the block to the gripper or removing it when a collision with the toy box detected, or not missing any event but leaving less computational resources for other agents controllers and state machines.

- `event.stop_propagation()` method once implemented in the building block controller would stop the same event being watched by the rest same-type agents controllers. For example, there are three conveyor gears in this project, and if there is an event which triggers the gears' state change in their FSMs, implementing `event.stop_propagation()` in the FSM (conveyor_gear.h file) would work for only one gear and not for the rest two.

- Simple multi-thread events (gripping one block without affecting the rest) could be dealt with emitting events with "block_id" value field.
## Installation
Keeping in mind that the Docker container is already installed...

First, clone the git repo with

```bash
git clone https://github.com/SanjarNormuradov/EEP520_SWE_EmbApp/Assignments/Final_Project
```
Second, start the docker image environment with (v1.5 is preferred, as later versions are unstable and earlier ones do not contain new methods for futher modifications)
```bash
docker run -p80:80 -p8765:8765 -v $PWD/EE520_FinalProject:/source -it klavins/enviro:v1.5 bash
```
If this is the first time using this image file, it will be automatically downloaded.
## Run the Simulation
As long as the image is in use, you would see PWD as `root@#######:/source`
Then execute the following commands:

- To connect to [http://localhost](http://localhost/) where the simulation is rendered
```bash
esm start
```
- To make existing header and source files executable (shared object type files with `.so` extension)
```bash
make
```
- To start simulation
```bash
enviro
```
## Code
There are 7 agents in this project:

1. __conveyor_belt__: the main tasks are to notify _conveyor_gear_ about the sensor scan (detected object or no) and generate randomly-coloured blocks with a frequency of 3 seconds (no matter how many times "_Emergency Stop_", "_Start Conveyor_", "_Stop Conveyor_" are pressed to reset the counting stopwatch).
2. __conveyor_gear__ (3 instances): the main tasks are to visualize the rotating conveyor belt by its rotation and start/stop moving _building_block_.
3. __rail_road__: the main task is to move with the gripper along the X-axis of the factory's ceiling.
4. __object_gripper__: the main task is to grip a _building_block_ and transfer it from the _conveyor_belt_ to the _toy_box_.
5. __gripper_center__: the main task is to enable the object_gripper to move along Y-axis.
6. __toy_box__: the main task is to remove _building_block_ in touch.
7. __building_block__: the main task is to be a block, but with the project's most implementations such as moving along the _conveyor_belt_ or with _object_gripper_ and being removed once it collides with the _toy_box_.


## Acknowledgements

 - [ENVIRO: the multi-agent, multi-user, multi-everything simulator](https://github.com/klavinslab/enviro)
 - [Elma: An event loop manager for embedded systems](http://klavinslab.org/elma/)
