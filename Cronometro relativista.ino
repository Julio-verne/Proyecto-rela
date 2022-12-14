#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//On an arduino UNO: A4(SDA), A5(SCL)
#define OLED_RESET -1 //Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3c //See datasheet for Address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define rxPin 2
#define txPin 3
SoftwareSerial neogps(rxPin,txPin);
int i;
float tr;
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  //Begin serial communication Arduino IDE (Serial Monitor)

  //Begin serial communication Neo6mGPS
  neogps.begin(9600);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.display();
  

}

void loop() {
    
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (neogps.available())
    {
      if (gps.encode(neogps.read()))
      {
        newData = true;
      }
    }
  }

  //If newData is true
  if(newData == true)
  {
    newData = false;
    print_speed();
  }
  else
  {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.setTextSize(3);
    display.print("No Data");
    display.display();
  }  
  
}

void print_speed()
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
       
  if (gps.location.isValid() == 1)
  {
   //String gps_speed = String(gps.speed.kmph());
    if(i<999){
        delay(0);
        float v=gps.speed.mps();
        tr=i*(1-((v*v)/(30*30)));//2=velocidad luz
        i++;
    }
    display.setCursor(0, 25);
    display.setTextSize(4);
    display.print(tr,1);
    
    display.setCursor(105, 35);
    display.setTextSize(2);
    display.print("s");

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("SAT:");
    display.setCursor(25, 0);
    display.print(gps.satellites.value());

    display.display();
    
  }

}
