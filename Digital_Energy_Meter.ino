#include <Wire.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Create two ADS1115 instances
Adafruit_ADS1115 ads1; // First ADS1115 for the (0x48) address object
Adafruit_ADS1115 ads2; // Second ADS1115 for the (0x49) address object

#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int numSamples = 100;
float voltageValues[numSamples];
float currentValues[numSamples];
int t;
double energy;
float voltage_offset;
float current_offset;
double voltage_gain = 0.10600427;
double current_gain = 0.00212065;
void setup() {
  Serial.begin(115200);
  
  // Initialize the ADS1115 instance
  ads1.setGain(GAIN_ONE);    
  ads2.setGain(GAIN_ONE);    

  t = micros();

  // Starting ads1 at 0x48 I²C position
  if (!ads2.begin(0x48)) {
    Serial.println("Failed to initialize ADS1.");
    while (1);
    }

  // Starting ads1 at 0x49 I²C position, ADDR is set to high for this address
  if (!ads1.begin(0x49)) {
    Serial.println("Failed to initialize ADS2.");
    while (1);
    }
   
   if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while(1); // Don't proceed, loop forever
   }
  // Set the data rate for both instances to the highest (860 SPS)
  ads1.setDataRate(RATE_ADS1115_860SPS);
  ads2.setDataRate(RATE_ADS1115_860SPS);

  // Set gain for both instances
  ads1.setGain(GAIN_ONE);
  ads2.setGain(GAIN_ONE);

  // Start continuous conversion mode on channel 0 for both instances
  ads1.startADCReading(ADS1X15_REG_CONFIG_MUX_SINGLE_0, true);
  ads2.startADCReading(ADS1X15_REG_CONFIG_MUX_SINGLE_0, true);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Initializing...");
  display.display();
  delay(200);

  energy = 0;

  Serial.println("Setup complete, starting loop...");
}

void loop() {
  t = micros() ;

  voltage_offset = 0;
  current_offset = 0;

  // Get the latest conversion results from both ADS1115 instances
  for (int i = 0; i < numSamples; i++) {
    voltageValues[i] = ads2.getLastConversionResults();
    voltage_offset += voltageValues[i]/100;

    currentValues[i] = ads1.getLastConversionResults();
    current_offset += currentValues[i]/100;
  }
  // Serial.println("Data collection complete.");


  float rmsVoltage = calculateRMS(voltageValues, numSamples,voltage_offset);
  if(rmsVoltage < 100)
  {
    rmsVoltage = 0;
  }
  float rmsCurrent = calculateRMS(currentValues, numSamples,current_offset);
  if(rmsCurrent < 80)
  {
    rmsCurrent = 0;
  }
  rmsVoltage = rmsVoltage*voltage_gain;
  rmsCurrent = rmsCurrent*current_gain;

  
  // Serial.print(t1);
  // Serial.print(",");
  // Serial.print("Voltage and Current");
  // for (int i = 0; i < numSamples; i++) {
  // Serial.print(voltageValues[i]);
  // Serial.print(",");
  // Serial.println(currentValues[i]);
  // }
  // Serial.println("RMS Vol and Curr");
  
  // Serial.print(rmsVoltage);
  // Serial.print(",");
  // Serial.print(rmsCurrent);
  
  display.clearDisplay();
  display.setCursor(0, 0); // Set cursor to the top-left corner
  display.print("Voltage(V)=  ");
  display.println(rmsVoltage);
  display.print("Current(A): ");
  display.println(rmsCurrent);
  display.print("Power(W): ");
  display.println(rmsCurrent*rmsVoltage);
  int t1 = micros()-t;
  display.print("Energy: ");
  energy += rmsCurrent*rmsVoltage*t1/(3600000000000);
  display.println(energy,6);
  display.display();
  
  // Small delay to avoid flooding the Serial Monitor
}


float calculateRMS(float *array, int length, int offset) {
  float sumOfSquares = 0.0;
  for (int i = 0; i < length; i++) {
    sumOfSquares += (array[i]-offset) * (array[i]-offset);
  }
  float meanOfSquares = sumOfSquares / length;
  return sqrt(meanOfSquares);
}