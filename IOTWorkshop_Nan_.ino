  #include <ESP8266WiFi.h>
  #include <MicroGear.h>
  #include "DHT.h"
  // ----- Change following 6 config variables ---------------------
  const char* ssid     = "nan";       // WiFi ssid
  const char* password = "smile12345";   // WiFi password
  #define APPID   "nanIOT"              // use group AppID 
  #define KEY     "xZ2qNRd3W5NhRWn"                // use group Key
  #define SECRET  "VRmfJkmbl9hNjYIrbD7EdpSbU"             // use group Secret
  #define ALIAS   "nan"  // your device name, ex. "A01"
  // -------------------------------------------------------------------
  #define LED1STATETOPIC "/led1state/" ALIAS  // topic for LED status
  #define LED2STATETOPIC "/led2state/" ALIAS  // topic for LED status
  #define LED3STATETOPIC "/led3state/" ALIAS  // topic for LED status
  #define LEDSWITCH "/ledswitch/" ALIAS
  #define DHTDATATOPIC "/dht/" ALIAS         // topic for dht
  #define DHTPIN    D7        // DHT data pin
  #define DHTTYPE   DHT22     // e.g. DHT11, DHT21, DHT22

  #define LEDPIN1     D0  // LED on NodeMCU board 
  #define LEDPIN2     D4
  #define LEDPIN3     D5
  #define SWPIN       D6


  DHT dht(DHTPIN, DHTTYPE);
  WiFiClient client;
  int timer = 0;
  int lastDHTRead = 0;
  int currentLED1State = 1;
  int currentLED2State=1;
  int currentLED3State=1;
  int LEDSwitchState=1;
  
  int SWState = 0;         // current state of the button
  int LastSWState = 0;     // previous state of the button
  
  int notify = 0;
  MicroGear microgear(client);

void updateLED(int state) {
    // LED on NodeMCU is on when LEDPIN=LOW
    if(state==1 && currentLED1State == 0){
        currentLED1State = 1;
        digitalWrite(LEDPIN1, LOW);    //LED ON
    }
    else if (state==0 && currentLED1State == 1) {
        currentLED1State = 0;
        digitalWrite(LEDPIN1, HIGH);  //LED OFF
    }
    microgear.publish(LED1STATETOPIC, currentLED1State);

}
void updateLED2(int state) {
    if(state==1 && currentLED2State == 0){
      currentLED2State = 1;
      digitalWrite(LEDPIN2, LOW);    //LED ON
    }
    else if (state==0 && currentLED2State == 1) {
        currentLED2State = 0;
        digitalWrite(LEDPIN2, HIGH);  //LED OFF
    }
    microgear.publish(LED2STATETOPIC, currentLED2State);
}
void updateLED3(int state) {
    if(state==1 && currentLED3State == 0){
      currentLED3State = 1;
      digitalWrite(LEDPIN3, LOW);    //LED ON
    }
    else if (state==0 && currentLED3State == 1) {
        currentLED3State = 0;
        digitalWrite(LEDPIN3, HIGH);  //LED OFF
    }
    microgear.publish(LED3STATETOPIC, currentLED3State);
}
void ledSwitch(int state) {
  if(state==1 && LEDSwitchState == 0){
      LEDSwitchState = 1;
      digitalWrite(SWPIN, LOW);    //LED ON
      digitalWrite(LEDPIN3, LOW);
      digitalWrite(LEDPIN2, LOW);
      digitalWrite(LEDPIN1, LOW);
    }
    else if (state==0 && LEDSwitchState == 1) {
        LEDSwitchState = 0;
        digitalWrite(SWPIN, HIGH);  //LED OFF
        digitalWrite(LEDPIN3, HIGH);
        digitalWrite(LEDPIN2, HIGH);
        digitalWrite(LEDPIN1, HIGH);
    }
    microgear.publish(LEDSWITCH, LEDSwitchState);
}


/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
    
    if (*(char *)msg == '0') updateLED(0); //LED1 OFF
    else if (*(char *)msg == '1') updateLED(1); //LED1 ON
    else if (*(char *)msg == '2') updateLED2(1); //LED2 ON
    else if (*(char *)msg == '3') updateLED2(0); //LED2 OFF
    else if (*(char *)msg == '4') updateLED3(1); //LED3 ON
    else if (*(char *)msg == '5') updateLED3(0); //LED3 OFF
    else if (*(char *)msg == '6') ledSwitch(1); //On
    else if (*(char *)msg == '7') ledSwitch(0); //off

}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Found new member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();  
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Lost member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setAlias(ALIAS);
    updateLED(0);
}


void setup() {

  pinMode(LEDPIN1, OUTPUT);    // set LEDPIN as OUTPUT
  digitalWrite(LEDPIN1, HIGH);  //LED OFF
  pinMode(LEDPIN2, OUTPUT);    // set LEDPIN as OUTPUT
  digitalWrite(LEDPIN2, HIGH);  //LED OFF
  pinMode(LEDPIN3, OUTPUT);    // set LEDPIN as OUTPUT
  digitalWrite(LEDPIN3, HIGH);  //LED OFF
  pinMode(SWPIN, INPUT);
    /* Add Event listeners */

    /* Call onMsghandler() when new message arraives */
    microgear.on(MESSAGE,onMsghandler);

    /* Call onFoundgear() when new gear appear */
    microgear.on(PRESENT,onFoundgear);

    /* Call onLostgear() when some gear goes offline */
    microgear.on(ABSENT,onLostgear);

    /* Call onConnected() when NETPIE connection is established */
    microgear.on(CONNECTED,onConnected);

    Serial.begin(115200);
    Serial.println("Starting...");
    dht.begin(); // initialize DHT Sensor
    /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
    /* You may want to use other method that is more complicated, but provide better user experience */
    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    /* Initial with KEY, SECRET and also set the ALIAS here */
    microgear.init(KEY,SECRET,ALIAS);

    /* connect to NETPIE to a specific APPID */
    microgear.connect(APPID);
  
}

void loop() {
    /* To check if the microgear is still connected */
    if (microgear.connected()) {
     //   Serial.println("connected");

        /* Call this method regularly otherwise the connection may be lost */
        microgear.loop();
        ReadDHTandSend();
        CheckSW1();
    }
    else {
        Serial.println("connection lost, reconnect...");
        if (timer >= 5000) {
            microgear.connect(APPID);
            timer = 0;
        }
        else timer += 100;
    }
    delay(100);
}

void ReadDHTandSend () {if(millis() - lastDHTRead > 2000){        // setup a 2-second period
    float humid = dht.readHumidity();     // read humidity
    float temp  = dht.readTemperature();  // read temperature
    lastDHTRead = millis();
          
    Serial.print("Humid: "); Serial.print(humid); Serial.print(" %, ");
    Serial.print("Temp: "); Serial.print(temp); Serial.println(" °C ");
    
    if (isnan(humid) || isnan(temp)) { // check humid/temp is valid or not
      Serial.println("Failed to read from DHT sensor!");
    }
    else{
      // from a string in this format --> "{humid},{temp}"
      String datastring = (String)humid+","+(String)temp;
      Serial.print("Sending --> ");
      Serial.println(datastring);
      microgear.publish(DHTDATATOPIC,datastring);       
      if((temp>30)&&(notify==0))
      {
           notify = 1;  
        
           microgear.publish("/@push/owner","Temperature is high");

      }
      else if(temp<28)
      {
        notify=0;
      }
      // for (ii)
      if (temp >= 25){
        digitalWrite(LEDPIN1, HIGH); 
        currentLED1State = 1;
        microgear.publish(LED1STATETOPIC, currentLED1State);
      } else {
        digitalWrite(LEDPIN1, LOW); 
        currentLED1State = 0;
        microgear.publish(LED1STATETOPIC, currentLED1State);
      }
      if (temp >= 26){
        digitalWrite(LEDPIN2, HIGH); 
        currentLED2State = 1;
        microgear.publish(LED2STATETOPIC, currentLED2State);
      } else {
        digitalWrite(LEDPIN2, LOW); 
        currentLED2State = 0;
        microgear.publish(LED2STATETOPIC, currentLED2State);
      }
      if (temp >= 27){
        digitalWrite(LEDPIN3, HIGH); 
        currentLED3State = 1;
        microgear.publish(LED3STATETOPIC, currentLED3State);
      } else {
        digitalWrite(LEDPIN3, LOW); 
        currentLED3State = 0;
        microgear.publish(LED3STATETOPIC, currentLED3State);
      }

      //for (iii)
      if (temp > 30 && humid > 90) {
        notify = 1;  
        microgear.publish("/@push/owner", "High Temperature & Humidity Alert!");
        microgear.publish("/@push/owner", "Temperature: " + (String)temp + ", " + "Humidity: " + (String)humid); 
      }
      if (temp < 27 && humid < 70) {
        notify = 1;  
        microgear.publish("/@push/owner", "Normal Temperature & Humidity Restoration");
        microgear.publish("/@push/owner", "Temperature: " + (String)temp + ", " + "Humidity: " + (String)humid);  
      }
      
      }
    }  
}

void CheckSW1()
 {
    // read the pushbutton input pin:
  SWState = digitalRead(SWPIN);

  // compare the buttonState to its previous state
  if (SWState != LastSWState) {
    // if the state has changed, increment the counter
    if (SWState == LOW) { //mean s/o press the switch
          if( LEDSwitchState == 0) {
             ledSwitch(1);
          }else {
            ledSwitch(0);
          }
    notify = 1;
    microgear.publish("/@push/owner", "Operation Reset");
       
    } 
    // Delay a little bit to avoid bouncing
    delay(5000);
  }
  // save the current state as the last state,
  //for next time through the loop
  LastSWState = SWState; //update the state
 }
