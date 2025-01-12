//Motor pins
#define rmf 10  //IN1
#define rmb 6  //IN2
#define lmf 8  //IN3
#define lmb 9  //IN4
//EnA and EnB
#define rms 3  //EnA
#define lms 5  //EnB

int sensor[5];  //to store the sensor value
int threshold = 512; //threshold = (minimum analog value + Maximum Analog Value) / 2
float c;
int left_motor_speed = 200, right_motor_speed = 200;
int left_motor, right_motor;
int kp = 40, kd = 1000;
int PID_value;
float current_error, previous_error;
int turn_speed = 120;
char t;

//push buttons
int button1 = 7;
int button2 = 2 ;
int button3 = 11;
int button4 = 12;
bool button1_state, button2_state, button3_state, button4_state;

void setup() {
  //motor driver pins as output
  pinMode(lmf, OUTPUT);
  pinMode(lmb, OUTPUT);
  pinMode(rmf, OUTPUT);
  pinMode(rmb, OUTPUT);
  
  //button pin as input
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  button_status();  //sensor_reading the status of push button

  //push button1 function : the car will start following the line
  while (button1_state != 1) {
    Line_Follow();  //line follow using pid
  }

  /*push button2 function :
  to check if both the motors are running in the same speed.
  if yes, the car will go straight. else it'll either go left or right */
  while (button2_state != 1) {
    motor(250, 250);
    delay(3000);
    motor(0, 0);
    button_status();
  }

  //push button3 function: stop mode
  while (button3_state != 1) {
    motor(0, 0);
    button_status();
  }

  //push button4 function : to check the analog values
  while (button4_state != 1) {
    (button3_state = digitalRead(button3) != 1) ? button3_state = 0, button4_state = 1 : button4_state = 0;
    show_analog_value();
  }
}

void sensor_reading() {
  float a;
  float b;
  
  // Loop to read values from the 5 channel IR sensor array connected to A3 to A7
  for (int i = 0; i < 5; i++) {
    sensor[i] = analogRead(A3 + i);  // Read analog values from A3 to A7
    
    /* Threshold logic: If the sensor value is above the threshold, set it to 1 (indicating line detected),
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

void Line_Follow() {
  while (1) {
    sensor_reading();  // Read the sensor values

    // Straight Line Follow (Using the center sensors: 2 and 3)
    if (sensor[2] == 1 || sensor[3] == 1) {
      current_error = 2.0 - c;  // The ideal position is in the middle (sensor[2] and sensor[3])
      PID_value = current_error * kp + kd * (current_error - previous_error);
      previous_error = current_error;
      right_motor = right_motor_speed - PID_value;
      left_motor = left_motor_speed + PID_value;
      motor(left_motor, right_motor);  // Set motor speeds to follow the line
    }
    
    // All sensors on white surface (no line detected)
    if (sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 0 && sensor[3] == 0 && sensor[4] == 0) {
      if (t == 'r') right();  // Turn right if the 'r' flag is set
      else if (t == 'l') left();  // Turn left if the 'l' flag is set
      else U_turn();  // Otherwise, U-turn
    }

    // Right Turn: If the rightmost sensor (sensor[4]) is detecting the line and the leftmost (sensor[0]) is not
    if (sensor[4] == 0 && sensor[0] == 1) 
       t = 'r';

    // Left Turn: If the leftmost sensor (sensor[0]) is detecting the line and the rightmost (sensor[4]) is not
    if (sensor[4] == 1 && sensor[0] == 0)
       t = 'l';

    // All sensors on the black surface (line detected on all sensors)
    if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 1) {
      delay(30);  // Small delay to stabilize the sensor readings
      sensor_reading();  // Re-read the sensor values
      if ((sensor[0] + sensor[1] + sensor[2] + sensor[3] + sensor[4]) == 5) {
        motor(0, 0);  // Stop the robot if all sensors detect the line
        while ((sensor[0] + sensor[1] + sensor[2] + sensor[3] + sensor[4]) == 5) sensor_reading();  // Wait until the robot is no longer over the line
      } 
      else if ((sensor[0] + sensor[1] + sensor[2] + sensor[3] + sensor[4]) == 0)
        t = 'r';  // If no sensor detects the line, turn right
    }
  }
}

void right() {
  while (1) {
    motor(turn_speed, -turn_speed);
    while (sensor[2] == 0 && sensor[3] == 0) sensor_reading();
    motor(-turn_speed, turn_speed);
    delay(20);
    break;
  }
}
void left() {
  while (1) {
    motor(-turn_speed, turn_speed);
    while (sensor[2] == 0 && sensor[3] == 0) sensor_reading();
    motor(turn_speed, -turn_speed);
    delay(20);
    break;
  }
}
void U_turn() {
  while (1) {
    delay(120);
    motor(-turn_speed, turn_speed);
    while (sensor[2] == 0 && sensor[3] == 0)
     sensor_reading();
    motor(turn_speed, -turn_speed);
    delay(20);
    break;
  }
}

//sensor_reading the status of push button and saving the state
void button_status() {
  button1_state = digitalRead(button1);
  button2_state = digitalRead(button2);
  button3_state = digitalRead(button3);
  button4_state = digitalRead(button4);
}

//motor run function
void motor(int a, int b) {
  if (a > 0) {
    digitalWrite(lmf, 1);
    digitalWrite(lmb, 0);
  } else {
    a = -(a);
    digitalWrite(lmf, 0);
    digitalWrite(lmb, 1);
  }
  if (b > 0) {
    digitalWrite(rmf, 1);
    digitalWrite(rmb, 0);
  } else {
    b = -(b);
    digitalWrite(rmf, 0);
    digitalWrite(rmb, 1);
  }
  if (a > 250) a = 250;
  if (b > 250) b = 250;

  analogWrite(lms, a);
  analogWrite(rms, b);
}

void show_analog_value() {
  // Loop through all the 5 sensors (A3, A4, A5, A6, A7)
  for (short int i = 0; i < 5; i++) {
    Serial.print(String(analogRead(A3 + i)) + " "); // Read analog values from A3 to A7 and print them
  }
  delay(100);  // Small delay to make the output readable
  Serial.println();  // Move to the next line after printing all the sensor values
}




