#include <Servo.h>

#include <IRremote.h>

int led_g=2, led_r=3, led_b=4, led_y=5;
int ir_1=7,ir_2=8, ir_3=19, ir_4=10;
int led[]={0,0,0,0};//order information = A, B, C, D, 0=False, 1=True

Servo servo_1, servo_2;

int us_1_trig=A3, us_1_echo=A2;
int us_2_trig=A5, us_2_echo=A4;

long duration_entry, distance_entry;
long duration_exit, distance_exit;

void setup()
{
  Serial.begin(115200);
  
  pinMode(led_g, OUTPUT);
  pinMode(led_r, OUTPUT);
  pinMode(led_b, OUTPUT);
  pinMode(led_y, OUTPUT);
  
  pinMode(us_1_trig, OUTPUT);
  pinMode(us_1_echo, INPUT);
  pinMode(us_2_trig, OUTPUT);
  pinMode(us_2_echo, INPUT);
  
  servo_1.attach(13);
  servo_2.attach(6);
  

}

void loop()
{
  servo_1.write(0);
  servo_2.write(0);
  
  Serial.println("Welcome to McDonalds! Move to the entry gate.");
  digitalWrite(us_1_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(us_1_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(us_1_trig, LOW);
  duration_entry=pulseIn(us_1_echo, HIGH);
  distance_entry=duration_entry/58.4;
  if(distance_entry<300){
    servo_1.write(90);
    delay(1000);
    Serial.println("Entry Barricade opened. You can pass.");
    delay(2000);
    servo_1.write(0);
    delay(1000);
    Serial.println("Entry Barricade closed.");
  }
  
  Serial.println("Moving to order area. ");
  delay(5000);
  
  for(int i=1; i<=4; i=i+1){
    IRrecv receiver(i+6);
    decode_results output;
    receiver.enableIRIn();
    Serial.print("Item "); 
    Serial.print(i);
    Serial.print(" waiting for yes or no(Press 1 or 0) :- ");
    delay(5000);
  	if (receiver.decode(&output)) {
		unsigned int value = output.value;
      	Serial.print(value);
      	Serial.print("\n");
      if(value==2295){
    	switch(i){
      		case 1 : 	digitalWrite(led_r, HIGH);
      					led[0]=1;
      					break;
   		    case 2 : 	digitalWrite(led_b, HIGH);
      					led[1]=1;
      					break;
      		case 3 :	digitalWrite(led_g, HIGH);
      				    led[2]=1;
      					break;
     		case 4 :	digitalWrite(led_y, HIGH);
      					led[3]=1;
    	}
      }
    }
    
  receiver.resume();

  }
  
  
  Serial.println("\n Items successfully delivered!");
  Serial.print("Order Summary : ");
  for(int i=0; i<4; i=i+1){
    Serial.print(led[i]);
    if(i!=3)
    	Serial.print(" , ");
    else
      	Serial.print("\n");
  }
  
  Serial.println("Have a nice day! Move through the exit gate.");
  digitalWrite(us_2_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(us_2_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(us_2_trig, LOW);
  duration_exit=pulseIn(us_2_echo, HIGH);
  distance_exit=duration_entry/58.4;
  if(distance_exit<300){
    servo_2.write(90);
    delay(1000);
    Serial.println("Exit Barricade opened. You can pass.");
    delay(2000);
    servo_2.write(0);
    delay(1000);
    Serial.println("Exit Barricade closed.");
  }
}