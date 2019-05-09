



#include <LiquidCrystal.h>



// USER DEFINABLE VARIABLES

int Startup_Delay = 20;
int Pallet_Zone_2_Delay = 10;
int Pallet_Zone_6_Delay = 20;
int Report_Hold = 20;
int Restart_Delay = 20;


int Pallet_Distance = 100;
int Pallet_Distance_Margin = 10;

int Slip_Sheet_Height = 100;
int Slip_Sheet_Height_Margin = 10;

int Runner_Height = 100;
int Runner_Height_Margin = 10;



int Allowable_Stock_Margin_LR = 10; // mm
int Allowable_Stock_Margin_FR = 10; // cycles


int Slip_Sheet_Distance = 100;
int Slip_Sheet_Distance_Margin = 10;
int Allowable_Slip_Sheet_Margin_FR = 10; // cycles



// SYSTEM VARIABLES

const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



//bool Front_Runner_Error = false;
//bool Front_Stock_Over_Hang_Error = false;
//bool Front_Slip_Sheet_Over_Hang_Error = false;
//
//bool Rear_Runner_Error = false;
//bool Rear_Stock_Over_Hang_Error = false;
//bool Rear_Slip_Sheet_Over_Hang_Error = false;
//
//bool Left_Stock_Over_Hang_Error = false;
//bool Left_Slip_Sheet_Over_Hang_Error = false;
//
//
//bool Right_Stock_Over_Hang_Error = false;
//bool Right_Slip_Sheet_Over_Hang_Error = false;


int LCD_Inputs = A0;


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



int trigPinB = 32;    // Trigger
int echoPinB = 33;    // Echo

String Mode = "Start-up";
int Mode_Time_Counter = 0;

int Allowable_Stock_Margin_FR_Counter = 0;
int Allowable_Slip_Sheet_Margin_FR_Counter = 0;


bool Minor_Setup_Flag = false;
bool Override_Flag = false;
bool General_Error_Flag = false;

long Sensor_Duration_A1, Sensor_Duration_B1, Sensor_Duration_C1, Sensor_Duration_A2, Sensor_Duration_B2, Sensor_Duration_C3;
long Sensor_mm_A1, Sensor_mm_B1, Sensor_mm_C1, Sensor_mm_A2, Sensor_mm_B2, Sensor_mm_C2;






void setup() {

Serial.begin (9600);

pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, LOW);
digitalWrite(Visual_Error_Indicator, LOW);


lcd.begin(16, 2);
lcd.print("hello, world!");


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

lcd.clear();
  
}

void loop() {

Override_Flag = digitalRead(Override_Switch);
//Serial.print(digitalRead(Override_Switch));

//if(analogRead(LCD_Inputs)>620&&analogRead(LCD_Inputs)<660){Override_Flag=true;}else{Override_Flag=false;}



if(Mode=="Start-up"){
if(Minor_Setup_Flag==false){Serial.print("System Start-up");}Minor_Setup_Flag = true;
if(Override_Flag){Mode = "Override"; Mode_Restart();}

lcd.setCursor(0, 1);
lcd.print(Progress_Bar(Startup_Delay, Mode_Time_Counter));
Mode_Time_Counter = Mode_Time_Counter + 1;

Serial.print(".");

if(Mode_Time_Counter > Startup_Delay){Serial.println("Completed");Mode = "Waiting"; Mode_Restart();}
}





if(Mode=="Waiting"){
if(Override_Flag){Mode = "Override"; Mode_Restart();}
if(Minor_Setup_Flag==false){Serial.print("Waiting for Pallet Start");}Minor_Setup_Flag = true;
Sensor_mm_A1 = Scan_Sensor(Sensor_Trigger_A1, Sensor_Echo_A1);
Sensor_mm_A2 = Scan_Sensor(Sensor_Trigger_A2, Sensor_Echo_A2);
Sensor_mm_B1 = Scan_Sensor(Sensor_Trigger_B1, Sensor_Echo_B1);
//Sensor_mm_B2 = Scan_Sensor(Sensor_Trigger_B2, Sensor_Echo_B2);
Sensor_mm_C1 = Scan_Sensor(Sensor_Trigger_C1, Sensor_Echo_C1);
//Sensor_mm_C2 = Scan_Sensor(Sensor_Trigger_C2, Sensor_Echo_C2);

lcd.clear();
lcd.setCursor(0, 1);lcd.print("A1:");
lcd.setCursor(3, 1);lcd.print(Sensor_mm_A1);
lcd.setCursor(8, 1);lcd.print("A2:");
lcd.setCursor(11, 1);lcd.print(Sensor_mm_A2);

//if(int(Sensor_mm_A1)<100){Mode = "Pallet"; Mode_Restart();}

Serial.print(".");
//Serial.println(Sensor_mm_B1);

if(Within_Margin(int(Sensor_mm_A1), Pallet_Distance, Pallet_Distance_Margin)){Serial.println("(A1) Triggered");Mode = "Pallet (1)"; Mode_Restart();}
if(Within_Margin(int(Sensor_mm_A2), Pallet_Distance, Pallet_Distance_Margin)){Serial.println("(A2) Triggered");Mode = "Pallet (1)"; Mode_Restart();}

if(Within_Margin(int(Sensor_mm_B1), Pallet_Distance, Pallet_Distance_Margin)){Allowable_Stock_Margin_FR_Counter = Allowable_Stock_Margin_FR_Counter + 1;}
if(Within_Margin(int(Sensor_mm_B2), Pallet_Distance, Pallet_Distance_Margin)){Allowable_Stock_Margin_FR_Counter = Allowable_Stock_Margin_FR_Counter + 1;}
if(Allowable_Stock_Margin_FR_Counter>Allowable_Stock_Margin_FR){Serial.println("Stock Overhang Error");General_Error_Flag = true;}

if(Within_Margin(int(Sensor_mm_C1), Slip_Sheet_Distance, Slip_Sheet_Distance_Margin)){Allowable_Slip_Sheet_Margin_FR_Counter = Allowable_Slip_Sheet_Margin_FR_Counter + 1;}
if(Within_Margin(int(Sensor_mm_C2), Slip_Sheet_Distance, Slip_Sheet_Distance_Margin)){Allowable_Slip_Sheet_Margin_FR_Counter = Allowable_Slip_Sheet_Margin_FR_Counter + 1;}
if(Allowable_Slip_Sheet_Margin_FR_Counter>Allowable_Slip_Sheet_Margin_FR){Serial.println("Slip Sheet Overhang Error");General_Error_Flag = true;}


}



if(Mode=="Pallet (1)"){
  if(Override_Flag){Mode = "Override"; Mode_Restart();}
  Minor_Setup_Flag = true;
// X Sensor_mm_A1 = Scan_Sensor(Sensor_Trigger_A1, Sensor_Echo_A1);
//Sensor_mm_B1 = Scan_Sensor(Sensor_Trigger_B1, Sensor_Echo_B1); // TEMP


Sensor_mm_C1 = Scan_Sensor(Sensor_Trigger_C1, Sensor_Echo_C1); // TO ADD AFTER
Sensor_mm_C2 = Scan_Sensor(Sensor_Trigger_C2, Sensor_Echo_C2); // TO ADD AFTER


lcd.clear();
lcd.setCursor(0, 1);
lcd.print(Sensor_mm_B1);

Serial.print("Check Runner (C1)...");
if(Within_Margin(int(Sensor_mm_C1), Slip_Sheet_Height, Slip_Sheet_Height_Margin)){Serial.println("Pass");}else{Serial.println("Error");General_Error_Flag = true;} // NEED TO ADD FAULT FLAG

Serial.print("Check Runner (C2)...");
if(Within_Margin(int(Sensor_mm_C2), Slip_Sheet_Height, Slip_Sheet_Height_Margin)){Serial.println("Pass");}else{Serial.println("Error");General_Error_Flag = true;} // NEED TO ADD FAULT FLAG


Mode = "Pallet (2)"; Mode_Restart();

}





if(Mode=="Pallet (2)"){
if(Override_Flag){Mode = "Override"; Mode_Restart();}
if(Minor_Setup_Flag==false){Serial.print("Waiting for Middle");}Minor_Setup_Flag = true;
lcd.setCursor(0, 1);
lcd.print(Progress_Bar(Pallet_Zone_2_Delay, Mode_Time_Counter));
Serial.print(".");
Mode_Time_Counter = Mode_Time_Counter + 1;
if(Mode_Time_Counter > Pallet_Zone_2_Delay){Serial.println("Completed");Mode = "Pallet (3)"; Mode_Restart();}

}






if(Mode=="Pallet (3)"){
if(Override_Flag){Mode = "Override"; Mode_Restart();}
Minor_Setup_Flag = true;
Sensor_mm_B1 = Scan_Sensor(Sensor_Trigger_B1, Sensor_Echo_B1);
Sensor_mm_B2 = Scan_Sensor(Sensor_Trigger_B1, Sensor_Echo_B2);

Serial.print("Check Stock Pos Right (B1)...");
if(Within_Margin(int(Sensor_mm_B1), Slip_Sheet_Height, Slip_Sheet_Height_Margin)){Serial.println("Pass");}else{Serial.println("Error");General_Error_Flag = true;} // NEED TO ADD FAULT FLAG

Serial.print("Check Stock Pos Right (B2)...");
if(Within_Margin(int(Sensor_mm_B2), Slip_Sheet_Height, Slip_Sheet_Height_Margin)){Serial.println("Pass");}else{Serial.println("Error");General_Error_Flag = true;} // NEED TO ADD FAULT FLAG



Mode = "Pallet (4)"; Mode_Restart();

}



if(Mode=="Pallet (4)"){


if(Override_Flag){Mode = "Override"; Mode_Restart();}
if(Minor_Setup_Flag==false){Serial.print("Waiting for Pallet End");}Minor_Setup_Flag = true;

Sensor_mm_A1 = Scan_Sensor(Sensor_Trigger_A1, Sensor_Echo_A1);
Sensor_mm_A2 = Scan_Sensor(Sensor_Trigger_A2, Sensor_Echo_A2);
lcd.setCursor(0, 1);
lcd.print(Sensor_mm_A1);

Serial.print(".");
if(!Within_Margin(int(Sensor_mm_A1), Pallet_Distance, Allowable_Stock_Margin_LR)){Serial.println("(A1) Triggered");Mode = "Pallet (5)"; Mode_Restart();}
if(!Within_Margin(int(Sensor_mm_A2), Pallet_Distance, Allowable_Stock_Margin_LR)){Serial.println("(A2) Triggered");Mode = "Pallet (5)"; Mode_Restart();}




}


if(Mode=="Pallet (5)"){
if(Override_Flag){Mode = "Override"; Mode_Restart();}
Minor_Setup_Flag = true;
Sensor_mm_C1 = Scan_Sensor(Sensor_Trigger_C1, Sensor_Echo_C1); // TO ADD AFTER
Sensor_mm_C2 = Scan_Sensor(Sensor_Trigger_C2, Sensor_Echo_C2); // TO ADD AFTER

lcd.clear();
lcd.setCursor(0, 1);
lcd.print(Sensor_mm_B1);

Serial.print("Check Runner (C1)...");
if(Within_Margin(int(Sensor_mm_C1), Slip_Sheet_Height, Slip_Sheet_Height_Margin)){Serial.println("Pass");}else{Serial.println("Error");General_Error_Flag = true;} // NEED TO ADD FAULT FLAG

Serial.print("Check Runner (C2)...");
if(Within_Margin(int(Sensor_mm_C2), Slip_Sheet_Height, Slip_Sheet_Height_Margin)){Serial.println("Pass");}else{Serial.println("Error");General_Error_Flag = true;} // NEED TO ADD FAULT FLAG


Mode = "Pallet (6)"; Mode_Restart();

}








if(Mode=="Pallet (6)"){
if(Override_Flag){Mode = "Override"; Mode_Restart();}
if(Minor_Setup_Flag==false){Serial.print("Monitor End");}Minor_Setup_Flag = true;
lcd.setCursor(0, 1);
lcd.print(Progress_Bar(Pallet_Zone_2_Delay, Mode_Time_Counter));
Serial.print(".");

if(Within_Margin(int(Sensor_mm_A1), Pallet_Distance, Pallet_Distance_Margin)){Serial.println("(A1) Triggered");Mode = "Pallet (1)"; Mode_Restart();}
if(Within_Margin(int(Sensor_mm_A2), Pallet_Distance, Pallet_Distance_Margin)){Serial.println("(A2) Triggered");Mode = "Pallet (1)"; Mode_Restart();}

//if(Within_Margin(int(Sensor_mm_B1), Pallet_Distance, Pallet_Distance_Margin)){Allowable_Stock_Margin_FR_Counter = Allowable_Stock_Margin_FR_Counter + 1;}
//if(Within_Margin(int(Sensor_mm_B2), Pallet_Distance, Pallet_Distance_Margin)){Allowable_Stock_Margin_FR_Counter = Allowable_Stock_Margin_FR_Counter + 1;}
if(Allowable_Stock_Margin_FR_Counter>Allowable_Stock_Margin_FR){Serial.println("Stock Overhang Error");General_Error_Flag = true;}

if(Within_Margin(int(Sensor_mm_B1), Slip_Sheet_Distance, Slip_Sheet_Distance_Margin)){Allowable_Slip_Sheet_Margin_FR_Counter = Allowable_Slip_Sheet_Margin_FR_Counter + 1;}
if(Within_Margin(int(Sensor_mm_C2), Slip_Sheet_Distance, Slip_Sheet_Distance_Margin)){Allowable_Slip_Sheet_Margin_FR_Counter = Allowable_Slip_Sheet_Margin_FR_Counter + 1;}
if(Allowable_Slip_Sheet_Margin_FR_Counter>Allowable_Slip_Sheet_Margin_FR){Serial.println("Slip Sheet Overhang Error");General_Error_Flag = true;}



Mode_Time_Counter = Mode_Time_Counter + 1;
if(Mode_Time_Counter > Pallet_Zone_6_Delay){Serial.println("Completed");Mode = "Report"; Mode_Restart();}

}













if(Mode=="Report"){
if(Override_Flag){Mode = "Override"; Mode_Restart();}
if(Minor_Setup_Flag==false){Serial.print("Sending External Signal");}Minor_Setup_Flag = true;



lcd.setCursor(0, 1);
lcd.print(Progress_Bar(Report_Hold, Mode_Time_Counter));
Serial.print(".");
Mode_Time_Counter = Mode_Time_Counter + 1;
if(Mode_Time_Counter > Report_Hold){Serial.println("Completed");Mode = "R.Delay"; Mode_Restart();General_Error_Flag = false;}

if(General_Error_Flag){digitalWrite(LED_BUILTIN, HIGH);}else{digitalWrite(LED_BUILTIN, LOW);}
if(General_Error_Flag){digitalWrite(Visual_Error_Indicator, HIGH);}else{digitalWrite(Visual_Error_Indicator, LOW);}


}




if(Mode=="R.Delay"){

if(Override_Flag){Mode = "Override"; Mode_Restart();}
if(Minor_Setup_Flag==false){Serial.print("Retrigger Delay");}Minor_Setup_Flag = true;
lcd.setCursor(0, 1);
lcd.print(Progress_Bar(Restart_Delay, Mode_Time_Counter));
Serial.print(".");
Mode_Time_Counter = Mode_Time_Counter + 1;
if(Mode_Time_Counter > Restart_Delay){Serial.println("Completed");Mode = "Waiting"; Mode_Restart();}



}



if(Mode=="Override"){
  
if(Minor_Setup_Flag==false){Serial.println();Serial.println("System in Override");}Minor_Setup_Flag = true;
if(!Override_Flag){Mode = "Start-up"; Mode_Restart();}

}


if(Mode=="test"){


// OBTAIN SENSOR DATA
Sensor_Duration_A1 = Scan_Sensor(Sensor_Trigger_A1, Sensor_Echo_A1);
Sensor_Duration_B1 = Scan_Sensor(Sensor_Trigger_B1, Sensor_Echo_B1);

Sensor_mm_A1 = (Sensor_Duration_A1/2)*0.303;
//test= duration;
Sensor_mm_B1 = (Sensor_Duration_B1/2)*0.303;

Serial.print("Sensor A: ");
Serial.print(Sensor_mm_A1);
Serial.print("; Sensor B: ");
Serial.print(Sensor_mm_B1);

Serial.println();

lcd.clear();

lcd.setCursor(0, 0);
lcd.print("Mode: "+Mode);

lcd.setCursor(0, 1);
lcd.print(int(Sensor_mm_A1));

lcd.setCursor(6, 1);
lcd.print(int(Sensor_mm_B1));
//lcd.print(millis() / 1000);

}

  

lcd.setCursor(0, 0);
lcd.print("Mode: " + Mode);

delay(250);
//delay(500);
}


// FUNCTION FOR OBTAING DISTANCE FROM ULTRASONIC SENSORS (RAW PULSES)
long Scan_Sensor(int Sensor_Trigger, int Sensor_Echo){
long Sensor_Duration = 0;
long Sensor_mm = 0;
digitalWrite(Sensor_Trigger, LOW);
delayMicroseconds(5);  
digitalWrite(Sensor_Trigger, HIGH);
delayMicroseconds(10);  
digitalWrite(Sensor_Trigger, LOW);
pinMode(Sensor_Echo, INPUT);
Sensor_Duration =  pulseIn(Sensor_Echo, HIGH);
Sensor_mm = (Sensor_Duration/2)*0.303;
return Sensor_mm;
}


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



bool Within_Margin(int Measurement, int Set_Distance, int Margin){
bool Output_String = false;

if(Measurement>(Set_Distance-Margin)&&Measurement<(Set_Distance+Margin)){
Output_String = true;
  
}

  return Output_String; 
}




void Mode_Restart(){
lcd.clear(); 
Mode_Time_Counter = 0;
Allowable_Slip_Sheet_Margin_FR_Counter = 0;
Allowable_Stock_Margin_FR_Counter = 0;

  Minor_Setup_Flag = false;
}





