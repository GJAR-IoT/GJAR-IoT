#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SimpleDHT.h> //kniznica k DHT

// WiFi Parameters
const char* ssid = "...";
const char* password = "...";
#define redLED 12
#define greenLED 16
#define GND 14

int pinDHT22 = 2;   //DHT pin je D4
SimpleDHT22 dht22; // typ DHT je DHT22

#define INTERVAL    3 // pocet docastnych hodnot na priemerovanie
#define dispAVG_INTERVAL  3 // po kolkych meraniach sa posiela priemer teploty
#define sensors_INTERVAL 2500 //cas pokial senzory nameraju nove hodnoty
#define EEPROM_size 512 //velkost EEPROMu
#define EEPROM_INTERVAL 3 //pocet zalohovanych merani

float DHTtemperature = 0;  //vynulovanie zaciatocnej meranej teploty
float DHThumidity = 0;     //vynulovanie zaciatocnej meranej vlhkosti
int err = SimpleDHTErrSuccess;  // sprava ak meranie bude chybne

int sensorValue;  //citana hodnota z LDR

float tmpT[INTERVAL];  // docastna hodnota, z kt. sa bude pocitat priemer
float avgT;            // priemerna teplota;
int iTmp = 0;         //index docasnej hodnoty
int dispAVG = 0;      //zobrazi priemerne hodnoty ked disAVG bude rovne nule

float tmpH[INTERVAL];  // docastna hodnota, z kt. sa bude pocitat priemer
float avgH;           // priemerna vlhkost;

float voltage = 0;  //vynulovanie pociatocneho napatia
float avgV;            //priemerne napatie
float tmpV[INTERVAL];  //docastna hodnota, z kt. sa bude pocitat priemer

os_timer_t timJSONalive;

void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  
  os_timer_setfn(&timJSONalive, JSONalive, NULL);     // spustenie a nastavernie Timera pre snimanie teploty a spol.
  os_timer_arm(&timJSONalive,1800000, true); 
  
  delay(10);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(pinDHT22 ,INPUT_PULLUP);
  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);
  Serial.println('\n'); 
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    
    digitalWrite(redLED, HIGH);
  delay(250);
  digitalWrite(redLED, LOW);
  delay(250);
  }
  digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);

  
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
  Serial.println("----START----");
}
void loop(){
 
if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
   JSONconnect();
  } else { 
    Serial.println("Error in WiFi connection");
   }
  GetSensorsData();// volanie funkcie na meranie hodnot zo senzorov
}

void GetSensorsData() {
  //nacitanie dat z DHT a vyhodnotenie cimeranie bolo uspesne
  if ((err = dht22.read2(pinDHT22, &DHTtemperature, &DHThumidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT22 failed, err="); 
    Serial.println(err); delay(2000);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, HIGH);
  delay(100);
  digitalWrite(redLED, LOW);
  delay(100);
  digitalWrite(redLED, HIGH);
  delay(100);
  digitalWrite(redLED, LOW);
  digitalWrite(redLED, HIGH);
  delay(100);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
  JSONerror();
    return;
  }

  sensorValue = analogRead(A0);   // nacita hodnotu z LDR
  voltage = sensorValue * (5.0 / 1023.0);   // prepocita hodnotu na V


  tmpT[iTmp] = DHTtemperature; //ulozi teplotu do pola teplot
  tmpH[iTmp] = DHThumidity;   //ulozi vlhkost do pola vlhkosti
  tmpV[iTmp] = voltage;       //ulozi napatie do pola napatia
  iTmp++;                     //zvysenie indexu pola nameranych hodnot
  if (iTmp > (INTERVAL - 1)) iTmp = 0; // ak je index pola hodnot vacsi ako 2 vynuluje ho


  avgT = 0; //vynuluje sa priemerna teplota
  avgH = 0; //vynuluje sa priemerna vlhkost
  avgV = 0; //vynuluje sa priemerne napatie
  for (int i = 0; i < INTERVAL; i++) // spocitame vsetky ulozene teploty
  {
    avgT = avgT + tmpT[i]; //scitanie hodnot teploty
    avgH = avgH + tmpH[i]; //scitanie hodnot vlhkosti
    avgV = avgV + tmpV[i]; //scitanie hodnot napatia
  }
  avgT = avgT / INTERVAL; //priemerna teplota je podiel spocitanych teplot s ich poctom
  avgH = avgH / INTERVAL; //priemerna vlhkost je podiel spocitanych vlhkosti s ich poctom
  avgV = avgV / INTERVAL; //priemerne napatie je podiel spocitanych napati s ich poctom

  if (dispAVG == 0) // ak dispAVG je nula zobrazi priemerne hodnoty
  {
      Serial.println("--------------------------");
      Serial.print("AVG Temp: ");
      Serial.print(avgT);
      Serial.println(" °C ");
      Serial.print("AVG Hum: ");
      Serial.print(avgH);
      Serial.println(" %  ");
      Serial.print("AVG Voltage: ");
      Serial.print(avgV);
      Serial.println(" V");
      Serial.println("--------------------------");
    dispAVG++;
    
    if (avgT > 0 && avgH > 0 && avgV > 0){
      JSONdata();
      }else{
        Serial.println("Namerane hodnoty su chybne!");
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);
        delay(15000);
        digitalWrite(greenLED, HIGH);
        digitalWrite(redLED, LOW);
        JSONerror();
        }
  
  }
  else //ak je dispAVG iny ako 0 neprebehne zobrazenie priemernych hodnot
  {
    dispAVG++;
    if (dispAVG > (dispAVG_INTERVAL - 1)) dispAVG = 0; // ak dispAVG dosiahne hodnotu pri ktorej sa ma zobrazit priemerna hodnota dispAVG vynuluje
  }


  delay(sensors_INTERVAL); //cas pokial senzory nameraju nove hodnoty
}

void JSONconnect(){
 
    StaticJsonBuffer<100> JSONconnect;   //Declaring static JSON buffer
    JsonObject& connecT = JSONconnect.createObject(); 
    connecT["id"] = 0;
    char connectMessage [100];
    connecT.prettyPrintTo(connectMessage, sizeof(connectMessage));

    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("https://iot.gjar-po.sk/api/v1/connect");      //prikaz zmenit na daco ine napr. connect
    http.addHeader("Content-Type", "application/json");  //specifikuje ze ide o json
 
    int httpConnect = http.POST(connectMessage);   //posle request
    String payloadConnect = http.getString();                                        //Get the response payload
    Serial.println(httpConnect);   //Print HTTP return code
    Serial.println(payloadConnect);    //Print request response payload
 
    http.end();  //Close connection
 
  }

void JSONdata() {
   
    DynamicJsonBuffer JSONdata;
    JsonObject& data = JSONdata.createObject();
    data["token"] = "dd431e8fd96bf3dfabba88184810f9ba521f5f9b6b2d6288572a7599f4143c09";
    JsonArray& dataObject = data.createNestedArray("data");  
    JsonObject& objectD = dataObject.createNestedObject();    
    objectD["time"] = "time";
    objectD["humidity"] = (avgH);
    objectD["temperature"] = (avgT);
    objectD["voltage"] = (avgV);
    char dataMessage[300];
    data.prettyPrintTo(dataMessage, sizeof(dataMessage));
   
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("https://iot.gjar-po.sk/api/v1/data");      //prikaz zmenit na daco ine napr. connect
    http.addHeader("Content-Type", "application/json");  //specifikuje ze ide o json   

    int httpData = http.POST(dataMessage);   //posle request
    String payloadData = http.getString();                                        //Get the response payload
    Serial.println(httpData);   //Print HTTP return code
    Serial.println(payloadData);    //Print request response payload
 
    http.end();  //Close connection
   }

void JSONerror(){
  
    StaticJsonBuffer<100> JSONerror;
    JsonObject& error = JSONerror.createObject();
    error["id"] = 0;
    error["level"] = "ERROR";
    char errorMessage[100];
    error.prettyPrintTo(errorMessage, sizeof(errorMessage));

    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("https://iot.gjar-po.sk/api/v1/error");      //prikaz zmenit na daco ine napr. connect
    http.addHeader("Content-Type", "application/json");  //specifikuje ze ide o json   

    int httpData = http.POST(errorMessage);   //posle request
    String payloadData = http.getString();                                        //Get the response payload
    Serial.println(httpData);   //Print HTTP return code
    Serial.println(payloadData);    //Print request response payload
 
    http.end();  //Close connection
  }

  void JSONalive(void *pArg) {

     StaticJsonBuffer<100> JSONalive;
    JsonObject& alive = JSONalive.createObject();
    alive["token"]  = "dd431e8fd96bf3dfabba88184810f9ba521f5f9b6b2d6288572a7599f4143c09";
    char aliveMessage[100];
    alive.prettyPrintTo(aliveMessage, sizeof(aliveMessage));

    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("https://iot.gjar-po.sk/api/v1/error");      //prikaz zmenit na daco ine napr. connect
    http.addHeader("Content-Type", "application/json");  //specifikuje ze ide o json   

    int httpData = http.POST(aliveMessage);   //posle request
    String payloadData = http.getString();                                        //Get the response payload
    Serial.println(httpData);   //Print HTTP return code
    Serial.println(payloadData);    //Print request response payload
 
    http.end();  //Close connection
    
    }

  void JSONview(){
    
    DynamicJsonBuffer JSONview;
    JsonObject& view = JSONview.createObject();
    view["room"] = 0;   
    JsonArray& viewObject = view.createNestedArray("time");
    JsonObject& objectV = viewObject.createNestedObject();
    objectV["time"] = "str";
    objectV["time-from"] = "str";
    objectV["time-to"] = "str";
    view["data"] = "list of str";
    char viewMessage[200];
    view.prettyPrintTo(viewMessage, sizeof(viewMessage));

 HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("https://iot.gjar-po.sk/api/v1/view");      //prikaz zmenit na daco ine napr. connect
    http.addHeader("Content-Type", "application/json");  //specifikuje ze ide o json   

    int httpData = http.POST(viewMessage);   //posle request
    String payloadData = http.getString();                                        //Get the response payload
    Serial.println(httpData);   //Print HTTP return code
    Serial.println(payloadData);    //Print request response payload
 
    http.end();  //Close connection
    }