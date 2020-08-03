int value = 0;
int x_value = 0;
int y_value = 0;
int x_out = 0; 
int button_value = 0;
int buffer = 10;
int pinX = A0;
int pinY = A1;
void setup() {
  //pinMode(A0, INPUT);
  //pinMode(A1, INPUT);
    pinMode(7, INPUT_PULLUP);
    Serial.begin(9600);
  
  
  
}

int Angle() {
  X_value = analogRead(pinX);
 // Serial.print("X:");
  //Serial.print(x_value, DEC);
  if (X_value >= (1024/2-buffer) && X_value <=1024/2+buffer){
   X_out = 512;
    }
  else{ X_out = X_value ;}
  //Serial.print("Angle =");
  return map(X_out,0,1023,0,180);
   
  //Y_value = 1023 - analogRead(A1);
  //Serial.print(" | Y:");
  //Serial.print(Y_value, DEC);
  //if (Y_value >= 480 && Y_value <=544){
  //  Serial.print("ZERO");
 // }

 // Button_value = digitalRead(7);
  //Serial.print(" | Button:");
  //Serial.println(Button_value, DEC);

  delay(100);

}
