# logic-action-steering

This service is responsible for generating the appropriate rack position (of the linear actuator) as a function of the current AimPoint. The rack position is encapsulated in a message of type GroundSteeringRequest. 

### Messages:

- **In:**
  - AimPoint - Either describes the aimpoint of the car (i.e. where the vehicle is currently aiming to go) or it's describing the reference steering wheel angle (only AzimuthAngle property of AimPoint is used)
- **Out:**
  - GroundSteeringRequest

###  How it works:

The steering angle (delta) of the front wheels is calculated using proportional control with the azimuth angle of the AimPoint as reference signal. Then, the rack position offset is calculated by linear conversion from delta.

### Improve:

- Parameter values (such as *tol*, *Kp*) are hard-coded. Should be passable from command line.
- calcSteering takes delta and distance, but only uses delta. (Uses void cast to suppress warning)
- Update m_latestMessage (currently only instanciated once)

### Questions:

- What is the purpose of the if-statement that compares rackPosition to m_prevPos 
- Is linear conversion of delta to rackPosition appropriate?



## logic-action-x

In our car, we have three main components that we can control to make the car move: The linear actuator (steering), the brakes (braking) and the inverters (motion).

The logic-action-x services are responsible for calculating an appropriate control signal for their respective components given some reference signal(s) sent from the main cognition service (cognition-acceleration or cognition-track).

There are two different OD4Sessions in each service: One that is responsible for receiving messages from cognition-services, and one that is responsible for sending messages to low-level services.

**To improve**: We believe that the solution to have three independent microservices responsible for generating the control signals for each of the control components (steering, inverters, brakes) is restrictive. As it is now, it's hard to implement a control scheme where different control signals are dependent by design.

**To improve**: The naming of the OD4Session which communicates to the low-level stuff (StateMachine, aka. BeagleBone) is very inconsistent between the logic-action services. Basically, od4_proxy (steering), od4Brakes (braking) and od4StateMachine (motion) all live on the same conference with defaullt CID=219.