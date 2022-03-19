#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> //library NodeMCU atau ESP8266
#include <BlynkSimpleEsp8266.h> //library Blynk
#include <ThingSpeak.h> //library thingspeak
#define TRIGGERPIN D1 //pin Trig pada HC-SR04 dihubungkan ke pin D1 NodeMCU
#define ECHOPIN    D2 //pin Echo pada HC-SR04 dihubungkan ke pin D2 NodeMCU

unsigned long ch_no = 1462722; //channel no di Thingspeak
const char * write_api = "EUNN33DB9SA2AG3D"; //write api key di Thingspeak

char auth[] = "LgiRd4gvQT5I7xq5RLNn8UviIsHv748o"; //token pada Blynk
char ssid[] = "ZTEF660"; //nama wifi
char pass[] = "qwerty123";//password wifi

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000;

WiFiClient client; //memanggil WiFi Client

long duration; //deklarasi durasi
int distance; //jarak
WidgetLCD lcd(V1); //layar LCD pada aplikasi Blynk

void setup()
{
  pinMode(TRIGGERPIN, OUTPUT); //pin trigger sebagai output data
  pinMode(ECHOPIN, INPUT); //pin echo sebagai input data
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass); //inisiasi Blynk
  WiFi.begin(ssid, pass); //inisiasi koneksi WiFi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Terkoneksi");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client); //inisiasi server Thingspeak
  startMillis = millis();
  
  lcd.clear(); //membersihkan layar LCD pada tampilan di Blynk
  lcd.print(0, 0, "Jarak dalam cm"); //menampilkan jarak dalam cm di Blynk
}

void loop()
{
  lcd.clear(); //membersihkan layar LCD pada tampilan di Blynk
  lcd.print(0, 0, "Jarak dalam cm"); //menampilkan jarak dalam cm di Blynk
  
  Blynk.run(); //menjalankan Blynk
  
  digitalWrite(TRIGGERPIN, LOW);
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGERPIN, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGERPIN, LOW);
  duration = pulseIn(ECHOPIN, HIGH);
  distance = duration*0.034/2;
  lcd.print(7, 1, distance);
  Serial.print(distance);
  
  if (distance <= 5){
    Blynk.notify("Peringatan: Air sudah mau penuh"); //notifikasi di Blynk
    delay(1500);
  }
  currentMillis = millis();
  
  if (currentMillis - startMillis >= period)
  {
    ThingSpeak.setField(1, distance);
    ThingSpeak.writeFields(ch_no, write_api);
    startMillis = currentMillis;
  }
  Serial.println("Cm");
  delay(2000);
}
