#include <Arduino.h>

 /*
                            JAI SHREE RAM
 DEVELOPED BY JOY:          INSTRUCTIONS...
 (i)adjusting the sensor distances----->find{ctrl+f} "adjust" it will lead you to the code line
 (iifor testing the sensors S
      step-1> paste these line of codes in "loop" function 
          detectObstacle() ;                         
          Garage_sensorgatestate();
          Blynk.virtualWrite(V5,"\n\n"); 
      step-2>uncomment by finding "testing ultra sonic sensors." in detectobstracle() & Garage_sensorgatestate()
          */

      


#define BLYNK_TEMPLATE_ID "TMPL3PVYwPnqR"
#define BLYNK_TEMPLATE_NAME "Switch"
#define BLYNK_AUTH_TOKEN "CD3lfUD_9TY75GvIjFhMABy3DqwibQVI"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <NewPing.h>

#include <Preferences.h>//try2
Preferences pref;//try2



int sensorcheckup=0;//test
const int buzzerPin = 1;



//************** MAIN GATE START ********************
const int motor1Pin1 = 2;
const int motor1Pin2 = 4;
const int motor2Pin1 = 15;
const int motor2Pin2 = 5;
const int relayPin = 25;           // Pin connected to the relay module
const int trigPin =16;                // Ultrasonic sensor trigger pin
const int echoPin =17;                // Ultrasonic sensor echo pin

//STATE maintainance start
int gateState=0;  // 0: Closed, 1: Opened
bool operationInProgress = false;  // Flag to track if an operation is in progress
bool usSensorState = false;  // State of the ULTRA SONIC sensor


//STATE maintainance end

// Define push button pins for garage control
const int buttonV5Pin = 21;  // Replace with the actual pin number for your button
const int buttonV6Pin = 22;  // Replace with the actual pin number for your button
const int buttonVbothPin=23;
// Add global variables for garage push buttons
bool buttonV5State;
bool buttonV6State;
bool buttonVbothState;
//************** MAIN GATE END********************

//************** GARAGE ROLL UP START********************
const int garage_motor1Pin1=32;
const int garage_Motor1Pin2=33;

const int trigPin2 =14;  // Ultrasonic sensor 2 trigger pin
const int echoPin2 =26;  // Ultrasonic sensor 2 echo pin

const int trigPin1 =13;  // Ultrasonic sensor 1 trigger pin
const int echoPin1 =12;  // Ultrasonic sensor 2 echo pin

// Define push button pins
const int buttonV3Pin = 18;  // Replace with the actual pin number for your button
const int buttonV4Pin = 19;  // Replace with the actual pin number for your button

// Add global variables for PHYSICAL push BUTTONS 
bool buttonV3State; 
bool buttonV4State; 

//STATE maintainance start
bool Garage_operationInProgress = false; 
int Garage_gateState;  // 0: Closed, 1: Opened
int usSensorState1;
int usSensorState2;
int secure_mode;
//STATE maintainance end

//************** GARAGE ROLL UP END********************


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "JOY -2.4GHz";
char pass[] = "joy@12345678";
// V7 is FREE


BlynkTimer timer;

void openDoor();
void closeDoor();
void updateBlynkApp();

BLYNK_WRITE(V1) {
  detectObstacle();
  if (param.asInt() == 1 && gateState == 0 && !operationInProgress && !usSensorState )
   {
    operationInProgress = true;
    openDoor();
  } else {
  if(gateState==1)
  {
   Blynk.virtualWrite(V5, "GATE IS ALREADY OPENED"); 
  }
  else if(operationInProgress)
  {
   Blynk.virtualWrite(V5, "OPERATION IN PROGRESS"); 
  }
    else if(usSensorState)
  {
   Blynk.virtualWrite(V5, "Operation Not Allowed! The door is Manually locked");
   testPulseTone();
  }
  

  }
   operationInProgress = false;  // Update flag
}

BLYNK_WRITE(V2) {
  detectObstacle();
  if (param.asInt() == 1 && gateState == 1 && !operationInProgress) {
    operationInProgress = true;
    closeDoor();
  } else {
    if(gateState==0)
  {
   Blynk.virtualWrite(V5, "GATE IS ALREADY CLOSED"); 
  }
  else if(operationInProgress)
  {
   Blynk.virtualWrite(V5, "OPERATION IN PROGRESS"); 
  }
  }
   operationInProgress = false;  // Update flag
}



BLYNK_WRITE(V8)
{detectObstacle();
  //V8 PUSH OPEN BUTTON
  gateState = 1;  // 0: Closed, 1: Opened (here setted as OPENED)
  operationInProgress = false;
  Blynk.virtualWrite(V5,"\n::setted GateState=1(OPENED)/OIP=FALSE::\n");
  if(param.asInt() == 1 && !operationInProgress )// && usSensorState
  {
    
        digitalWrite(relayPin, LOW);
        delay(500);
      // Motor 1
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  
  // Motor 2
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  }
  else
  {  
         // Motor 1
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  
  // Motor 2
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
    digitalWrite(relayPin, HIGH);
  }

}

BLYNK_WRITE(V9)
{detectObstacle();
  //THE PUSH CLOSE BUTTON

   gateState = 0;  // 0: Closed, 1: Opened (Here setted as closed)
   operationInProgress = false;
  Blynk.virtualWrite(V5,"\n::setted GateState=0(CLOSED)/OIP=FALSE::\n");
  if(param.asInt() == 1 && !operationInProgress)//  && usSensorState
  {
      
  digitalWrite(relayPin, LOW);
  delay(500);
      // Motor 1
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  
  // Motor 2
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  }
  else
  {
         // Motor 1
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  
  // Motor 2
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
    digitalWrite(relayPin, HIGH);
  }
  
}

//************** GARAGE ROLL UP S********************relayPin
BLYNK_WRITE(V3) {
  Garage_sensorgatestate();
  if (param.asInt() == 1 && !Garage_operationInProgress && Garage_gateState == 0) {
    Garage_operationInProgress = true;
    openGarage();
  } else {
    if (Garage_operationInProgress) {
      Blynk.virtualWrite(V5, "OPERATION IN PROGRESS");
    }
    else if(Garage_gateState == 1)
    {
      
            Blynk.virtualWrite(V5, "Garage is already opened");
    }
    else if(Garage_gateState ==-99)
    {
            Blynk.virtualWrite(V5, "Can-not detect GATE STATE\n Both the sensors detected obstracles\n\n\nMANUAL ALERT:PLEASE CHECK & REMOVE THE UNWANTED OBSTRACLE FROM THE SENSOR\n\n\n");
            testPulseTone();
    }
    else
    {
            Blynk.virtualWrite(V5, "Can-not detect GATE STATE\n Sensors detected Nothing\n GATE IS IN MIDDLE of open * close\n\n\nMANUAL ALERT: Please OPEN/CLOSE the door MANUALLY\n\n\n");
            testPulseTone();
    }
  }
  
  Garage_operationInProgress=false;
}

BLYNK_WRITE(V4) {
  Garage_sensorgatestate();
  if (param.asInt() == 1 && !Garage_operationInProgress && Garage_gateState ==1) {
    Garage_operationInProgress = true;
    closeGarage();
  } else { updateBlynkAppGarage();
    if (Garage_operationInProgress) {
      Blynk.virtualWrite(V5, "OPERATION IN PROGRESS");
    }
     else if(Garage_gateState == 0)
    {
            Blynk.virtualWrite(V5, "Garage is already Closed");
    }
    else if(Garage_gateState ==-99)
    {
            Blynk.virtualWrite(V5, "Can-not detect GATE STATE\n Both the sensors detected obstracles\n\n\nMANUAL ALERT:PLEASE CHECK & REMOVE THE UNWANTED OBSTRACLE FROM THE SENSOR\n\n\n");
            testPulseTone();
    }
    else
    {
            Blynk.virtualWrite(V5, "Can-not detect GATE STATE\n Sensors detected Nothing\n GATE IS IN MIDDLE of open * close\n\n\nMANUAL ALERT: Please OPEN/CLOSE the door MANUALLY\n\n\n");
            testPulseTone();
    }
  }
  Garage_operationInProgress=false;
}


BLYNK_WRITE(V6) {
    detectObstacle();
    // V6 PUSH BUTTON FOR SECURE MODE
    if (param.asInt() == 1) {
        Blynk.virtualWrite(V5, "\nSECURE MODE ACTIVATED...\nOUTDOOR BUTTONS WILL NOT WORK\n");
        secure_mode = 1;
    } else {
        Blynk.virtualWrite(V5, "\nSECURE MODE DE-activated...\nOUTDOOR BUTTONS WILL WORK\n");
        secure_mode = 0;
    }

    // Save secure_mode to preferences
    pref.putInt("secure_mode", secure_mode);
}



bool blinkButtonState = false;  // Variable to track the state of the blink button
BLYNK_WRITE(V7) {
  // blinkButtonState = param.asInt();
    if(gateState==0)
    {
      v1();
      v3();
    }
    else
    {
      v2();
      v4();
    }
}


void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  checkWiFiConnection();


  // Initialize the Preferences library
  pref.begin("your-app-name", false);
  // Retrieve the last state of secure_mode from preferences
  secure_mode = pref.getInt("secure_mode", 0);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(relayPin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

//garage one::::start
  pinMode(garage_motor1Pin1, OUTPUT);
  pinMode(garage_Motor1Pin2, OUTPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

 // Set push button pins as inputs
  pinMode(buttonV3Pin, INPUT_PULLUP);
  pinMode(buttonV4Pin, INPUT_PULLUP);


  pinMode(buttonV5Pin, INPUT_PULLUP);
  pinMode(buttonV6Pin, INPUT_PULLUP);

pinMode(buttonVbothPin, INPUT_PULLUP);

digitalWrite(relayPin, HIGH);
//garage one:::end
pinMode(buzzerPin, OUTPUT);


Blynk.virtualWrite(V5, "\n     ___________________JAI SHREE RAM__________________\n");
Blynk.virtualWrite(V5, "\t\t\t\t\t\t\t\t\t\t\t\t.__ .          .            * __."
                     "\n\t\t\t\t\t\t\t\t\t\t\t\t[__)|_  _.._  _| _.._.  .   '(__"
                     "\n\t\t\t\t\t\t\t\t\t\t\t\t[__)[ )(_][ )(_](_][  \\_|    .__)"
                     "\n\t\t\t\t\t\t\t\t\t\t\t\t                      ._|"
                     "\n");


  Blynk.virtualWrite(V5, "\n Automation is ONLINE! System ready to receive commands! \n");
testSirenSound();
testSensorsLoop_first();//test
  // Assume gate starts in the closed state
  // Setup a timer to update Blynk app after operations are completed
  timer.setInterval(1000L, updateBlynkApp);
}



void loop() {
  Blynk.run();
  timer.run();
  checkWiFiConnection();
  handleButtonPress();

  if (secure_mode == 0) {
    handleButtonPress();
  }

  // Remove the checkSerialCommand() function call from here
  delay(250);  // Add a small delay to prevent continuous checking from affecting other operations
}

bool testSensors = false; // Global variable to control sensor testing loop

BLYNK_WRITE(V5) { // Virtual pin for Blynk terminal
  String command = param.asStr();
  command.trim(); // Remove leading and trailing whitespaces

  if (command.equals("test_sensors_on")) {
    testSensorsOn(); // Start sensor testing loop
  } else if (command.equals("test_sensors_off")) {
    testSensorsOff(); // Stop sensor testing loop
  }
}

void testSensorsOn() {
  Blynk.virtualWrite(V5, "Starting sensor test...\n");
  testSensors = true; // Start continuous sensor testing loop
  testSensorsLoop(); // Start the sensor testing loop immediately
}

void testSensorsOff() {
  Blynk.virtualWrite(V5, "Stopping sensor test...\n");
  testSensors = false; // Stop continuous sensor testing loop
}

void testUltrasonicSensor(int trigPin, int echoPin, String sensorName) {
  NewPing usSensor(trigPin, echoPin);
  int pingResult = usSensor.ping_cm();

  if (pingResult == 0 || pingResult == US_ROUNDTRIP_CM) {
    Blynk.virtualWrite(V5, "Error: " + sensorName + " Ultrasonic Sensor not responding\n");
  } else {
    Blynk.virtualWrite(V5, sensorName + " Ultrasonic Sensor is working. Distance: " + String(pingResult) + " cm\n");
  
  }
}

void testSensorsLoop() {
  while (testSensors) { // Continuous loop as long as testSensors is true
    // Test each ultrasonic sensor one by one
    testUltrasonicSensor(trigPin, echoPin, "MAIN GATE");
    testUltrasonicSensor(trigPin1, echoPin1, "GARAGE SENSOR 1");
    testUltrasonicSensor(trigPin2, echoPin2, "GARAGE SENSOR 2");
    Blynk.virtualWrite(V5, "\n\n");
    delay(1000); // Delay between sensor tests (adjust as needed)

  }
}


void testUltrasonicSensor_first(int trigPin, int echoPin, String sensorName) {
  NewPing usSensor(trigPin, echoPin);
  int pingResult = usSensor.ping_cm();

  if (pingResult == 0 || pingResult == US_ROUNDTRIP_CM) {
    Blynk.virtualWrite(V5, "Error: " + sensorName + " Ultrasonic Sensor not responding\n");
  } else {
    sensorcheckup++;
  }
}

void testSensorsLoop_first() {
    Blynk.virtualWrite(V5, "Performing system startup sensor check:::");
    // Test each ultrasonic sensor one by one
    testUltrasonicSensor_first(trigPin, echoPin, "MAIN GATE");
    testUltrasonicSensor_first(trigPin1, echoPin1, "GARAGE SENSOR 1");
    testUltrasonicSensor_first(trigPin2, echoPin2, "GARAGE SENSOR 2");
    if(sensorcheckup==3)
    {
      Blynk.virtualWrite(V5, "OK\n");
      return;
    }
    else
    {
      testPulseTone();
      testPulseTone();
    }
    

  }









void checkWiFiConnection() {
  const int delayBetweenAttempts = 10000;  // Delay between reconnection attempts in milliseconds
  const char* successMessage = "\n::::::Reconnected to WiFi!::::::\n";
  const char* failureMessage = "\n::::::Failed to reconnect to WiFi!::::::\n";

  while (WiFi.status() != WL_CONNECTED) {
    Blynk.virtualWrite(V5, "\n::::::No WiFi Connection! Attempting to reconnect...::::::\n");

    Serial.println("Searching for WiFi...");
    internetbeep(); // Beep before each attempt to connect to WiFi
      
    WiFi.begin(ssid, pass);
    delay(delayBetweenAttempts);  // Wait before next attempt

    if (WiFi.status() == WL_CONNECTED) {
      Blynk.virtualWrite(V5, successMessage);
    } else {
      Blynk.virtualWrite(V5, failureMessage);
    }
  }
}







void handleButtonPress() {
  buttonV5State = digitalRead(buttonV5Pin);
  buttonV6State = digitalRead(buttonV6Pin);
  buttonV3State = digitalRead(buttonV3Pin);
  buttonV4State = digitalRead(buttonV4Pin);
  buttonVbothState = digitalRead(buttonVbothPin);

 

  if (buttonV3State == LOW) {
    beep();
    Blynk.virtualWrite(V5, "OUTDOOR: GARAGE OPEN button pressed");
    v3();
  } else if (buttonV4State == LOW) {
    beep();
    Blynk.virtualWrite(V5, "OUTDOOR: GARAGE CLOSE button pressed");
    v4();
  } else if (buttonV5State == LOW) {
    beep();
    Blynk.virtualWrite(V5, "OUTDOOR: MAIN GATE OPEN button pressed");
    v1();
  } else if (buttonV6State == LOW) {
    beep();
    Blynk.virtualWrite(V5, "OUTDOOR: MAIN GATE CLOSE button pressed");
    v2();
   }

  else if (buttonVbothState == LOW) {
    beep();
    Blynk.virtualWrite(V5, "OUTDOOR: GATES OPEN/CLOSE button pressed");
    if (gateState == 0) {
      v1();
      v3();
    } else {
      v2();
      v4();
    }
  } else {
    Blynk.virtualWrite(V3, 0);
    Blynk.virtualWrite(V4, 0);
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V2, 0);
  }
}

void beep() {
  tone(buzzerPin, 400, 1111);
}

void openDoor() {
  Blynk.virtualWrite(V5, "Main Gate Opening...\n");
  testAscendingTone();
  int c = 0;
  gateState = 1;  // Set gate state to opened

  digitalWrite(relayPin, LOW);  // Activate the relay to unlock
  delay(2000);
  c++;

  // Motor 1 for 3 seconds.
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  delay(3000);
  c++;

  // Motor 2 and Motor 1 for 7 seconds.
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  delay(7000);

  // Motor 2 for 3 seconds.
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  delay(3000);

  // Stop Motors
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  c++;

  digitalWrite(relayPin, HIGH);  // Deactivate the relay to lock
 
  Serial.print("DOOR OPENED SUCCESSFULLY\n");
  c++;

  if (c == 4) {
    Blynk.virtualWrite(V5, "MAIN GATE OPENED SUCCESSFULLY\n");
  } else {
    Blynk.virtualWrite(V5, "\n......SORRY MALIK, OPENING FAILED......\n");
  }

  // Update Blynk app
  updateBlynkApp();
}

void closeDoor() {
  Blynk.virtualWrite(V5, "Main Gate Closing... \n");
  testDescendingTone();

  int c = 0;

  c++;
  gateState = 0;  // Set gate state to closed
Blynk.virtualWrite(V10, 1);

  digitalWrite(relayPin, LOW);  // Activate the relay to unlock
  delay(2000);

  // Motor 2 for 3 seconds.

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  delay(3000);
  c++;

  // Motor 2 and Motor 1 for 7 seconds.
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  delay(7000);

  // Motor 1 for 3 seconds.
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(3000);

  // Stop Motors
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  c++;

  digitalWrite(relayPin, HIGH);  // Deactivate the relay to lock
 
  c++;

  if (c == 4) {
    Blynk.virtualWrite(V5, "MAIN GATE CLOSED SUCCESSFULLY\n");
  } else {
    Blynk.virtualWrite(V5, "\n......SORRY MALIK, CLOSING FAILED......\n");
  }
  Serial.print("DOOR CLOSED\n");
  // Update Blynk app
  updateBlynkApp();
}


//***************************** GARAGE ROLL UP S********************************
void openGarage() {
 
  Blynk.virtualWrite(V5, "Garage Opening...\n");
  testAscendingTone();

  digitalWrite(garage_motor1Pin1, HIGH);
  digitalWrite(garage_Motor1Pin2, LOW);

  unsigned long startTime = millis();  // Record the start time

  // Run the motor for a maximum of 10 seconds or until echoPin2 is HIGH
  while (Garage_gateState != 1 && millis() - startTime < 20000) {
    Garage_sensorgatestate();
    delay(100);  // Wait for 100 milliseconds before checking again
  }

  // Stop the garage door motor
  digitalWrite(garage_motor1Pin1, LOW);
  digitalWrite(garage_Motor1Pin2, LOW);

  Garage_gateState = 1;
  updateBlynkAppGarage();
  Blynk.virtualWrite(V5, "GARAGE GATE OPENED SUCCESSFULLY\n");
}

void closeGarage() {

 
  Blynk.virtualWrite(V5, "Garage Closing...\n");
  testDescendingTone();

  digitalWrite(garage_motor1Pin1, LOW);
  digitalWrite(garage_Motor1Pin2, HIGH);

  unsigned long startTime = millis();  // Record the start time

  // Run the motor for a maximum of 10 seconds or until Garage_gateState is 0
  while (Garage_gateState != 0 && millis() - startTime < 20000) {
    Garage_sensorgatestate();
    delay(100);  // Wait for 100 milliseconds before checking again
  }

  // Stop the garage door motor
  digitalWrite(garage_motor1Pin1, LOW);
  digitalWrite(garage_Motor1Pin2, LOW);

  Garage_gateState = 0;
  updateBlynkAppGarage();
  Blynk.virtualWrite(V5, "GARAGE GATE CLOSED SUCCESSFULLY\n");
}






//*************************** GARAGE ROLL UP E***********************************



void Garage_sensorgatestate() {
  // Ultrasonic sensor 2
  NewPing usSensor2(trigPin2, echoPin2);
  int pingResult2 = usSensor2.ping_cm();
  if (pingResult2 == 0 || pingResult2 == US_ROUNDTRIP_CM) {
    Blynk.virtualWrite(V5, "Error: Ultrasonic Sensor 2 not responding");
    return;
  }
  usSensorState2 = pingResult2 < 40;  // Adjust the distance (40 cm) as needed


  // Ultrasonic sensor 1
  NewPing usSensor1(trigPin1, echoPin1);
  int pingResult1 = usSensor1.ping_cm();
  if (pingResult1 == 0 || pingResult1 == US_ROUNDTRIP_CM) {
    Blynk.virtualWrite(V5, "Error: Ultrasonic Sensor 1 not responding");
    return;
  }
  usSensorState1 = pingResult1 < 40;  // Adjust the distance (40 cm) as needed

  // Blynk.virtualWrite(V5, "usSensorState2 = " + String(usSensorState2));//      testing ultra sonic sensors.
  // Blynk.virtualWrite(V5, "usSensorState1 = " + String(usSensorState1));//      testing ultra sonic sensors.

  if (usSensorState2 && !usSensorState1) {
    Garage_gateState = 1;  // Opened
    // Blynk.virtualWrite(V5, "GARAGE OPENED");
  } else if (!usSensorState2 && usSensorState1) {
    Garage_gateState = 0;  // Closed
    // Blynk.virtualWrite(V5, "GARAGE CLOSED");
  } else if (usSensorState2 && usSensorState1) {
    Garage_gateState = -99;
  } else if (!usSensorState2 && !usSensorState1) {
    // Both sensors detect an obstacle, show error message
    Garage_gateState = -98;
  }
}




void updateBlynkAppGarage()
{

  Blynk.virtualWrite(V3,0);
  Blynk.virtualWrite(V4,0);

}

void updateBlynkApp() {
  // Turn off buttons and update switch status in Blynk app
  Blynk.virtualWrite(V1, 0);
  Blynk.virtualWrite(V2, 0);
  Blynk.virtualWrite(V3, 0);
  // Blynk.virtualWrite(V4, 0);

  // Update switch status in Blynk app based on the current gate state

}


#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define PING_TIMEOUT 500 // Timeout value in milliseconds

void detectObstacle() 
{
   NewPing usSensor(trigPin, echoPin);
int pingResult = usSensor.ping_cm();
if (pingResult == 0 || pingResult == US_ROUNDTRIP_CM) {
  Blynk.virtualWrite(V5, "Error: MAIN GATE Ultrasonic Sensor not responding");
  usSensorState=1;//stopping the operation
  return;
}
usSensorState = pingResult < 10;  // Adjust the distance (40 cm) as needed
// Blynk.virtualWrite(V5, "usSensorState(MAIN GATE ) = " + String(usSensorState));//testing ultra sonic sensors.
}






void v3()
{
  Garage_sensorgatestate();
  if (!Garage_operationInProgress && Garage_gateState == 0) {
    Garage_operationInProgress = true;
    openGarage();
  } 
  else {
    if (Garage_operationInProgress) {
      Blynk.virtualWrite(V5, "OPERATION IN PROGRESS");
    }
    else if(Garage_gateState == 1)
    {
      
            Blynk.virtualWrite(V5, "Garage is already opened");
    }
    else if(Garage_gateState ==-99)
    {
            Blynk.virtualWrite(V5, "Can-not detect GATE STATE\n Both the sensors detected obstracles\n\n\nMANUAL ALERT:PLEASE CHECK & REMOVE THE UNWANTED OBSTRACLE FROM THE SENSOR\n\n\n");
    }
    else
    {
            Blynk.virtualWrite(V5, "Can-not detect GATE STATE\n Sensors detected Nothing\n GATE IS IN MIDDLE of open * close\n\n\nMANUAL ALERT: Please OPEN/CLOSE the door MANUALLY\n\n\n");
    }
}
Garage_operationInProgress=false;
}

void v4()
{ 
  Garage_sensorgatestate();
  if (!Garage_operationInProgress && Garage_gateState ==1) {
    Garage_operationInProgress = true;
    closeGarage();
  } else { updateBlynkAppGarage();
    if (Garage_operationInProgress) {
      Blynk.virtualWrite(V5, "OPERATION IN PROGRESS");
    }
     else if(Garage_gateState == 0)
    {
            Blynk.virtualWrite(V5, "Garage is already Closed");
    }
    else if(Garage_gateState ==-99)
    {
            Blynk.virtualWrite(V5, "Can-not detect GATE STATE\n Both the sensors detected obstracles\n\n\nMANUAL ALERT:PLEASE CHECK & REMOVE THE UNWANTED OBSTRACLE FROM THE SENSOR\n\n\n");
    }
    else
    {
            Blynk.virtualWrite(V5, "Can-not detect GATE STATE\n Sensors detected Nothing\n GATE IS IN MIDDLE of open * close\n\n\nMANUAL ALERT: Please OPEN/CLOSE the door MANUALLY\n\n\n");
    }
  }
  Garage_operationInProgress=false;
}

void v1()
{
  detectObstacle();
  if (gateState == 0 && !operationInProgress)//  && usSensorState
  {
    operationInProgress = true;
    openDoor();
  } else {
  if(gateState==1)
  {
   Blynk.virtualWrite(V5, "GATE IS ALREADY OPENED"); 
  }
  else if(operationInProgress)
  {
   Blynk.virtualWrite(V5, "OPERATION IN PROGRESS"); 
  }
    else if(!usSensorState)
  {
   Blynk.virtualWrite(V5, "Operation Not Allowed! The door is Manually locked");
  }
  

  }
   operationInProgress = false;  // Update flag
}

void v2()
{
 detectObstacle();
  if (gateState == 1 && !operationInProgress) {
    operationInProgress = true;
    closeDoor();
  } else {
    // Blynk.virtualWrite(V5, "\n\t\t\t\t::::Already Closed or Busy in other Progress or IR Sensor is active::::\n");
    // Serial.print(gateState);
    // Serial.print(operationInProgress);

    if(gateState==0)
  {
   Blynk.virtualWrite(V5, "GATE IS ALREADY CLOSED"); 
  }
  else if(operationInProgress)
  {
   Blynk.virtualWrite(V5, "OPERATION IN PROGRESS"); 
  }
  }
   operationInProgress = false;  // Update flag
}





void testAscendingTone() {
  // Serial.println("Testing Ascending Tone");
  digitalWrite(buzzerPin, LOW); // Ensure the pin is low before generating sound
  for (int i = 1000; i <= 2000; i += 100) {
    tone(buzzerPin, i);
    delay(100);
  }
  noTone(buzzerPin);
  digitalWrite(buzzerPin, HIGH); // Ensure the pin is low after generating sound
}

void testDescendingTone() {
  // Serial.println("Testing Descending Tone");
  digitalWrite(buzzerPin, LOW); // Ensure the pin is low before generating sound
  for (int i = 2000; i >= 1000; i -= 100) {
    tone(buzzerPin, i);
    delay(100);
  }
  noTone(buzzerPin);
  digitalWrite(buzzerPin, HIGH); // Ensure the pin is low after generating sound
}

void testPulseTone() {
  // Serial.println("Testing Pulse Tone");
  digitalWrite(buzzerPin, LOW); // Ensure the pin is low before generating sound
  for (int i = 0; i < 9; i++) {
    tone(buzzerPin, 1500);
    delay(100);
    noTone(buzzerPin);
    delay(100);
  }
  digitalWrite(buzzerPin, HIGH); // Ensure the pin is low after generating sound
}

void testSirenSound() {
  // Serial.println("Testing Siren Sound");
  digitalWrite(buzzerPin, LOW); // Ensure the pin is low before generating sound
  for (int i = 1000; i <= 2000; i += 100) {
    tone(buzzerPin, i);
    delay(50);
  }
  delay(500);
  for (int i = 2000; i >= 1000; i -= 100) {
    tone(buzzerPin, i);
    delay(50);
  }
  noTone(buzzerPin);
  digitalWrite(buzzerPin, HIGH); // Ensure the pin is low after generating sound
}

void internetbeep() {
  // Serial.println("Testing Pulse Tone");
  digitalWrite(buzzerPin, LOW); // Ensure the pin is low before generating sound
  for (int i = 0; i < 2; i++) {
    tone(buzzerPin, 1500);
    delay(100);
    noTone(buzzerPin);
    delay(100);
  }
  digitalWrite(buzzerPin, HIGH); // Ensure the pin is low after generating sound
}