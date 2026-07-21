#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ----------------------- Pins -----------------------
const int potPin = 34;
const int buzzerPin = 25;

int animation = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED not found");
    while (true);
  }

  // -------- Startup Screen --------
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(18,10);
  display.println("EV BATTERY");

  display.setCursor(8,25);
  display.println("MANAGEMENT");

  display.setCursor(32,37);
  display.println("SYSTEM");

  display.setCursor(20,53);
  display.println("Starting...");

  display.display();

  delay(2000);
}

void loop()
{
    // Read ADC
  int adc = analogRead(potPin);

  // Convert ADC to Voltage
  float voltage = adc * 3.3 / 4095.0;

  // Convert Voltage to Battery %
  float battery = (voltage / 3.3) * 100.0;

  // Limit battery percentage
  if (battery > 100) battery = 100;
  if (battery < 0) battery = 0;

  // Battery Status
  String status;

  if (battery < 20)
  {
    status = "LOW";
    digitalWrite(buzzerPin, HIGH);
  }
  else if (battery < 80)
  {
    status = "NORMAL";
    digitalWrite(buzzerPin, LOW);
  }
  else
  {
    status = "FULL";
    digitalWrite(buzzerPin, LOW);
  }

  // OLED
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Title
  display.setCursor(18,0);
  display.println("EV BATTERY");

  // Battery Outline
  display.drawRect(90,18,30,15,SSD1306_WHITE);

  // Battery Positive Terminal
  display.fillRect(120,22,3,7,SSD1306_WHITE);

  // Charging Animation when battery > 80%
  if (battery > 80)
  {
    animation += 2;

    if (animation > 28)
    {
      animation = 0;
    }

    display.fillRect(91,19,animation,13,SSD1306_WHITE);
  }
  else
  {
    int batteryWidth = map((int)battery,0,100,0,28);
    display.fillRect(91,19,batteryWidth,13,SSD1306_WHITE);
  }

  // Voltage
  display.setCursor(0,18);
  display.print("V : ");
  display.print(voltage,2);
  display.println("V");

  // Battery %
  display.setCursor(0,34);
  display.print("Battery : ");
  display.print((int)battery);
  display.println("%");

  // Status
  display.setCursor(0,50);

  if(battery < 20)
  {
    display.println("LOW BATTERY!");
  }
  else
  {
    display.print("Status : ");
    display.println(status);
  }

  display.display();

  Serial.print("ADC = ");
  Serial.print(adc);

  Serial.print("   Voltage = ");
  Serial.print(voltage,2);

  Serial.print(" V   Battery = ");
  Serial.print((int)battery);

  Serial.print("%   Status = ");
  Serial.println(status);

  delay(300);
}