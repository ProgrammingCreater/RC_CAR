
#include <IRremote.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

// Maps the button hex values to String 
#define    UP          0xF609FF00
#define    DOWN        0xF807FF00
#define    Button_0    0xE916FF00
#define    Button_1    0xF30CFF00
#define    Button_2    0xE718FF00
#define    Button_3    0xA15EFF00
#define    Button_4    0xF708FF00
#define    Button_5    0xE31CFF00
#define    Button_6    0xA55AFF00
#define    Button_7    0xBD42FF00
#define    Button_8    0xAD52FF00
#define    Button_9    0xB54AFF00
#define    STOP        0xBF40FF00

#define MINIMUM_DISTANCE 40

int trigPin = 12;
int echoPin = 13;

int sensorPin = 2;

int ena1 = 5;
int in1 = 6;
int in2 = 7;

int ina3 = 8;
int ina4 = 9;
int ena2 = 10;

int rightMotorSpeed = 128;
int leftMotorSpeed = rightMotorSpeed*.9375;

float distance;
bool slow = true;

// right = 
// left = 5.6

void runCar(String direction){
    unsigned long startTime = millis();
    while(millis() - startTime < 2000){
      if(IrReceiver.decode()){
        uint32_t code = IrReceiver.decodedIRData.decodedRawData;
        if (code == STOP) {
          Serial.println("STOP");
          IrReceiver.resume();
          break;
        }
        IrReceiver.resume();
      }


      if(direction == "backward"){
          // CLOCKWISE Half SPEED
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          analogWrite(ena1,rightMotorSpeed);

          digitalWrite(ina3, LOW);
          digitalWrite(ina4, HIGH);
          analogWrite(ena2,leftMotorSpeed);
      }
      else {

        distance = checkDistance();
        
        if(distance < MINIMUM_DISTANCE){
          break;
        }

        else if(direction == "forward"){
          // CLOCKWISE Half SPEED
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          analogWrite(ena1,rightMotorSpeed);

          digitalWrite(ina3, HIGH);
          digitalWrite(ina4, LOW);
          analogWrite(ena2,leftMotorSpeed);
        }
        else if(direction == "right"){
          // CLOCKWISE Half SPEED
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
            analogWrite(ena1,rightMotorSpeed);

            digitalWrite(ina3, HIGH);
            digitalWrite(ina4, LOW);
            analogWrite(ena2,leftMotorSpeed-50);
        }
        else if(direction == "left"){
          // CLOCKWISE Half SPEED
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
            analogWrite(ena1,rightMotorSpeed-50);

            digitalWrite(ina3, HIGH);
            digitalWrite(ina4, LOW);
            analogWrite(ena2,leftMotorSpeed);
        }
      }
    }
    //delay(1000);
          
    //STOP
    if(slow){
      slowStop();
    } else {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(ina3, LOW);
      digitalWrite(ina4, LOW);
    }
    //delay(1000);
}

void slowStop(){
  for(int i = 0; i <= 100; i+=10){
    analogWrite(ena1,rightMotorSpeed - (rightMotorSpeed * (i/100.0)));
    analogWrite(ena2,leftMotorSpeed - (leftMotorSpeed * (i/100.0)));
    
    delay(8);

    if(i == 100){
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(ina3, LOW);
      digitalWrite(ina4, LOW);
    }
  }
}

float checkDistance(){
  // The 10 microsecond delay is required the 2 microsecond one is just good practice
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  
  // pulseIn function measures the time (microseconds) the pin is on HIGH (this is proportional to distance)
  // the 30000 => timeout for the pulse
  long duration = pulseIn(echoPin, HIGH, 30000);

  // Calculates distance by multiplying speed of sound (343 m/s) into cm/microseconds, its divided by 2 due to the full loop the signal makes
  float distance = (duration * 0.0343)/2;

  delay(60);
  return distance;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(ena1, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(ena2, OUTPUT);
  pinMode(ina3, OUTPUT);
  pinMode(ina4, OUTPUT);

  // Starts the IR receiver and makes the LED on it to light up whenever it detects a signal
  IrReceiver.begin(sensorPin, ENABLE_LED_FEEDBACK);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (IrReceiver.decode()) {
    uint32_t code = IrReceiver.decodedIRData.decodedRawData;
    if (code != 0x0) {
      Serial.print("Raw: ");
      Serial.println(code, HEX);
    }

    switch(code){
      case UP:
        Serial.println("Up");
        runCar("forward");
        break;
      case DOWN:
        Serial.println("Down");
        runCar("backward");
        break;
      case Button_1:
        Serial.println("1");
        Serial.println(checkDistance());
        break;
      case Button_2:
        Serial.println("2");
        runCar("right");
        break;
      case Button_3:
        Serial.println("3");
        runCar("left");
        break;
      case Button_4:
        Serial.println("true");
        slow = true;
        break;
      case Button_5:
        Serial.println("false");
        slow = false;
        break;
    }

    IrReceiver.resume();
  }
}
