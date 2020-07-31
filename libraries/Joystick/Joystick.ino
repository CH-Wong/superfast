int value = 0;
int X_value = 0;
int Y_value = 0;
int X_out = 0; 
int Button_value = 0;
int Buffer = 10;
int PinX = A0;
int PinY = A1;
void setup() {
  //pinMode(A0, INPUT);
  //pinMode(A1, INPUT);
    pinMode(7, INPUT_PULLUP);
    Serial.begin(9600);
  
  
  
}

int loop() {
  X_value = analogRead(PinX);
 // Serial.print("X:");
  //Serial.print(X_value, DEC);
  if (X_value >= (1024/2-Buffer) && X_value <=1024/2+Buffer){
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
