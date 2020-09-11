#define BMS_ALERT_PIN 2     // attached to interrupt INT0
#define BMS_BOOT_PIN 7      // connected to TS1 input
#define BMS_I2C_ADDRESS 0x18
#include<math.h>
#include<EEPROM.h>
#include<bq769x0.h> //library for our BMS
bq769x0 BMS(bq76920, BMS_I2C_ADDRESS);    // battery management system object
int Current, Voltage;
  float i,v,dI,dV,R,Q;
  float Ro = 1e-3; //Ro is internal resistance of cell at the time of manufacturing
  float a,b;
  float capacity = 4.5*3600; //if capacity is 4.5Ah then its value in coulomb is 4.5*3600 don't forget this conversion
  float SOC = 0;
  float SOH = 0;
  float soc;// addresses in eeprom
  float soh;// addresses in eeprom  
  float DOD;// address in eeprom
  int c[5]; // no. of hardware interrupts decide as per microcontroller
  int k=1; //counting the no. of times protection ckt. has tripped
  int RST;
void BatteryManagement()
{
  int err = BMS.begin(BMS_ALERT_PIN, BMS_BOOT_PIN);

  BMS.setTemperatureLimits(-20, 45, 0, 45);
  BMS.setShuntResistorValue(5);
  BMS.setShortCircuitProtection(14000, 200);  // delay in us
  BMS.setOvercurrentChargeProtection(8000, 200);  // delay in ms
  BMS.setOvercurrentDischargeProtection(8000, 320); // delay in ms
  BMS.setCellUndervoltageProtection(2600, 2); // delay in s
  BMS.setCellOvervoltageProtection(3650, 2);  // delay in s

  BMS.setBalancingThresholds(0, 3300, 20);  // minIdleTime_min, minCellV_mV, maxVoltageDiff_mV
  BMS.setIdleCurrentThreshold(100);
  BMS.enableAutoBalancing();
  BMS.enableDischarging();
}

void updates() // data from IC 
  {
    BMS.update();  // should be called at least every 20 ms
    //BMS.printRegister();
  } 
void SOC_Calc()
 {
  //SOC Calculation
  v = analogRead(Voltage);
  a = v;
  delay(1);
  v = analogRead(Voltage);
  dV = v-a;
  while(dV<0) //dV<0 signifies discharging
  { 
    i = analogRead(Current);
    Q = Q+i*1e-3; //current is read at every 1 millisecond
    SOC = (1-Q/capacity)*100;  
    delay(1);
  }
  while(dV>0)
  {
    i = analogRead(Current);
    Q = Q+i*1e-3;
    SOC = (Q/capacity)*100;
    delay(1);
  }
 }
void SOH_Calc()
{
  //SOH Calculation
  while(v>=4.11) //Take value as close as possible to end of charge voltage as per chemistry and number of cells in series
  {
     a = v;
     delay(1);
     v = analogRead(Voltage);
     dV = abs(v-a);
     i = analogRead(Current);
     b=i;
     delay(1);
     i = analogRead(Current);
     dI= abs(b-i);
     R = dV/dI;
  }
  SOH=(R/Ro)*100;
  EEPROM.write(soc, SOC);
  EEPROM.write(soh, SOH);
  EEPROM.write(DOD, Q);
}
 void overcurrent()
 {
  while(k<=5)
   {
    delay(10000);
    k++;
   }
  }
 void setup()
 {
  pinMode(Current, INPUT);
  pinMode(Voltage, INPUT);
  Q = EEPROM.read(DOD);
  attachInterrupt(c[0],overcurrent,FALLING);
  attachInterrupt(c[1], overcurrent, FALLING);
  attachInterrupt(c[2], overcurrent, FALLING);
  attachInterrupt(c[3], overcurrent, FALLING);
 }
void loop()
 {
  SOC_Calc();
  SOH_Calc();
  capacity = capacity*(R/Ro);//Capacity of battery degrades over time.
 }
