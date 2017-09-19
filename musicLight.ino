#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define NOTE_D0 -1
#define NOTE_D1 294
#define NOTE_D2 330
#define NOTE_D3 350
#define NOTE_D4 393
#define NOTE_D5 441
#define NOTE_D6 495
#define NOTE_D7 556

#define NOTE_DL1 147
#define NOTE_DL2 165
#define NOTE_DL3 175
#define NOTE_DL4 196
#define NOTE_DL5 221
#define NOTE_DL6 248
#define NOTE_DL7 278

#define NOTE_DH1 589
#define NOTE_DH2 661
#define NOTE_DH3 700
#define NOTE_DH4 786
#define NOTE_DH5 882
#define NOTE_DH6 990
#define NOTE_DH7 112
//以上部分是定义是把每个音符和频率值对应起来，用NOTE_D+数字表示音符，NOTE_DH+数字表示上面有点的那种音符，NOTE_DL+数字表示下面有点的那种音符
#define WHOLE 1
#define HALF 0.5
#define QUARTER 0.25
#define EIGHTH 0.25
#define SIXTEENTH 0.625

//这部分是用英文对应了拍子，这样后面也比较好看
//葫芦娃
int tune1[] = 
{
  NOTE_DH1,NOTE_D6,NOTE_D5,NOTE_D6,NOTE_D0,
  NOTE_DH1,NOTE_D6,NOTE_D5,NOTE_DH1,NOTE_D6,NOTE_D0,NOTE_D6,
  NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D6,NOTE_D0,NOTE_D6,
  NOTE_DH1,NOTE_D6,NOTE_D5,NOTE_DH1,NOTE_D6,NOTE_D0,
  
  NOTE_D1,NOTE_D1,NOTE_D3,
  NOTE_D1,NOTE_D1,NOTE_D3,NOTE_D0,
  NOTE_D6,NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D6,
  NOTE_D5,NOTE_D1,NOTE_D3,NOTE_D0,
  NOTE_DH1,NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D6,
  NOTE_D5,NOTE_D1,NOTE_D2,NOTE_D0,
  NOTE_D7,NOTE_D7,NOTE_D5,NOTE_D3,
  NOTE_D5,
  NOTE_DH1,NOTE_D0,NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D5,NOTE_D6,NOTE_D6,
  NOTE_D0,NOTE_D5,NOTE_D1,NOTE_D3,NOTE_D0,
  NOTE_DH1,NOTE_D0,NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D5,NOTE_D6,NOTE_D6,
  NOTE_D0,NOTE_D5,NOTE_D1,NOTE_D2,NOTE_D0,
  NOTE_D3,NOTE_D3,NOTE_D1,NOTE_DL6,
  NOTE_D1,
  NOTE_D3,NOTE_D5,NOTE_D6,NOTE_D6,
  NOTE_D3,NOTE_D5,NOTE_D6,NOTE_D6,
  NOTE_DH1,NOTE_D0,NOTE_D7,NOTE_D5,
  NOTE_D6,
};//这部分就是整首曲子的音符部分，用了一个序列定义为tune，整数
float duration1[]=
{
  1,1,0.5,0.5,1,
  0.5,0.5,0.5,0.5,1,0.5,0.5,
  0.5,1,0.5,1,0.5,0.5,
  0.5,0.5,0.5,0.5,1,1,
  
  1,1,1+1,
  0.5,1,1+0.5,1,
  1,1,0.5,0.5,1,
  0.5,1,1+0.5,1,
  0.5,0.5,0.5,0.5,1+1,
  0.5,1,1+0.5,1,
  1+1,0.5,0.5,1,
  1+1+1+1,
  0.5,0.5,0.5+0.25,0.25,0.5+0.25,0.25,0.5+0.25,0.25,
  0.5,1,0.5,1,1,
  0.5,0.5,0.5+0.25,0.25,0.5+0.25,0.25,0.5+0.25,0.25,
  0.5,1,0.5,1,1,
  1+1,0.5,0.5,1,
  1+1+1+1,
  0.5,1,0.5,1+1,
  0.5,1,0.5,1+1,
  1+1,0.5,0.5,1,
  1+1+1+1
};//这部分是整首曲子的接拍部分，也定义个序列duration，浮点（数组的个数和前面音符的个数是一样的，一一对应么）

//欢乐颂
int tune2[]=
{
   NOTE_D3,NOTE_D3,NOTE_D4,NOTE_D5,
   NOTE_D5,NOTE_D4,NOTE_D3,NOTE_D2,
   NOTE_D1,NOTE_D1,NOTE_D2,NOTE_D3,
   NOTE_D3,NOTE_D2,NOTE_D2,
   NOTE_D3,NOTE_D3,NOTE_D4,NOTE_D5,
   NOTE_D5,NOTE_D4,NOTE_D3,NOTE_D2,
   NOTE_D1,NOTE_D1,NOTE_D2,NOTE_D3,
   NOTE_D2,NOTE_D1,NOTE_D1,
   NOTE_D2,NOTE_D2,NOTE_D3,NOTE_D1,
   NOTE_D2,NOTE_D3,NOTE_D4,NOTE_D3,NOTE_D1,
   NOTE_D2,NOTE_D3,NOTE_D4,NOTE_D3,NOTE_D2,
   NOTE_D1,NOTE_D2,NOTE_DL5,NOTE_D0,
   NOTE_D3,NOTE_D3,NOTE_D4,NOTE_D5,
   NOTE_D5,NOTE_D4,NOTE_D3,NOTE_D4,NOTE_D2,
   NOTE_D1,NOTE_D1,NOTE_D2,NOTE_D3,
   NOTE_D2,NOTE_D1,NOTE_D1
};
float duration2[]= 
{
    1,1,1,1,
    1,1,1,1,
    1,1,1,1,
    1+0.5,0.5,1+1,
    1,1,1,1,
    1,1,1,1,
    1,1,1,1,
    1+0.5,0.5,1+1,
    1,1,1,1,
    1,0.5,0.5,1,1,
    1,0.5,0.5,1,1,
    1,1,1,1,
    1,1,1,1,
    1,1,1,0.5,0.5,
    1,1,1,1,
    1+0.5,0.5,1+1,
};


int length1;//这里定义一个变量，后面用来表示共有多少个音符
int length2;
int whichMusic=2;
const int tonePin=D5;//蜂鸣器的pin

// Lab11 使用 74HC595 和三支脚位控制 8 颗LED
 
// 接 74HC595 的 ST_CP (pin 12,latch pin)
int latchPin = D6;
// 接 74HC595 的 SH_CP (pin 11, clock pin)
int clockPin = D7;
// 接 74HC595 的 DS (pin 14)
int dataPin = D8;

const char* ssid = "biloba";//连接的路由器的名字
const char* password = "2373880422";//连接的路由器的密码
const char* mqtt_server = "39.108.161.122";//服务器的地址 
const int port=61613;//服务器端口号
const char* inTopic = "musicLight";
bool isPlay=false;
bool isSend=false;

WiFiClient espClient;
PubSubClient client(espClient);
String json="";
int* tune;
float* dur;
int length;


void setup_wifi() {//自动连WIFI接入网络
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("...");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {//用于接收数据
  String str((char*)payload);
  json=str.substring(0,length);
  Serial.println(json);
  decodeJson();
}

void reconnect() {//等待，直到连接上服务器
  while (!client.connected()) {//如果没有连接上
    if (client.connect("biloba")+random(999999999)) {//接入时的用户名，尽量取一个很不常用的用户名
      client.subscribe(inTopic);//接收外来的数据时的intopic
    } else {
      Serial.print("failed, rc=");//连接失败
      Serial.print(client.state());//重新连接
      Serial.println(" try again in 5 seconds");//延时5秒后重新连接
      delay(5000);
    }
  }
}



void setup()
{
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(tonePin,OUTPUT);//设置蜂鸣器的pin为输出模式
  
  length1 = sizeof(tune1)/sizeof(tune1[0]);//这里用了一个sizeof函数，可以查出tone序列里有多少个音符
  length2 = sizeof(tune2)/sizeof(tune2[0]);

  setup_wifi();//自动连WIFI接入网络
  client.setServer(mqtt_server, port);//端口号
  client.setCallback(callback); //用于接收服务器接收的数据

  offAllLight();
}

//音乐灯
void musicLight(int* tune,float* duration,int length)
{
  for(int x=0;x<length&&isPlay;x++)//循环音符的次数
  {
    tone(tonePin,tune[x]);//此函数依次播放tune序列里的数组，即每个音符

    digitalWrite(latchPin, LOW);
    // 使用 shiftOut 函式送出資料
    shiftOut(dataPin, clockPin, MSBFIRST, tune[x]/4);  
    // 送完資料後要把 latchPin 拉回成高電位
    digitalWrite(latchPin, HIGH);
    
    delay(400*duration[x]);//每个音符持续的时间，即节拍duration，400是调整时间的越大，曲子速度越慢，越小曲子速度越快
    noTone(tonePin);//停止当前音符，进入下一音符

    reconnect();//确保连上服务器，否则一直等待。
    client.loop();//MUC接收数据的主循环函数。
  }
}

//全灭
void offAllLight()
{
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);  
    digitalWrite(latchPin, HIGH);
}

void  decodeJson() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
   if (!root.success()) {
     Serial.println("parseObject() failed");
   }
   int op=root["op"];
   switch(op){
    case 0:{
      isPlay=root["sw"];
      break;
    }
    case 1:{
      isPlay=false;
      length=root["cou"];
      tune=new int[length];
      dur=new float[length];
      for(int i=0;i<length;i++){
        String json=root["leds"][i];
         JsonObject& jsonLed = jsonBuffer.parseObject(json);
         int t=jsonLed["num"];
         tune[i]=t*4;
         float d=jsonLed["dur"];
         dur[i]=d/0.4;
      }
      isSend=true;
      break;
    }
   }
}

void loop()
{

  reconnect();//确保连上服务器，否则一直等待。
  client.loop();//MUC接收数据的主循环函数。

  if(isPlay){
    switch(whichMusic){
      case 1:{
        musicLight(tune1,duration1,length1);
        whichMusic=2;
        break;
      }
      case 2:{
        musicLight(tune2,duration2,length2);
        whichMusic=1;
        break;
      }
    }

    offAllLight();
    delay(5000);//等待5秒后，循环重新开始
  }else{
    offAllLight();
  }

  if(isSend){
    isPlay=true;
    musicLight(tune,dur,length);
    isSend=false;
    offAllLight();
    delay(5000);//等待2秒后，循环重新开始
  }
}

