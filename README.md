# Arduino Line Follower Car

## **Project Overview**
This project involves designing and programming an **Arduino-based Line Follower Car**. 
The car uses a 5-channel array of IR sensors to detect and follow a black line on a white surface. The motors are controlled via a PID algorithm.

---

## **Hardware**

  1.	  Arduino Nano
  2.	  5 channel IR sensor array
  3.	  L298N motor driver 
  4.	  Buck and Boost converters 
  5.	  4 push buttons
  6.	  T- connector
  7.	  Battery holder
  8.	  N20 gear motors: 300rpm
  9.	  Lithium ion battery: 3.7V
 10.	  N20 ball caster, N20 wheels and N20 motor clamps
 11.	  A Veroboard
 12.	  Female header pins 


---

## **Software**
1. Arduino IDE (for uploading the code)

---
## **Features**

1. **Microcontroller (the brain)**:
   - Arduino Nano, a microcontroller that processes inputs from the sensors and generates appropriate control signals for the motors.
2. **Line detection**:
   - A 5-channel IR sensor array is used for line detection, where the sensors detect the difference in reflectivity between the line and the surface, sending data to the Arduino for path correction.
3. **Algorithm used**:
   - Implements a PID control algorithm to follow a line precisely.
4. **Power**:
   - Powered by N20 gear motors with a speed of 300 RPM, which provide precise and efficient movement due to their compact size and high torque.
5. **Motor control**:
   - The motors are controlled by an L298N motor driver, which acts as an interface between the motors and the control signals, allowing bidirectional movement and speed adjustment. 
6. **Power Management**:
   - Handled by buck and boost converters, which regulate the voltage to ensure the components receive a stable and suitable power supply, regardless of fluctuations. 




---


## **Code Highlights**
### **PID Control Logic**
```cpp
if (sensor[2] == 1 || sensor[3] == 1) {
      current_error = 2.0 - c;  // The ideal position is in the middle (sensor[2] and sensor[3])
      PID_value = current_error * kp + kd * (current_error - previous_error);
      previous_error = current_error;

      right_motor = right_motor_speed - PID_value;
      left_motor = left_motor_speed + PID_value;
      motor(left_motor, right_motor);  // Set motor speeds to follow the line
    }
```

### **Sensor Reading Function**
```cpp
void sensor_reading() {
  float a;
  float b;
  
  // Loop to read values from the 5 channel IR sensor array connected to A3 to A7
  for (int i = 0; i < 5; i++) {
    sensor[i] = analogRead(A3 + i);  // Read analog values from A3 to A7
    /* Threshold logic: 
       If the sensor value is above the threshold, set it to 1 (indicating line detected),
       else set it to 0 */
    if (sensor[i] > threshold) {
      sensor[i] = 1;
    } else {
      sensor[i] = 0;
    }
  }

  // Calculate weighted sum (a) and total (b) for computing the average position (c)
  a = (sensor[0] * 1 + sensor[1] * 2 + sensor[2] * 3 + sensor[3] * 4 + sensor[4] * 5);
  b = (sensor[0] + sensor[1] + sensor[2] + sensor[3] + sensor[4]);

  // Calculate the center of mass (c) to determine where the line is in relation to the sensors
  if (b > 0) {
    c = a / b;  // This gives a weighted average to determine the line position
  }
}

```
---

## **Significance**
- Demonstrates the basic principles of robotics, such as sensor-based navigation and automation. 
- Widely used in industries for tasks like material handling and process automation, improving efficiency and reducing manual effort. 
- Serves as an educational tool and a platform for innovation, inspiring advancements in autonomous systems and robotics research.


---
## **Contribution**
Feel free to fork this repository and submit a pull request for any improvements or bug fixes.

---


