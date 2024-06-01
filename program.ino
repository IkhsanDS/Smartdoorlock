#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Password.h>
#include <Servo.h>
#include <Buzzer.h>


#define echoPin A0 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin A1 //attach pin D3 Arduino to pin Trig of HC-SR04
byte currentLength = 0;
Password password = Password("1234");//Password bisa di ubah ubah
LiquidCrystal_I2C lcd(0x27,16,2);
Servo myservo;
const int pinBuzzer = 11;
int LedMerah = A3;
int LedPutih = A2;
const byte ROWS= 4; //baris pada keypad
const byte COLS= 4; //Kolom pada keypad
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement


/*keymap mendefinisikan tombol ditekan sesuai
dengan baris dan kolom seperti muncul pada keypad*/
char keys[ROWS][COLS] =
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};


byte rowPins[ROWS]= {9,8,7,6}; 
byte colPins[COLS]= {5,4,3,2}; 


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
  pinMode(pinBuzzer, OUTPUT);
  pinMode(LedMerah, OUTPUT);
  pinMode(LedPutih, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

  Serial.begin(9600);
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
  //sumber diambil pada examples di library password.
  keypad.addEventListener(keypadEvent);
  lcd.init();
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Doorlock System");
  lcd.setCursor(0, 1);
  lcd.print("BY:IKHSAN");
  delay(5000);
  lcd.clear();
  myservo.attach(10);
}

// Jika tombol ditekan, tombol ini disimpan dalam 'keypressed' variabel
// Jika kunci tidak sama dengan 'NO_KEY', maka tombol ini dicetak
// Jika jumlah = 17, maka penghitungan reset kembali ke 0 (ini berarti tidak ada tombol yang ditekan selama proses pemindaian seluruh keypad
void loop()
{
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  lcd.setCursor(0, 0);
  lcd.print("Masukan Password");
  lcd.setCursor(0,1);
  lcd.print("Pass: ");
  keypad.getKey();
  if(distance < 5){
    digitalWrite(LedPutih, HIGH);
    delay(200);
    digitalWrite(pinBuzzer, HIGH);    
    delay(200);                       
    digitalWrite(pinBuzzer, LOW);     
    delay(200);                       
    digitalWrite(pinBuzzer, HIGH);    
    delay(200);                       
    digitalWrite(pinBuzzer, LOW);    
    delay(100); 
    myservo.write(0);
      lcd.clear();
      lcd.print("Berhasil dari ultrasonic");
      Serial.println("Berhasil Dari ultraman");
      digitalWrite(LedPutih, LOW); 
      delay(5000);//Lama waktu relay
      myservo.write(130);
      password.reset();
    }
}


void checkPassword(){ 
   if(password.evaluate()){
    digitalWrite(LedPutih, HIGH);    
    delay(1000);
    
    digitalWrite(pinBuzzer, HIGH);    
    delay(200);                       
    digitalWrite(pinBuzzer, LOW);     
    delay(200);                       
    digitalWrite(pinBuzzer, HIGH);    
    delay(200);                       
    digitalWrite(pinBuzzer, LOW);    
    delay(100);        
      myservo.write(0);
      lcd.clear();
      lcd.print("Berhasil dari keypad");
      Serial.println("Berhasil Dari keypad");
      digitalWrite(LedPutih, LOW); 
      delay(5000);//Lama waktu relay
      myservo.write(130);
      password.reset();
  
   }
   else {
    digitalWrite(LedMerah, HIGH);    
    delay(1000);
    digitalWrite(pinBuzzer, HIGH);    
    delay(2000);                       
    digitalWrite(pinBuzzer, LOW);     
    delay(200);                       
    digitalWrite(pinBuzzer, HIGH);    
    delay(1000);                
    digitalWrite(pinBuzzer, LOW);
      lcd.clear();
      lcd.print("Salah coba lagi");
      Serial.println("Salah coba lagi");
      digitalWrite(LedMerah, LOW);      
      delay(5000);//lama led on 
      password.reset();
   }
}


void keypadEvent(KeypadEvent eKey){
  
  switch (keypad.getState()){  
    case PRESSED:
  lcd.setCursor(0,1);
    lcd.print(eKey);
    Serial.println(eKey);
  switch (eKey){
    case '*': checkPassword(); 
    lcd.clear(); currentLength=0;break;
    case '#': password.reset();
    lcd.clear();currentLength=0; break;
    default:// password.append(eKey);
          password << eKey;
          currentLength++;
     
        //Print some feedback.
        lcd.setCursor(0,1);
        lcd.print("Pass: ");
        for (byte i=0; i<currentLength; i++){
            lcd.print('*');
        }
           
   }
  }  
}  

//IKHSAN DWI SETO
