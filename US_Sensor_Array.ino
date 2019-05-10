/*
// /////////////////////////////////////////////////////////////////////// COPYRIGHT NOTICE
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with Pip-Project.  If not, see <http://www.gnu.org/licenses/>.
// /////////////////////////////////////////////////////////////////////// COPYRIGHT NOTICE
// /////////////////////////////////////////////////////////////////////// VERSION CONTROL
// PAGE CREATED BY: Phillip Kraguljac
// PAGE CREATED DATE: 2019-05-10
// DATE       || NAME           || MODIFICATION
// 2019-05-10   || Phillip Kraguljac    || Created.
// /////////////////////////////////////////////////////////////////////// VERSION CONTROL
*/

#include <LiquidCrystal.h>

// /////////////////////////////////////////////////////////////////////// USER DEFINABLE VARIABLES

int Startup_Delay = 20;                                                             // Cycle time allocated for start-up.
int Restart_Delay = 20;                                                             // Cycle time allocated for the re-trigger delay.
int Pallet_Distance = 100;                                                          // Expected distance between sensor and pallet beam.
int Pallet_Distance_Error = 60;                                                     // Margin of error for pallet distance.
int Pallet_Counter_Error = 2;                                                       // Margin of error for logic cyclic operations.
int Stock_Distance_Allowable = 20;                                                  // TBC.
int Stock_Distance_Error = 20;                                                      // TBC.
int Runner_Count = 5;                                                               // Expected number of pallet runner / pallet.
int Runner_Height = 140;                                                            // Expected distance between sensor and pallet runner.
int Runner_Height_Error = 20;                                                       // Margin of error for pallet distance.
int Runner_Error_Limit = 1;                                                         // Margin of error for logic cyclic operations.
int Slip_Sheet_Height = 240;                                                        // TBC.
int Slip_Sheet_Height_Error = 10;                                                   // TBC.


// /////////////////////////////////////////////////////////////////////// SYSTEM VARIABLES

const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;                           // Required for LCD.
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                                          // Required for LCD.
int LCD_Inputs = A0;                                                                // Refer to electrical schematics.
int Sensor_Trigger_A1 = 22;
int Sensor_Echo_A1 = 23;
int Sensor_Trigger_B1 = 24;
int Sensor_Echo_B1 = 25;
int Sensor_Trigger_C1 = 26;
int Sensor_Echo_C1 = 27;
int Sensor_Trigger_A2 = 28;
int Sensor_Echo_A2 = 29;
int Sensor_Trigger_B2 = 30;
int Sensor_Echo_B2 = 31;
int Sensor_Trigger_C2 = 32;
int Sensor_Echo_C2 = 33;
int Visual_Error_Indicator = 39;
int Override_Switch = 53;
bool A1_Pallet_Detected = false;                                                    // Flag used for detecting pallet beam.
bool A2_Pallet_Detected = false;                                                    // Flag used for detecting pallet beam.
bool A1_Pallet_Checked = false;                                                     // Flag used for checking beam.
bool A2_Pallet_Checked = false;                                                     // Flag used for checking beam.
int A1_Pallet_Error_Indexer = 0;                                                    // Index used for logging cycle errors.
int A2_Pallet_Error_Indexer = 0;                                                    // Index used for logging cycle errors.
int A1_Pallet_Error_Limit = 1;                                                      // Allowable cycle errors.
int A2_Pallet_Error_Limit = 1;                                                      // Allowable cycle errors.
bool B1_Stock_Overhang_Detected = false;                                            // Flag used for single side stock overhang.
bool B2_Stock_Overhang_Detected = false;                                            // Flag used for single side stock overhang.
bool Stock_Overhang_Alarm = false;                                                  // Flag used for general stock overhang.
bool C1_Runner_Detected = false;                                                    // Flag used for detecting pallet runner.
bool C2_Runner_Detected = false;                                                    // Flag used for detecting pallet runner.
bool C1_Runner_Detected_Memory = false;                                             // Memory used for detecting pallet runner.
bool C2_Runner_Detected_Memory = false;                                             // Memory used for detecting pallet runner.
bool C1_Runner_Checked = false;                                                     // Flag used for checking runner.
bool C2_Runner_Checked = false;                                                     // Flag used for checking runner.
int C1_Runner_Count_Indexer = 0;                                                    // Index used for counting pallet runners.
int C2_Runner_Count_Indexer = 0;                                                    // Index used for counting pallet runners.
int C1_Runner_Error_Indexer = 0;                                                    // Index used for logging cycle errors.
int C2_Runner_Error_Indexer = 0;                                                    // Index used for logging cycle errors.
bool C1_Last_Runner = false;                                                        // Flag used for registering last pallet runner.
bool Runner_Alarm = false;
int Mode_Time_Indexer = 0;                                                          // Index used for mode cycle timers.
bool C1_Slip_Sheet_Detected = false;                                                // TBC.
bool C2_Slip_Sheet_Detected = false;                                                // TBC.
bool Slip_Sheet_Alarm = false;                                                      // TBC.
bool General_Alarm = false;                                                         // Flag used for displaying system fault.
int General_Alarm_Count = 0;                                                        // Variable used for recording # of faults.
int General_Pallet_Count = 0;                                                       // Variable used for recording # of good pallets.
bool Override_Flag = false;                                                         // Flag used for triggering system override.
long Sensor_mm_A1;                                                                  // Variables used to store sensor distances.
long Sensor_mm_B1;
long Sensor_mm_C1;
long Sensor_mm_A2;
long Sensor_mm_B2;
long Sensor_mm_C2; 
String Mode = "START-UP";                                                           // Mode Indexer.


void setup() {

Serial.begin (9600);
pinMode(LED_BUILTIN, OUTPUT);                                                       // Declare IO's.
pinMode(Sensor_Trigger_A1, OUTPUT);                                                 
pinMode(Sensor_Echo_A1, INPUT);
pinMode(Sensor_Trigger_B1, OUTPUT);
pinMode(Sensor_Echo_B1, INPUT);
pinMode(Sensor_Trigger_C1, OUTPUT);
pinMode(Sensor_Echo_C1, INPUT);
pinMode(Sensor_Trigger_A2, OUTPUT);
pinMode(Sensor_Echo_A2, INPUT);
pinMode(Sensor_Trigger_B2, OUTPUT);
pinMode(Sensor_Echo_B2, INPUT);
pinMode(Sensor_Trigger_C2, OUTPUT);
pinMode(Sensor_Echo_C2, INPUT);
pinMode(Override_Switch, INPUT);
pinMode(Visual_Error_Indicator, OUTPUT);
digitalWrite(LED_BUILTIN, LOW);                                                     // Ensure Onboard LED is reset.
digitalWrite(Visual_Error_Indicator, LOW);                                          // Ensure strobe is reset.
lcd.clear();                                                                        // Reset LCD screen.
Mode_Restart();                                                                     // Complete Mode reset.  
}


void loop() {

Override_Flag = digitalRead(Override_Switch);

Sensor_mm_A1 = Scan_Sensor(Sensor_Trigger_A1, Sensor_Echo_A1);                      // Convert sensor signal to approx. mm value.
//Sensor_mm_B1 = Scan_Sensor(Sensor_Trigger_B1, Sensor_Echo_B1);                    // Convert sensor signal to approx. mm value.
Sensor_mm_C1 = Scan_Sensor(Sensor_Trigger_C1, Sensor_Echo_C1);                      // Convert sensor signal to approx. mm value.
Sensor_mm_A2 = Scan_Sensor(Sensor_Trigger_A2, Sensor_Echo_A2);                      // Convert sensor signal to approx. mm value.
//Sensor_mm_B2 = Scan_Sensor(Sensor_Trigger_B2, Sensor_Echo_B2);                    // Convert sensor signal to approx. mm value.
Sensor_mm_C2 = Scan_Sensor(Sensor_Trigger_C2, Sensor_Echo_C2);                      // Convert sensor signal to approx. mm value.
A1_Pallet_Detected = Detect_Pallet(Sensor_mm_A1, Pallet_Distance, Pallet_Distance_Error);                                               // Determine if pallet is present.
A2_Pallet_Detected = Detect_Pallet(Sensor_mm_A2, Pallet_Distance, Pallet_Distance_Error);                                               // Determine if pallet is present.
B1_Stock_Overhang_Detected = Detect_Stock_Overhang(Sensor_mm_B1, Pallet_Distance, Stock_Distance_Allowable, Stock_Distance_Error);      // Determine if stock overhang is present.
B2_Stock_Overhang_Detected = Detect_Stock_Overhang(Sensor_mm_B2, Pallet_Distance, Stock_Distance_Allowable, Stock_Distance_Error);      // Determine if stock overhang is present.
C1_Runner_Detected = Detect_Runner(Sensor_mm_C1, Runner_Height, Runner_Height_Error);                                                   // Determine if pallet runner is present.
C2_Runner_Detected = Detect_Runner(Sensor_mm_C2, Runner_Height, Runner_Height_Error);                                                   // Determine if pallet runner is present.
C1_Slip_Sheet_Detected = Detect_Slip(Sensor_mm_C1, Slip_Sheet_Height, Slip_Sheet_Height_Error);                                         // Determine if slip sheet is present.
C2_Slip_Sheet_Detected = Detect_Slip(Sensor_mm_C2, Slip_Sheet_Height, Slip_Sheet_Height_Error);                                         // Determine if slip sheet is present.

// /////////////////////////////////////////////////////////////////////// MODE(S)

// [MODE] => "START-UP"
if(Mode=="START-UP"){                                                               // Start-up mode in operation.
if(Override_Flag){Mode = "OVERRIDE"; Mode_Restart();}                               // Go to override if switched.
lcd.setCursor(0, 1);                                                                // Set LCD cursor.
lcd.print(Progress_Bar(Startup_Delay, Mode_Time_Indexer));                          // Display progress bar string.
Mode_Time_Indexer = Mode_Time_Indexer + 1;                                          // Increment mode counter.
if(Mode_Time_Indexer > Startup_Delay){Mode = "WAITING"; Mode_Restart();}            // Switch mode once complete.
}


// [MODE] => "WAITING"
if(Mode=="WAITING"){                                                                // Waiting for pallet mode in operation.
if(Override_Flag){Mode = "OVERRIDE"; Mode_Restart();}                               // Go to override if switched.
General_Serial_Output();                                                            // Provide 3rd party feedback via serial (if requried).
if(!A1_Pallet_Checked && A1_Pallet_Detected){                                       // When a pallet has been detected.
A1_Pallet_Error_Indexer = A1_Pallet_Error_Indexer + 1;                              // Increment indexer.
if(A1_Pallet_Error_Indexer>A1_Pallet_Error_Limit){                                  // When the indexer reaches the limit.
if(A1_Pallet_Detected&&C1_Runner_Detected){A1_Pallet_Checked = true; A1_Pallet_Error_Indexer = 0;}                                      // Check the pallet [Passed].
if(A1_Pallet_Detected&&!C1_Runner_Detected){A1_Pallet_Checked = true; Runner_Alarm = true; A1_Pallet_Error_Indexer = 0;}                // Check the pallet [Failed].
}  
}
if(!A1_Pallet_Checked && !A1_Pallet_Detected){                                      // After false trigger clears.
A1_Pallet_Error_Indexer = 0;                                                        // Reset indexer.
}
if(!A2_Pallet_Checked && A2_Pallet_Detected){                                       // When a pallet has been detected.
A2_Pallet_Error_Indexer = A2_Pallet_Error_Indexer + 1;                              // Increment indexer.
if(A2_Pallet_Error_Indexer>A2_Pallet_Error_Limit){                                  // When the indexer reaches the limit.
if(A2_Pallet_Detected&&C2_Runner_Detected){A2_Pallet_Checked = true; A2_Pallet_Error_Indexer = 0;}                                      // Check the pallet [Passed].
if(A2_Pallet_Detected&&!C2_Runner_Detected){A2_Pallet_Checked = true; Runner_Alarm = true; A2_Pallet_Error_Indexer = 0;}                // Check the pallet [Failed].
}  
}
if(!A2_Pallet_Checked && !A2_Pallet_Detected){                                      // After false trigger clears.
A2_Pallet_Error_Indexer = 0;                                                        // Reset indexer.
}
if(A1_Pallet_Checked&&A2_Pallet_Checked){Mode="PALLET"; Mode_Restart();}            // Switch mode once complete.
}


// [MODE] => "PALLET" RUNNER COUNTING MODULE
if(Mode=="PALLET"){                                                                 // Monitoring pallet mode in operation.
if(Override_Flag){Mode = "OVERRIDE"; Mode_Restart();}                               // Go to override if switched.
General_Serial_Output();                                                            // Provide 3rd party feedback via serial (if requried).
if(C1_Runner_Detected!=C1_Runner_Detected_Memory){                                  // When detecting the beginning or end of a pallet runner.
if(C1_Runner_Error_Indexer>=Runner_Error_Limit){                                    // Detect noise.
if(C1_Runner_Detected){C1_Runner_Count_Indexer = C1_Runner_Count_Indexer + 1;}      // Increment noise indexer.
C1_Runner_Detected_Memory = C1_Runner_Detected;                                     // Update operational memory.
C1_Runner_Error_Indexer = 0;                                                        // Reset error indexer - once fail trigger disappears.
}else{
C1_Runner_Error_Indexer = C1_Runner_Error_Indexer + 1;                              // Increment error counter.
}
}                                                                                   // [ ADD COUNTER FOR OTHER SIDE HERE IF REQUIRED]
if(C1_Runner_Count_Indexer>=Runner_Count){                                          // When runner count reaches expected #.
C1_Last_Runner = true;                                                              // Flag that all of the pallet runners are detected.
}
if(C1_Last_Runner&&!C1_Runner_Checked){                                             // When last runner detected and check hasn't already occurred.
  if(A1_Pallet_Detected&&!C1_Runner_Detected){C1_Runner_Checked = true; Runner_Alarm = true;}                                   // Acklowledge check and flag error if fault exists.
 if(A1_Pallet_Detected&&C1_Runner_Detected){C1_Runner_Checked = true;}              // Acklowledge check.
}
if(C1_Last_Runner&&!C2_Runner_Checked){                                             // When last runner detected and check hasn't already occurred.
  if(A2_Pallet_Detected&&!C2_Runner_Detected){C2_Runner_Checked = true; Runner_Alarm = true;}                                   // Acklowledge check and flag error if fault exists.
  if(A2_Pallet_Detected&&C2_Runner_Detected){C2_Runner_Checked = true;}             // Acklowledge check.
}
if(B1_Stock_Overhang_Detected||B2_Stock_Overhang_Detected){                         // If over hang is detected.
  Stock_Overhang_Alarm = true;                                                      // Activate alarm.
}
if(C1_Runner_Checked&&C2_Runner_Checked){Mode="REPORTING"; Mode_Restart();}         // Switch mode once complete.
}


// [MODE] => "REPORTING"
if(Mode=="REPORTING"){                                                              // Visual warning and system triggering mode in operation.
if(Override_Flag){Mode = "OVERRIDE"; Mode_Restart();}                               // Go to override if switched.
if(Runner_Alarm){                                                                   // When sub alarm(s) has been activated.
General_Alarm = true;                                                               // Set General alarm.
}
General_Serial_Output();                                                            // Provide 3rd party feedback via serial (if requried).
lcd.setCursor(0, 1);                                                                // Set LCD cursor.
lcd.print(Progress_Bar(Startup_Delay, Mode_Time_Indexer));                          // Display progress bar string.
Mode_Time_Indexer = Mode_Time_Indexer + 1;                                          // Increment mode counter.
if(Mode_Time_Indexer > Startup_Delay){Serial.println("Completed");Mode = "COUNT"; Mode_Restart();}                              // Switch mode once complete.
if(General_Alarm){digitalWrite(LED_BUILTIN, HIGH);}else{digitalWrite(LED_BUILTIN, LOW);}                                        // Set on-board LED indicator.
if(General_Alarm){digitalWrite(Visual_Error_Indicator, HIGH);}else{digitalWrite(Visual_Error_Indicator, LOW);}                  // Set strobe error indicator.
}


// [MODE] => "COUNT"
if(Mode=="COUNT"){                                                                  // Stats mode in operation.
if(Override_Flag){Mode = "OVERRIDE"; Mode_Restart();}                               // Go to override if switched.
General_Serial_Output();                                                            // Provide 3rd party feedback via serial (if requried).
if(General_Alarm){General_Alarm_Count = General_Alarm_Count + 1;}                   // Increment general alarm stats.
if(!General_Alarm){General_Pallet_Count=General_Pallet_Count + 1;}                  // Increment good pallet stats.
Mode = "RESETTING"; Mode_Restart();                                                 // Switch mode once complete.
}


// [MODE] => "RESETTING"
if(Mode=="RESETTING"){                                                              // Reset mode in operation.
if(Override_Flag){Mode = "OVERRIDE"; Mode_Restart();}                               // Go to override if switched.
General_Serial_Output();                                                            // Provide 3rd party feedback via serial (if requried).
Stock_Overhang_Alarm = false;                                                       // Reset sub alarm.
Runner_Alarm = false;                                                               // Reset sub alarm.
Slip_Sheet_Alarm = false;                                                           // Reset sub alarm.
General_Alarm = false;                                                              // Reset general alarm.
digitalWrite(LED_BUILTIN, LOW);                                                     // Reset on-board LED warning indicator.
digitalWrite(Visual_Error_Indicator, LOW);                                          // Reset strobe warning indicator.
lcd.setCursor(0, 1);                                                                // Set LCD cursor.
lcd.print(Progress_Bar(Restart_Delay, Mode_Time_Indexer));                          // Display progress bar string.
Mode_Time_Indexer = Mode_Time_Indexer + 1;                                          // Increment mode counter.
if(Mode_Time_Indexer > Restart_Delay){Serial.println("Completed");Serial.println();Mode = "WAITING"; Mode_Restart();}           // Switch mode once complete.
}


// [MODE] => "OVERRIDE"
if(Mode=="OVERRIDE"){                                                               // Override mode in operation.
lcd.setCursor(0, 1);                                                                // Set LCD cursor.
lcd.print("E#: ");                                                                  // Display sub heading.
lcd.setCursor(3, 1);                                                                // Set LCD cursor.
lcd.print(General_Alarm_Count);                                                     // Display value.
lcd.setCursor(8, 1);                                                                // Set LCD cursor.
lcd.print("G#: ");                                                                  // Display sub heading.
lcd.setCursor(11, 1);                                                               // Set LCD cursor.
lcd.print(General_Pallet_Count);                                                    // Display value.
if(!Override_Flag){Mode = "START-UP"; Mode_Restart();}                              // Switch mode once complete.
}

delay(25);                                                                          // Base cycle throttle. [Do not remove!]
}



// ///////////////////////////////////////////////////////////////// FUNCTION(S)

// [FUNCTION] => OBTAING DISTANCE FROM ULTRASONIC SENSORS (RAW PULSES)
long Scan_Sensor(int Sensor_Trigger, int Sensor_Echo){
long Sensor_Duration = 0;                                                           // Value retrieved from sensor.
long Sensor_mm = 0;                                                                 // Converted measurement.
digitalWrite(Sensor_Trigger, LOW);                                                  // Set output LOW.
delayMicroseconds(5);                                                               // Wait clearance time.
digitalWrite(Sensor_Trigger, HIGH);                                                 // Set output HIGH.
delayMicroseconds(10);                                                              // Sending input trigger.
digitalWrite(Sensor_Trigger, LOW);                                                  // Set output LOW.
pinMode(Sensor_Echo, INPUT);
Sensor_Duration =  pulseIn(Sensor_Echo, HIGH);                                      // Retrieve HCSR04 feedback.
Sensor_mm = (Sensor_Duration/2)*0.303;                                              // Convert to mm.
return Sensor_mm;                                                                   // Return value.
}


// [FUNCTION] => Detect Pallet.
bool Detect_Pallet(long Measurement, int Set_Distance, int Error){
bool Output_String = false;
if(Measurement>(Set_Distance-Error)&&Measurement<(Set_Distance+Error)){Output_String = true;}
  return Output_String;   
}

// [FUNCTION] => Detect Runner.
bool Detect_Runner(long Measurement, int Set_Distance, int Error){
bool Output_String = false;
if(Measurement>(Set_Distance-Error)&&Measurement<(Set_Distance+Error)){Output_String = true;}
  return Output_String;   
}

// [FUNCTION] => Detect Slip Sheet.
bool Detect_Slip(long Measurement, int Set_Distance, int Error){
bool Output_String = false;
if(Measurement>(Set_Distance-Error)&&Measurement<(Set_Distance+Error)){Output_String = true;}
  return Output_String;   
}

// [FUNCTION] => Detect Stock Overhang.
bool Detect_Stock_Overhang(long Measurement, int Set_Distance, int Allowable, int Error){
bool Output_String = false;
if(Measurement<(Set_Distance-Allowable-Error)){Output_String = true;}
  return Output_String;   
}

// [FUNCTION] => Progress Bar Text Creation.
String Progress_Bar(int Set_Value, int Counter){
String Output_String = "";
long Available_Segments = 16;
float Real_Progress_Ratio = float(Counter) / float(Set_Value);
int Artificial_Progress = int(Real_Progress_Ratio*Available_Segments);
int i;
for (i = 0; i < Artificial_Progress; i++){
Output_String += ".";  
}
return Output_String;  
}


// [FUNCTION] => Determine if value is within margin.
bool Within_Margin(int Measurement, int Set_Distance, int Margin){
bool Output_String = false;
if(Measurement>(Set_Distance-Margin)&&Measurement<(Set_Distance+Margin)){
Output_String = true;  
}
  return Output_String; 
}


// [FUNCTION] => General Serial Output
void General_Serial_Output(){
Serial.print(":");
Serial.print(A1_Pallet_Detected);
Serial.print(":");
Serial.print(C1_Runner_Detected);
Serial.print(":");
Serial.print(Runner_Alarm);
Serial.print(":");
Serial.println(General_Alarm);
}

// [FUNCTION] => Mode Restart
void Mode_Restart(){
Mode_Time_Indexer = 0; // Reset mode indexer.
C1_Runner_Detected_Memory = false;
C2_Runner_Detected_Memory = false;
A1_Pallet_Checked = false;
A2_Pallet_Checked = false;
A1_Pallet_Error_Indexer = false;
A2_Pallet_Error_Indexer = false;
C1_Runner_Checked = false;
C2_Runner_Checked = false;
C1_Runner_Count_Indexer = 0;
//C2_Runner_Counter = 0;
C1_Runner_Error_Indexer = 0;
C2_Runner_Error_Indexer = 0;
//Runner_Time_Out_Counter = 0;
C1_Last_Runner = false;
//General_Runner_Detected_Memory = false;
//General_Runner_End = false;
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Mode: ");
lcd.setCursor(6, 0);
lcd.print(Mode);
Serial.print("Mode: " );
Serial.println(Mode);
}





