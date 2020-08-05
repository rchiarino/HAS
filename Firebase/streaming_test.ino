//Firebase Test for streaming, addapted code used in final version.
//Only for ESP32,
//Renzo Chiarino.
//06/10/2020

//Default Library
#include <WiFi.h>

//Search for FirebaseESP32.h on google to get the zip and install it.
#include "FirebaseESP32.h"
                                         

// WiFi credentials.                    
#define WIFI_SSID "  "  //wifi name
#define WIFI_PASSWORD "  "  //wifi password

#define FIREBASE_HOST "  "  //firebase host from config                         
#define FIREBASE_AUTH "  "  //firebase token

FirebaseData firebaseData;



//On Board led, only for testing... used with a switch on google gome app.
int LED = 2;    


//WiFi
void WIFInit(){

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

}

//Firebase
void FirebaseInit(){
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  
  //Error 
  if(!Firebase.beginStream(firebaseData, "/")){
    Serial.println("Can't begin stream connection...");
    Serial.println("Reason: " + firebaseData.errorReason());
    Serial.println();
  }

  Firebase.setStreamCallback(firebaseData, streamCallback, streamTimeoutCallback);
}


void streamCallback(StreamData data){

  Serial.println("Stream Data...");
  Serial.println(data.streamPath());
  Serial.println(data.dataPath());
  Serial.println(data.dataType());

  //Print out the value
  //Stream data can be many types which can be determined from function dataType

  if (data.dataType() == "int")
    Serial.println(data.intData());
  else if (data.dataType() == "float")
    Serial.println(data.floatData(), 5);
  else if (data.dataType() == "double")
    printf("%.9lf\n", data.doubleData());
  else if (data.dataType() == "boolean")
    Serial.println(data.boolData() == 1 ? "true" : "false");
  else if (data.dataType() == "string")
    Serial.println(data.stringData());
  else if (data.dataType() == "json")
    
    //Serial.println(data.jsonString());
  if (data.dataType() == "json" && data.dataPath() == "/3/OnOff"){
    {
    Serial.println();
    FirebaseJson &json = data.jsonObject();

    //Print all object data
    size_t len = json.iteratorBegin();
    String key, value = "";
    int type = 0;
    for (size_t i = 0; i < len; i++)
    {
      json.iteratorGet(i, type, key, value);

      //Cach the value and turns the led on or off
      if(value == "true"){

        Serial.println("on");
        digitalWrite(LED, HIGH);

      }

      if(value == "false"){

        Serial.println("off");
        digitalWrite(LED, LOW);

      }


    }
    json.iteratorEnd();
  }
  }

  

  
    

}

//Time out
void streamTimeoutCallback(bool timeout){
  if(timeout){
    Serial.println("Stream timeout, resume streaming...");
  }
}




void setup() {

  Serial.begin(9600);

  pinMode(LED, OUTPUT);

  WIFInit();
  FirebaseInit();

}

void loop() { 



}      
