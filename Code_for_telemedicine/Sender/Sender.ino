#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define TCP_SERVER_ADDR "bemfa.com"
#define TCP_SERVER_PORT "8344"

//WIFI Name
#define DEFAULT_STASSID  "xxx"
//WIFI Password
#define DEFAULT_STAPSW "xxxxxxxx"
//User key
String UID = "xxxxxxxxxxxx";
//TOPIC
String TOPIC = "Telemedicine";


///*********************************************///

//upDataTime
#define upDataTime 1*1000
//upheartTime
#define upheartTime 30*1000
//SampleTime
#define SampleTime 50
#define SampleTime1 80
//upmsg
String upmsg = "";
int intNumber = 0;
int num = 0;
int num1 = 0;
//MAX_PACKETSIZE
#define MAX_PACKETSIZE 512





//tcpinitialization
WiFiClient TCPclient;
String TcpClient_Buff = "";
unsigned int TcpClient_BuffIndex = 0;
unsigned long TcpClient_preTick = 0;
unsigned long preHeartTick = 0;
unsigned long predataTick = 0;
unsigned long preTCPStartTick = 0;
unsigned long preMsgTick = 0;
unsigned long preMsgTick1 = 0;
bool preTCPConnected = false;

//Connect WIFI
void doWiFiTick();
void startSTA();

//TCP connect
void doTCPClientTick();
void startTCPClient();
void sendtoTCPServer(String p);
void doMsgTick();

//sendtoTCPServer
void sendtoTCPServer(String p){
  
  if (!TCPclient.connected()) 
  {
    Serial.println("Client is not readly");
    return;
  }
  TCPclient.print(p);
  Serial.println("[Send to TCPServer]:String");
  Serial.println(p);
}


//startTCPClient
void startTCPClient(){
  if(TCPclient.connect(TCP_SERVER_ADDR, atoi(TCP_SERVER_PORT))){
    Serial.print("\nConnected to server:");
    Serial.printf("%s:%d\r\n",TCP_SERVER_ADDR,atoi(TCP_SERVER_PORT));
    preTCPConnected = true;
    preHeartTick = millis();
    predataTick = millis();
    TCPclient.setNoDelay(true);   
  }
  else{
    Serial.print("Failed connected to server:");
    Serial.println(TCP_SERVER_ADDR);
    TCPclient.stop();
    preTCPConnected = false;
  }
  preTCPStartTick = millis();
}


//doTCPClientTick
void doTCPClientTick(){
   if(WiFi.status() != WL_CONNECTED) return;
   if (!TCPclient.connected()) {
    if(preTCPConnected == true){
      preTCPConnected = false;
      preTCPStartTick = millis();
      Serial.println();
      Serial.println("TCP Client disconnected.");
      TCPclient.stop();
      }
      else if(millis() - preTCPStartTick > 1*1000)
      startTCPClient();
  }
  else
  {
    if (TCPclient.available()) {
      char c =TCPclient.read();
      TcpClient_Buff +=c;
      TcpClient_BuffIndex++;
      TcpClient_preTick = millis();
      
      if(TcpClient_BuffIndex>=MAX_PACKETSIZE - 1){
        TcpClient_BuffIndex = MAX_PACKETSIZE-2;
        TcpClient_preTick = TcpClient_preTick - 200;
      }
      preHeartTick = millis();
      predataTick = millis();
    }
    if(millis() - preHeartTick >= upheartTime){
      preHeartTick = millis();
      String upstr = "";
      upstr = "cmd=0&msg=ping\r\n";
      intNumber++;
      sendtoTCPServer(upstr);
      upstr = "";
    }
    if(millis() - predataTick >= upDataTime){
      predataTick = millis();
      String upstr = "";
      upstr = "cmd=2&uid="+UID+"&topic="+TOPIC+"&msg="+upmsg+"\r\n";
      //intNumber++;
      sendtoTCPServer(upstr);
      upstr = "";
      upmsg = "";
      
    }
  }
  if((TcpClient_Buff.length() >= 1) && (millis() - TcpClient_preTick>=200))
  {//data ready
    TCPclient.flush();
    Serial.println("Buff");
    Serial.println(TcpClient_Buff);
    TcpClient_Buff="";
    TcpClient_BuffIndex = 0;
  }
}

void startSTA(){
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(DEFAULT_STASSID, DEFAULT_STAPSW);

}




void doWiFiTick(){
  static bool startSTAFlag = false;
  static bool taskStarted = false;
  static uint32_t lastWiFiCheckTick = 0;

  if (!startSTAFlag) {
    startSTAFlag = true;
    startSTA();
    Serial.printf("Heap size:%d\r\n", ESP.getFreeHeap());
  }
  if ( WiFi.status() != WL_CONNECTED ) {
    if (millis() - lastWiFiCheckTick > 1000) {
      lastWiFiCheckTick = millis();
    }
  }
  else {
    if (taskStarted == false) {
      taskStarted = true;
      Serial.print("\r\nGet IP Address: ");
      Serial.println(WiFi.localIP());
      startTCPClient();
    }
  }
}

void doMsgTick(){
    int k = 0;
    if(millis() - preMsgTick >= SampleTime){
      preMsgTick = millis(); 
      num =  analogRead(A0);
      upmsg = upmsg + num + ' ';
    }
}

void setup() {
  Serial.begin(115200);
}
void loop() {
  doMsgTick();
  doWiFiTick();
  doTCPClientTick();
}
