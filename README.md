# Digital_Energy_Meter
This Arduino sketch is designed for measuring voltage and current using two ADS1115 ADCs (Analog-to-Digital Converters) and displaying the results on an SSD1306 OLED display. The program collects data, calculates RMS (Root Mean Square) values for voltage and current, computes power and energy consumption, and displays these metrics.

**Dual ADS1115 ADCs:** The sketch initializes and reads from two ADS1115 ADCs, each with a unique I²C address (0x48 and 0x49). One for current, other for voltage.
OLED Display: An SSD1306 OLED display is used to show the RMS voltage, RMS current, power, and energy consumption.
High Data Rate: Both ADS1115 instances are configured to operate at the highest data rate (860 samples per second).
Continuous Conversion Mode: The ADS1115 ADCs are set to continuous conversion mode to ensure real-time data acquisition.
Energy Calculation: The sketch continuously calculates the energy consumption based on the power and time elapsed.

**Loop Function:**
Collects 100 samples of voltage and current.
Removes initial offset after calculating the average offset from both the values of current and voltage samples.
Calculates the RMS values for voltage and current of the sampled value, and applying gain corrections, we get the voltage and current values.
Displays the RMS voltage, RMS current, power, and energy consumption on the OLED display.
RMS Calculation: The calculateRMS() function computes the RMS value for the given array of samples, considering the offset.
