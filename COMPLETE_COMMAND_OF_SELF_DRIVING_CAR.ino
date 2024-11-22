//define variables for motor control pins
int leftmotorforwardpin = 11;
int leftmotorbackwardpin = 12;
int rightmotorforwardpin = 9;
int rightmotorbackwardpin = 10;
int leftmotorspeedpin = 6;
int rightmotorspeedpin = 5;

//define ir sensor pins
const int leftsensorpin = A0;
const int rightsensorpin = A1;

//threshold for analog reading of ir sensor
const int irthreshold = 200;

//declare variables for input ir sensor pin state
int leftpinstate;
int rightpinstate;

//declare constant for led pins
const int leftledpin = 13;
const int rightledpin = 2;

//define variablesfor ultrasonic sensor pins as constants
const int triggerpinn = 8;
const int echopin = 7;
const int threshold = 30;

//define variables for the ping and distance
long duration; //time for echo pin to return in microseconds
long cm; //distance in cm

void setup(){
  //motorpins as output
  pinMode(leftmotorforwardpin,OUTPUT);
  pinMode(leftmotorbackwardpin,OUTPUT);
  pinMode(rightmotorforwardpin,OUTPUT);
  pinMode(rightmotorbackwardpin,OUTPUT);
  pinMode(leftmotorspeedpin,OUTPUT);
  pinMode(rightmotorspeedpin,OUTPUT);

  //ultrasonic sensor pins as input or output
  Serial.begin(9600);
  pinMode(triggerpinn,OUTPUT);
  pinMode(echopin,INPUT);

  //set ir sensor pin as input with internal pullup resistor
  pinMode(leftsensorpin,INPUT_PULLUP);
  pinMode(rightsensorpin,INPUT_PULLUP);

}

//Forward Motion
void forward(int delay_time,int left_speed,int right_speed){ //choice of delay and time
  //spin forward
  analogWrite(leftmotorspeedpin, left_speed); //choice of speed
  analogWrite(rightmotorspeedpin, right_speed); //choice of speed
  digitalWrite(leftmotorforwardpin,HIGH);
  digitalWrite(leftmotorbackwardpin,LOW);
  digitalWrite(rightmotorforwardpin,HIGH);
  digitalWrite(rightmotorbackwardpin,LOW);
}

//Backward Motion
void backward(int delay_time,int left_speed,int right_speed){ //choice of delay and time
  //spin backward
  analogWrite(leftmotorspeedpin, left_speed); //choice of speed
  analogWrite(rightmotorspeedpin, right_speed); //choice of speed
  digitalWrite(leftmotorforwardpin,LOW);
  digitalWrite(leftmotorbackwardpin,HIGH);
  digitalWrite(rightmotorforwardpin,LOW);
  digitalWrite(rightmotorbackwardpin,HIGH);
}

//Turn Left Motion
void turnleft(int delay_time,int left_speed,int right_speed){ //choice of delay and time
  //turn left
  analogWrite(leftmotorspeedpin, left_speed); //choice of speed
  analogWrite(rightmotorspeedpin, right_speed); //choice of speed
  digitalWrite(leftmotorforwardpin,LOW);
  digitalWrite(leftmotorbackwardpin,HIGH);
  digitalWrite(rightmotorforwardpin,HIGH);
  digitalWrite(rightmotorbackwardpin,LOW);
}

//Turn Right Motion
void turnright(int delay_time,int left_speed,int right_speed){ //choice of delay and time
  //turn right
  analogWrite(leftmotorspeedpin, left_speed); //choice of speed
  analogWrite(rightmotorspeedpin, right_speed); //choice of speed
  digitalWrite(leftmotorforwardpin,HIGH);
  digitalWrite(leftmotorbackwardpin,LOW);
  digitalWrite(rightmotorforwardpin,LOW);
  digitalWrite(rightmotorbackwardpin,HIGH);
}

//Stop
void stop(int delay_time,int left_speed,int right_speed){ //choice of delay and time
  //stop
  analogWrite(leftmotorspeedpin, left_speed); //choice of speed
  analogWrite(rightmotorspeedpin, right_speed); //choice of speed
  digitalWrite(leftmotorforwardpin,LOW);
  digitalWrite(leftmotorbackwardpin,LOW);
  digitalWrite(rightmotorforwardpin,LOW);
  digitalWrite(rightmotorbackwardpin,LOW);

}

void loop(){
  //read the ir sensorpins
  leftpinstate = analogRead(leftsensorpin); //read the ir sensorpin
  leftpinstate = analogRead(rightsensorpin); //read the ir sensorpin

  //give a short low pulse to ensure a clean high pulse
  digitalWrite(triggerpinn,LOW);
  delayMicroseconds(2);
  digitalWrite(triggerpinn,HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerpinn,LOW);

  //use pulseIN command to measure the duration of echopin in microseconds
  duration = pulseIn(echopin,HIGH);

  //convert the time into a distance in cm
  cm = duration/58;

  //stop if distance is less than threshold, otherwise drive forward
  if((leftpinstate > irthreshold) && (rightpinstate > irthreshold)){
    digitalWrite(leftledpin,LOW); //turn led off
    digitalWrite(rightledpin,LOW); //turn led off
    forward(1000,200,200);
  }
  if((leftpinstate < irthreshold) && (rightpinstate > irthreshold)){
    digitalWrite(leftledpin,HIGH); //turn led on
    digitalWrite(rightledpin,LOW); //turn led off
    turnright(100,50,50);
  }
  if((leftpinstate > irthreshold) && (rightpinstate < irthreshold)){
    digitalWrite(leftledpin,LOW); //turn led off
    digitalWrite(rightledpin,HIGH); //turn led on
    turnleft(100,50,50);
  }
  if((leftpinstate < irthreshold) && (rightpinstate < irthreshold)){
    digitalWrite(leftledpin,HIGH); //turn led on
    digitalWrite(rightledpin,HIGH); //turn led on
    stop(2000,0,0);
    forward(1000,200,200);
  }
  if(cm <= threshold){
    digitalWrite(leftledpin,HIGH); //turn led on
    digitalWrite(rightledpin,HIGH); //turn led on
    stop(1000,0,0);
  }
  

  //print measurement to Serial monitor
  if(cm > threshold){
    Serial.print("top speed");
    Serial.println("distance is");
    Serial.print( cm);
    Serial.print("cm");
    Serial.println();
  }
  if(cm <= threshold){
    Serial.print("brake! stop!");
    Serial.println("distance is");
    Serial.print( cm);
    Serial.print("cm");
    Serial.println();
  }
  
  delay(100);
}



