int firstLine = 2;
int secondLine = 3;
int thirdLine = 4;
int cup = 5;

int snake= 7;


void setup() {
  pinMode(firstLine, OUTPUT);
  pinMode(secondLine, OUTPUT);
  pinMode(thirdLine, OUTPUT);
  pinMode(cup, OUTPUT);
  
  pinMode(snake, OUTPUT);

}

void loop() {
one_to_three();
three_to_one();
snake_and_cup();
Snake();
Cup();

snake_blinking();
cupblinking();


}
void one_to_three(){
  digitalWrite(firstLine,HIGH);
  delay(1000);
  digitalWrite(firstLine,LOW);
  delay(1000);
  digitalWrite(secondLine,HIGH);
  delay(1000);
  digitalWrite(secondLine,LOW);
  delay(1000);
  digitalWrite(thirdLine,HIGH);
  delay(1000);
  digitalWrite(thirdLine,LOW);
  delay(1000);

}
void three_to_one(){
  digitalWrite(thirdLine,HIGH);
  delay(1000);
  digitalWrite(thirdLine,LOW);
  delay(1000);
  digitalWrite(secondLine,HIGH);
  delay(1000);
  digitalWrite(secondLine,LOW);
  delay(1000);
  digitalWrite(firstLine,HIGH);
  delay(1000);
  digitalWrite(firstLine,LOW);
  delay(1000);
}

void snake_and_cup(){
  digitalWrite(snake,HIGH);
  delay(2);
  digitalWrite(snake,LOW);
  delay(2);
  digitalWrite(cup,HIGH);
  delay(2);
  digitalWrite(cup,LOW);
  delay(2);
}

void Cup(){

  ledstuck();
  digitalWrite(cup,HIGH);
  delay(1000);
  digitalWrite(cup,LOW);
  delay(1000);
}

void Snake(){
  ledstuck();
  digitalWrite(snake,HIGH);
  delay(1000);

  ledstuck();
  digitalWrite(snake,LOW);

  delay(1000);
}

void snake_blinking(){
  digitalWrite(snake,HIGH);
  
  delay(1000);
  digitalWrite(snake,LOW);
  
  delay(1000);
  digitalWrite(snake,HIGH);

  delay(1000);
  digitalWrite(snake,LOW);

  delay(1000);
  digitalWrite(snake,HIGH);

  delay(1000);
  digitalWrite(snake,LOW);

  delay(1000);

}
void cupblinking(){
  digitalWrite(cup ,HIGH);
  delay(1000);
  digitalWrite(cup ,LOW);
  delay(1000);
  digitalWrite(cup ,HIGH);
  delay(1000);
  digitalWrite(cup ,LOW);
  delay(1000);
  digitalWrite(cup ,HIGH);
  delay(1000);
  digitalWrite(cup ,LOW);
  delay(1000);

}
void ledstuck(){
  digitalWrite(firstLine,HIGH);
  delay(2);
  digitalWrite(firstLine,LOW);
  delay(2);
  digitalWrite(secondLine,HIGH);
  delay(2);
  digitalWrite(secondLine,LOW);
  delay(2);
  digitalWrite(thirdLine,HIGH);
  delay(2);
  digitalWrite(thirdLine,LOW);
  delay(2);
}