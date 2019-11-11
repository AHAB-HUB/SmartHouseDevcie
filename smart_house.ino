
//list of all needed variables

const int PIN1 = 12;
const int PIN2 = 13;
const int PIN3 = 11;
const int PIN4 = 8;
const int PIN_water_leakage = 4; // house switch that triggers water leakage.
const int PIN_stove = 5; //house switch that triggers stove.
const int PIN_fire_alarm = 2; //house switch that trigger fire alarm.
const int PIN_window = 6; // house switch that trigger window.

int incomingCommandByte = 0; // incoming data in serial
int waterLeakage_state; // read waterLeakage switch values
int stove_state; // read stove switch values
int fireAlarm_state; // used to read fireAlarm switch values
int window_state; // used to read window switch values


void setup() {
  
  Serial.begin(9600);
   pinMode(PIN1, OUTPUT);
   pinMode(PIN2, OUTPUT);
   pinMode(PIN3, OUTPUT);
   pinMode(PIN4, OUTPUT);
   pinMode(PIN_water_leakage, INPUT);
   pinMode(PIN_stove, INPUT);
   pinMode(PIN_fire_alarm, INPUT);
   pinMode(PIN_window, INPUT);
   timer1OFF();
   timer2OFF();
   Serial.println("Welcome to SMART HOUSE");
  
}

void loop() {
  
  smartHousePanel(); //To control the switches at the smart house.
       
   while (Serial.available() > 0){
  incomingCommandByte = Serial.read() - '0';
  
    if( incomingCommandByte == 1){
    indoorLightsON();
    incomingCommandByte = 0;
    }
     else if( incomingCommandByte == 2){
    indoorLightsOFF();
    incomingCommandByte = 0;
    }
    else if( incomingCommandByte == 3){
    outdoorLightsON();
    incomingCommandByte = 0;
    }
    else if( incomingCommandByte == 4){
    
    outdoorLightsOFF();
    incomingCommandByte = 0;
    }
    else if( incomingCommandByte == 5){
    radiatorON();
    incomingCommandByte = 0;
    }
    else if( incomingCommandByte == 6){
    radiatorOFF();
    incomingCommandByte = 0;
    }
    else if( incomingCommandByte == 7){
    
    incomingCommandByte = 0;
    }
    else if( incomingCommandByte == 8){
    
    incomingCommandByte = 0;
    }
    else if( incomingCommandByte == 9){
    
    incomingCommandByte = 0;
    }
}
}

 double getInternalTemperature(){
  //method to get internal temperature
  }
  
double getExternalTemperature(){
 // method to get external temperature 
 }


   void sendWaterLeakageState(){
    //send message to server on water leakage
      if(waterLeakage_state == HIGH){
        }

        //send message to server NO water leakage
        else if (waterLeakage_state == LOW){
          ////send message to server to tell there is no  water leakage
          }
      }

      //send message to server on window open
      void sendWindowState(){
      if(window_state == HIGH){
        
        }
        //send message to server window close
        else if (window_state == LOW){
          
          }
      }

      
      void sendStoveState(){
        //send message to server on stove ON
      if(stove_state == HIGH){
        
        }
        //send message to server on stove OFF
        else if (stove_state == LOW){
          
          }
      }

      
      void sendFireAlarmState(){

        //send message to server on fire alarm
      if(fireAlarm_state == HIGH){
        Serial.println("Fire alarm ON!");
        fireAlarmON();
        delay(100);
        }

        //send message to server NO fire alarm
        else if (fireAlarm_state == LOW){
          Serial.println("Fire alarm OFF!");
          fireAlarmOFF();
          delay(100);
          }
      }

      //method to TURN On indoor light (0.0.1.0)
 void indoorLightsON(){
   digitalWrite(PIN1, LOW);
   digitalWrite(PIN2, LOW);
   digitalWrite(PIN3, HIGH);
   digitalWrite(PIN4, LOW);
 
   
  }

 //TURN OFF indoor lights (1.0.1.0)
 void indoorLightsOFF(){
   digitalWrite(PIN1, HIGH);
   digitalWrite(PIN2, LOW);
   digitalWrite(PIN3, HIGH);
   digitalWrite(PIN4, LOW);
   
  
  }

//TURN ON outdoor lights (0.1.1.1)
 void outdoorLightsON(){
   digitalWrite(PIN1, LOW);
   digitalWrite(PIN2, HIGH);
   digitalWrite(PIN3, HIGH);
   digitalWrite(PIN4, HIGH);
   
  }

  //TURN OFF outdoor lights (1.1.1.1)
 void outdoorLightsOFF(){
   digitalWrite(PIN1, HIGH);
   digitalWrite(PIN2, HIGH);
   digitalWrite(PIN3, HIGH);
   digitalWrite(PIN4, HIGH);
 
  }

  // TURN ON radiator (0.1.0.1)
 void radiatorON(){
   digitalWrite(PIN1, LOW);
   digitalWrite(PIN2, HIGH);
   digitalWrite(PIN3, LOW);
   digitalWrite(PIN4, HIGH);
  
  }

  //TURN OFF radiator (1.1.0.1)
void radiatorOFF(){
   digitalWrite(PIN1, HIGH);
   digitalWrite(PIN2, HIGH);
   digitalWrite(PIN3, LOW);
   digitalWrite(PIN4, HIGH);
  
  }

//TURN ON timer 2 (0.1.0.0)
void timer1ON(){
   digitalWrite(PIN1, LOW);
   digitalWrite(PIN2, HIGH);
   digitalWrite(PIN3, LOW);
   digitalWrite(PIN4, LOW);
  
}
//TURN OFF the timer (2) (1.1.0.0)
void timer1OFF(){
   digitalWrite(PIN1, HIGH);
   digitalWrite(PIN2, HIGH);
   digitalWrite(PIN3, LOW);
   digitalWrite(PIN4, LOW);
   
}

//TURN ON timer 2 (0.0.0.1)
void timer2ON(){
   digitalWrite(PIN1, LOW);
   digitalWrite(PIN2, LOW);
   digitalWrite(PIN3, LOW);
   digitalWrite(PIN4, HIGH);
  
}
//TURN OFF the timer (2) (1.0.0.1)
void timer2OFF(){
   digitalWrite(PIN1, HIGH);
   digitalWrite(PIN2, LOW);
   digitalWrite(PIN3, LOW);
   digitalWrite(PIN4, HIGH);
  
}

//TURN ON fire alarm sound (1.0.0.0)
  void fireAlarmON(){
  digitalWrite(PIN1 ,HIGH);
  digitalWrite(PIN2 ,LOW);
  digitalWrite(PIN3 ,LOW);
  digitalWrite(PIN4 ,LOW);
 
  }

  //TURN OFF fire alarm sound (0.0.0.0)
   void fireAlarmOFF(){
  digitalWrite(PIN1 ,LOW);
  digitalWrite(PIN2 ,LOW);
  digitalWrite(PIN3 ,LOW);
  digitalWrite(PIN4 ,LOW);
 
  }

  void smartHousePanel(){
    
  waterLeakage_state = digitalRead(PIN_water_leakage);
  delay(100);
  stove_state = digitalRead(PIN_stove);
  delay(100);
  fireAlarm_state = digitalRead(PIN_fire_alarm);
  sendFireAlarmState();
  delay(100);
  window_state = digitalRead(PIN_window);
  delay(100);
 if(stove_state == HIGH || waterLeakage_state == HIGH || fireAlarm_state == HIGH || window_state == HIGH) {
  timer2ON();
  delay(100);
    }
 if(stove_state == LOW && waterLeakage_state == LOW && fireAlarm_state == LOW && window_state == LOW) {
  timer2OFF();
  delay(100);
      } 
    }

    
  
