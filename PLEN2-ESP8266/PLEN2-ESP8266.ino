#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_PWMServoDriver.h>
#include <EEPROM.h>
#include <FS.h>
#include "configuration.h"
#if SSD1306
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif
#ifndef WITH_SPIFFS
#include "html.h"
#endif
// Version
String FW_Version = "PLEN2 MG90S Firmware Version 0.1";

#if SSD1306
//  I2C Address 0x3C
#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(-1);
// OLED Display Index
int OLED_Display_Index = 0;
#endif

// I2C Address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servos Matrix
//const int MATRIX_SIZE = 21;    // 0 ~ 15 Servo + GPIO12 + GPIO13 + GPIO14 + GPIO16 + Run Time

#define DURATION_INDEX 		20
// could be 18 or 20
#define ALLSERVOS	18
#include "poses21.h"

// TODO set values for MG90S servos
const int PWM_Frequency = 53;    // PWM frequency 50Hz
const int PWMRES_Min = 1;        // PWM Resolution 1
const int PWMRES_Max = 160;      // PWM Resolution 160
const int SERVOMIN = 145;        // 145
const int SERVOMAX = 472;        // 472

// Servo Delay Base Time
const int BASEDELAYTIME = 10;    // 10ms


// AP Password
const char WiFiAPPSK[] = "12345678";

// Servo PWM Frequency
int Set_PWM_Freq = PWM_Frequency;

// Motion Data Index
int Servo_PROGRAM;
int Servo_PROGRAM_Stack;

// Backup Servo Value
int Running_Servo_POS[ALLSERVOS];

// Engineering model
int Engineering_Model;
// Start Web Server
ESP8266WebServer server(80);
// Start GPIO Software Servo
Servo servo12;  
Servo servo13;
Servo servo14;
Servo servo16;


/*============================================================================*/
void Set_PWM_to_Servo(int iServo, int iValue) {
  int NewPWM = map(iValue, PWMRES_Min, PWMRES_Max, SERVOMIN, SERVOMAX);
#if DEBUG
  Serial.print("Set_PWM_to_Servo = ");
  Serial.print(iServo);
  Serial.print(" , iValue = ");
  Serial.print(iValue);
  Serial.print(" , NewPWM = ");
  Serial.println(NewPWM);
#endif
  if (iServo >= 19) {
    servo16.write(iValue);
  } else {
    if (iServo >= 18) {
      servo14.write(iValue);
    } else {
      if (iServo >= 17) {
        servo13.write(iValue);
      } else {
        if (iServo >= 16) {
          servo12.write(iValue);
        } else {
          pwm.setPWM(iServo, 0, NewPWM);
        }
      }
    }
  }
}


/*============================================================================*/
void Servo_PROGRAM_Zero() {
  for ( unsigned int i = 0; i < ALLSERVOS; i++) {
    Running_Servo_POS[i] = Servo_Act_0[i] + (int8_t)EEPROM.read(i);
  }
  Running_Servo_POS[DURATION_INDEX] = Servo_Act_0[DURATION_INDEX] + (int8_t)EEPROM.read(DURATION_INDEX);
  

  for (unsigned int i = 0; i < ALLSERVOS; i++) {
    Set_PWM_to_Servo(i, Running_Servo_POS[i]);
    delay(10);
  }
}


/*============================================================================*/
void Servo_PROGRAM_Center() {
  for ( int Index = 0; Index < ALLSERVOS; Index++) {
    Running_Servo_POS[Index] = Servo_Act_1[Index] + (int8_t)EEPROM.read(Index);
  }
  Running_Servo_POS[DURATION_INDEX] = Servo_Act_1[DURATION_INDEX] + (int8_t)EEPROM.read(DURATION_INDEX);

  for (int iServo = 0; iServo < ALLSERVOS; iServo++) {
    Set_PWM_to_Servo(iServo, Running_Servo_POS[iServo]);
    delay(10);
  }
}


/*============================================================================*/
void Servo_PROGRAM_Run(const int iMatrix[][DURATION_INDEX+1],  int iSteps) {
  int INT_TEMP_A, INT_TEMP_B, INT_TEMP_C;

  for ( int MainLoopIndex = 0; MainLoopIndex < iSteps; MainLoopIndex++) {
    int InterTotalTime = iMatrix [ MainLoopIndex ] [DURATION_INDEX] + (int8_t)EEPROM.read(DURATION_INDEX);
    int InterDelayCounter = InterTotalTime / BASEDELAYTIME;

    for ( int InterStepLoop = 0; InterStepLoop < InterDelayCounter; InterStepLoop++) {

      for (int ServoIndex = 0; ServoIndex < ALLSERVOS; ServoIndex++) {

        INT_TEMP_A = Running_Servo_POS[ServoIndex];                                                    
        INT_TEMP_B = iMatrix[MainLoopIndex][ServoIndex] + (int8_t)EEPROM.read(ServoIndex);    

        if (INT_TEMP_A == INT_TEMP_B) {
          INT_TEMP_C = INT_TEMP_B;
        } else {
          if (INT_TEMP_A > INT_TEMP_B) {
            INT_TEMP_C =  map(BASEDELAYTIME * InterStepLoop, 0, InterTotalTime, 0, INT_TEMP_A - INT_TEMP_B);

            if (INT_TEMP_A - INT_TEMP_C >= INT_TEMP_B) {
              Set_PWM_to_Servo(ServoIndex, INT_TEMP_A - INT_TEMP_C);
            }
          } else {
            if (INT_TEMP_A < INT_TEMP_B) {
              INT_TEMP_C =  map(BASEDELAYTIME * InterStepLoop, 0, InterTotalTime, 0, INT_TEMP_B - INT_TEMP_A);
              if (INT_TEMP_A + INT_TEMP_C <= INT_TEMP_B) {
                Set_PWM_to_Servo(ServoIndex, INT_TEMP_A + INT_TEMP_C);
              }
            }
          }
        }
      }
      delay(BASEDELAYTIME);
    }

    for ( int Index = 0; Index < ALLSERVOS; Index++) {
      Running_Servo_POS[Index] = iMatrix[MainLoopIndex][Index] + (int8_t)EEPROM.read(Index);
    }
      Running_Servo_POS[DURATION_INDEX] = iMatrix[MainLoopIndex][DURATION_INDEX] + (int8_t)EEPROM.read(DURATION_INDEX);
  }
}

void Get_PWM_Frequency() {
  int iCab_Freq =  (int8_t)EEPROM.read(ALLSERVOS + 1);

  if (iCab_Freq > 120 || iCab_Freq < -120) {
    EEPROM.write(ALLSERVOS + 1, 0);
    EEPROM.commit();
    iCab_Freq = 0;
  }

  Set_PWM_Freq = PWM_Frequency + iCab_Freq;
}

void writeKeyValue(int8_t key, int8_t value) {
  EEPROM.write(key, value);
  EEPROM.commit();
}

int8_t readKeyValue(int8_t key) {
  int8_t value = EEPROM.read(key);
  Serial.print("read key (");
  Serial.print(key);
  Serial.print(") : ");
  Serial.println(value);
  return value;
}

void handleSave() {
  String key = server.arg("key");
  String value = server.arg("value");

  int8_t keyInt = key.toInt();
  int8_t valueInt = value.toInt();

  // Software PWM PIN detach
  servo12.detach();
  servo13.detach();
  servo14.detach();
  servo16.detach();
  delay(50);

  if (keyInt == 100) {
    writeKeyValue(0, 0);
    writeKeyValue(1, 0);
    writeKeyValue(2, 0);
    writeKeyValue(3, 0);
    writeKeyValue(4, 0);
    writeKeyValue(5, 0);
    writeKeyValue(6, 0);
    writeKeyValue(7, 0);
    writeKeyValue(8, 0);
    writeKeyValue(9, 0);
    writeKeyValue(10, 0);
    writeKeyValue(11, 0);
    writeKeyValue(12, 0);
    writeKeyValue(13, 0);
    writeKeyValue(14, 0);
    writeKeyValue(15, 0);
    writeKeyValue(16, 0);
    writeKeyValue(17, 0);
    writeKeyValue(18, 0);
    writeKeyValue(19, 0);
    writeKeyValue(20, 0);
    //writeKeyValue(21, 0); // Set_PWM_Freq 不清除
  } else {
    if (valueInt >= -125 && valueInt <= 125) {
      writeKeyValue(keyInt, valueInt);

      if (keyInt == 21) {
        Set_PWM_Freq = PWM_Frequency + valueInt;
        pwm.setPWMFreq(Set_PWM_Freq);
      }
    }
  }

  servo12.attach(12);
  servo13.attach(13);
  servo14.attach(14);
  servo16.attach(16);
  delay(10);

  server.send(200, "text/html", "(key, value)=(" + key + "," + value + ")");
}

void handleController() {
  String pm = server.arg("pm");
  String pms = server.arg("pms");
  String gpid = server.arg("gpid");
  String servo = server.arg("servo");

  if (pm != "") {
    Servo_PROGRAM = pm.toInt();
  }

  if (pms != "") {
    Servo_PROGRAM_Stack = pms.toInt();
  }

  if (servo != "") {
    int Servo_ID = servo.toInt();
    String ival = server.arg("value");
    int Servo_PWM = ival.toInt() + (int8_t)EEPROM.read(Servo_ID);
    int pulselength = map(Servo_PWM, PWMRES_Min, PWMRES_Max, SERVOMIN, SERVOMAX);
#if DEBUG
    Serial.print("Servo ID = ");
    Serial.print(Servo_ID);
    Serial.print(", PWM = ");
    Serial.println(pulselength);
#endif
    pwm.setPWM(Servo_ID, 0, pulselength);
  }

  if (gpid != "") {
    String ival = server.arg("value");
	int gpioVal;
	
	switch (gpid.toInt()) {
		case 12 :
			gpioVal = ival.toInt() + (int8_t)EEPROM.read(16);
			servo12.write(gpioVal);
			break;
		case 13 :
			gpioVal = ival.toInt() + (int8_t)EEPROM.read(17);
			servo13.write(gpioVal);
			break;
		case 14 :
			gpioVal = ival.toInt() + (int8_t)EEPROM.read(18);
			servo14.write(gpioVal);
			break;
		case 16 :
			gpioVal = ival.toInt() + (int8_t)EEPROM.read(19);
			servo16.write(gpioVal);
			break;
		default:
			break;
	}
  }

  server.send(200, "text/html", "(pm, pms)=(" + pm + "," + pms + ")");
}

void handleOnLine() {
  String m0 = server.arg("m0");
  String m1 = server.arg("m1");
  String m2 = server.arg("m2");
  String m3 = server.arg("m3");
  String m4 = server.arg("m4");
  String m5 = server.arg("m5");
  String m6 = server.arg("m6");
  String m7 = server.arg("m7");
  String m8 = server.arg("m8");
  String m9 = server.arg("m9");
  String m10 = server.arg("m10");
  String m11 = server.arg("m11");
  String m12 = server.arg("m12");
  String m13 = server.arg("m13");
  String m14 = server.arg("m14");
  String m15 = server.arg("m15");
  String g12 = server.arg("g12");
  String g13 = server.arg("g13");
  String g14 = server.arg("g14");
  String g16 = server.arg("g16");
  String t1 = server.arg("t1");

  int Servo_Prg_tmp [][DURATION_INDEX+1] = {
    //     0,             1,              2,              3,              4,              5,              6,              7,              8,              9,               10,              11,              12,              13,              14,              15,                  G12,           G13,           G14,           G16,        Time
    {m0.toInt(), m1.toInt(), m2.toInt(), m3.toInt(), m4.toInt(), m5.toInt(), m6.toInt(), m7.toInt(), m8.toInt(), m9.toInt(), m10.toInt(), m11.toInt(), m12.toInt(), m13.toInt(), m14.toInt(), m15.toInt(), g12.toInt(), g13.toInt(), g14.toInt(), g16.toInt(), t1.toInt()}
  };

  Servo_PROGRAM_Run(Servo_Prg_tmp, 1);

  server.send(200, "text/html", "(m0, m1)=(" + m0 + "," + m1 + ")");
}

#ifndef WITH_SPIFFS
void handleZero() {
  server.send(200, "text/html", ZERO_HTML);
}

void handleEditor() {
  server.send(200, "text/html", EDITOR_HTML);
}

void handleIndex() {
  server.send(200, "text/html", INDEX_HTML);
}

void handleSettings() {
  int servo22Val = readKeyValue(22);
  String servo22ValStr = String(servo22Val);

  int servo21Val = readKeyValue(21);
  String servo21ValStr = String(servo21Val);

  int servo20Val = readKeyValue(20);
  String servo20ValStr = String(servo20Val);

  int servo19Val = readKeyValue(19);
  String servo19ValStr = String(servo19Val);

  int servo18Val = readKeyValue(18);
  String servo18ValStr = String(servo18Val);

  int servo17Val = readKeyValue(17);
  String servo17ValStr = String(servo17Val);

  int servo16Val = readKeyValue(16);
  String servo16ValStr = String(servo16Val);

  int servo15Val = readKeyValue(15);
  String servo15ValStr = String(servo15Val);

  int servo14Val = readKeyValue(14);
  String servo14ValStr = String(servo14Val);

  int servo13Val = readKeyValue(13);
  String servo13ValStr = String(servo13Val);

  int servo12Val = readKeyValue(12);
  String servo12ValStr = String(servo12Val);

  int servo11Val = readKeyValue(11);
  String servo11ValStr = String(servo11Val);

  int servo10Val = readKeyValue(10);
  String servo10ValStr = String(servo10Val);

  int servo9Val = readKeyValue(9);
  String servo9ValStr = String(servo9Val);

  int servo8Val = readKeyValue(8);
  String servo8ValStr = String(servo8Val);

  int servo7Val = readKeyValue(7);
  String servo7ValStr = String(servo7Val);

  int servo6Val = readKeyValue(6);
  String servo6ValStr = String(servo6Val);

  int servo5Val = readKeyValue(5);
  String servo5ValStr = String(servo5Val);

  int servo4Val = readKeyValue(4);
  String servo4ValStr = String(servo4Val);

  int servo3Val = readKeyValue(3);
  String servo3ValStr = String(servo3Val);

  int servo2Val = readKeyValue(2);
  String servo2ValStr = String(servo2Val);

  int servo1Val = readKeyValue(1);
  String servo1ValStr = String(servo1Val);

  int servo0Val = readKeyValue(0);
  String servo0ValStr = String(servo0Val);

  String content = "";
  content += "<html>";
  content += "<head>";
  content += "  <title>MiniPlan Settings</title>";
  content += "  <style type=\"text/css\">";
  content += "  body {";
  content += "    color: white;";
  content += "    background-color: #000000 }";

  content += "  .pm_btn {";
  content += "  width: 120px;";
  content += "  -webkit-border-radius: 5;";
  content += "  -moz-border-radius: 5;";
  content += "  border-radius: 5px;";
  content += "  font-family: Arial;";
  content += "  color: #ffffff;";
  content += "  font-size: 24px;";
  content += "  background: #3498db;";
  content += "  padding: 10px 20px 10px 20px;";
  content += "  text-decoration: none;";
  content += "}";
  content += ".pm_text {";
  content += "width: 100px;";
  content += "-webkit-border-radius: 5;";
  content += "-moz-border-radius: 5;";
  content += "border-radius: 5px;";
  content += "font-family: Arial;";
  content += "font-size: 24px;";

  content += "padding: 10px 20px 10px 20px;";
  content += "text-decoration: none;";
  content += "}";

  content += ".pm_btn:hover {";
  content += "  background: #3cb0fd;";
  content += "  background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -o-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: linear-gradient(to bottom, #3cb0fd, #3498db);";
  content += "  text-decoration: none;";
  content += "}";

  content += ".pms_btn {";
  content += "width: 160px;";
  content += "-webkit-border-radius: 5;";
  content += "-moz-border-radius: 5;";
  content += "border-radius: 5px;";
  content += "font-family: Arial;";
  content += "color: #ffffff;";
  content += "font-size: 24px;";
  content += "background: #3498db;";
  content += "padding: 10px 20px 10px 20px;";
  content += "text-decoration: none;";
  content += "}";

  content += ".pms_btn:hover {";
  content += "background: #3cb0fd;";
  content += "background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -o-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: linear-gradient(to bottom, #3cb0fd, #3498db);";
  content += "text-decoration: none;";
  content += "}";
  content += "  </style>";
  content += "</head>";
  content += "<body>";

  content += "<table>";
  content += "<tr>";
  content += "<td>GPIO 16<br/><input class=\"pm_text\" type=\"text\" id=\"servo_19\" value=\"" + servo19ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(19,'servo_19')\">SET</button></td>";
  content += "<td>GPIO 14<br/><input class=\"pm_text\" type=\"text\" id=\"servo_18\" value=\"" + servo18ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(18,'servo_18')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>GPIO 13<br/><input class=\"pm_text\" type=\"text\" id=\"servo_17\" value=\"" + servo17ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(17,'servo_17')\">SET</button></td>";
  content += "<td>GPIO 12<br/><input class=\"pm_text\" type=\"text\" id=\"servo_16\" value=\"" + servo16ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(16,'servo_16')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 8<br/><input class=\"pm_text\" type=\"text\" id=\"servo_8\" value=\"" + servo8ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(8,'servo_8')\">SET</button></td>";
  content += "<td>Servo 7<br/><input class=\"pm_text\" type=\"text\" id=\"servo_7\" value=\"" + servo7ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(7,'servo_7')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 9<br/><input class=\"pm_text\" type=\"text\" id=\"servo_9\" value=\"" + servo9ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(9,'servo_9')\">SET</button></td>";
  content += "<td>Servo 6<br/><input class=\"pm_text\" type=\"text\" id=\"servo_6\" value=\"" + servo6ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(6,'servo_6')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 10<br/><input class=\"pm_text\" type=\"text\" id=\"servo_10\" value=\"" + servo10ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(10,'servo_10')\">SET</button></td>";
  content += "<td>Servo 5<br/><input class=\"pm_text\" type=\"text\" id=\"servo_5\" value=\"" + servo5ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(5,'servo_5')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 11<br/><input class=\"pm_text\" type=\"text\" id=\"servo_11\" value=\"" + servo11ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(11,'servo_11')\">SET</button></td>";
  content += "<td>Servo 4<br/><input class=\"pm_text\" type=\"text\" id=\"servo_4\" value=\"" + servo4ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(4,'servo_4')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 12<br/><input class=\"pm_text\" type=\"text\" id=\"servo_12\" value=\"" + servo12ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(12,'servo_12')\">SET</button></td>";
  content += "<td>Servo 3<br/><input class=\"pm_text\" type=\"text\" id=\"servo_3\" value=\"" + servo3ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(3,'servo_3')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 13<br/><input class=\"pm_text\" type=\"text\" id=\"servo_13\" value=\"" + servo13ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(13,'servo_13')\">SET</button></td>";
  content += "<td>Servo 2<br/><input class=\"pm_text\" type=\"text\" id=\"servo_2\" value=\"" + servo2ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(2,'servo_2')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 14<br/><input class=\"pm_text\" type=\"text\" id=\"servo_14\" value=\"" + servo14ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(14,'servo_14')\">SET</button></td>";
  content += "<td>Servo 1<br/><input class=\"pm_text\" type=\"text\" id=\"servo_1\" value=\"" + servo1ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(1,'servo_1')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 15<br/><input class=\"pm_text\" type=\"text\" id=\"servo_15\" value=\"" + servo15ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(15,'servo_15')\">SET</button></td>";
  content += "<td>Servo 0<br/><input class=\"pm_text\" type=\"text\" id=\"servo_0\" value=\"" + servo0ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(0,'servo_0')\">SET</button></td>";
  content += "</tr>";
  content += "</table>";

  content += "<br>";

  content += "<table>";
  content += "<tr>";
  content += "<td>PWM Frequency Calibration<br/><input class=\"pm_text\" type=\"text\" id=\"servo_21\" value=\"" + servo21ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(21,'servo_21')\">SET</button></td>";
  content += "<td>Voltage Calibration<br/><input class=\"pm_text\" type=\"text\" id=\"servo_22\" value=\"" + servo22ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(22,'servo_22')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Delay Time<br/><input class=\"pm_text\" type=\"text\" id=\"servo_20\" value=\"" + servo20ValStr + "\"><button class=\"pm_btn\" type=\"button\" onclick=\"saveServo(20,'servo_20')\">SET</button></td>";
  content += "</tr>";
  content += "</table>";

  content += "<br>";
  content += "<br>";

  content += "<table>";
  content += "<tr>";
  content += "<td><button class=\"pm_btn\" style=\"background: #ed3db5;\" type=\"button\" onclick=\"saveServo(100, 0)\">RESET</button></td>";
  content += "</tr>";
  content += "</table>";

  content += "<br>";

  content += "</body>";
  content += "<script>";

  content += "function saveServo(id, textId) {";
  content += "  var xhttp = new XMLHttpRequest();";
  content += "  var value = \"0\";";
  content += "  if(id==100){";
  //content += "    document.getElementById(\"servo_22\").value = \"0\";";
  //content += "    document.getElementById(\"servo_21\").value = \"0\";";
  content += "    document.getElementById(\"servo_20\").value = \"0\";";
  content += "    document.getElementById(\"servo_19\").value = \"0\";";
  content += "    document.getElementById(\"servo_18\").value = \"0\";";
  content += "    document.getElementById(\"servo_17\").value = \"0\";";
  content += "    document.getElementById(\"servo_16\").value = \"0\";";
  content += "    document.getElementById(\"servo_15\").value = \"0\";";
  content += "    document.getElementById(\"servo_14\").value = \"0\";";
  content += "    document.getElementById(\"servo_13\").value = \"0\";";
  content += "    document.getElementById(\"servo_12\").value = \"0\";";
  content += "    document.getElementById(\"servo_11\").value = \"0\";";
  content += "    document.getElementById(\"servo_10\").value = \"0\";";
  content += "    document.getElementById(\"servo_9\").value = \"0\";";
  content += "    document.getElementById(\"servo_8\").value = \"0\";";
  content += "    document.getElementById(\"servo_7\").value = \"0\";";
  content += "    document.getElementById(\"servo_6\").value = \"0\";";
  content += "    document.getElementById(\"servo_5\").value = \"0\";";
  content += "    document.getElementById(\"servo_4\").value = \"0\";";
  content += "    document.getElementById(\"servo_3\").value = \"0\";";
  content += "    document.getElementById(\"servo_2\").value = \"0\";";
  content += "    document.getElementById(\"servo_1\").value = \"0\";";
  content += "    document.getElementById(\"servo_0\").value = \"0\";";
  content += "  }else{";
  content += "    value = document.getElementById(textId).value;";
  content += "   }";
  content += "  xhttp.open(\"GET\",\"save?key=\"+id+\"&value=\"+value, true);";
  content += "  xhttp.send();";
  content += "}";

  content += "</script>";
  content += "</html>";
  server.send(200, "text/html", content);
}
#else
void sendSettings() {
  String json = "{";
  for (int i = 0 ; i < 21; i++) {
    int val = readKeyValue(i);
    Serial.println(val);
    json +=  "\"s" + String(i) + "\":" + String(val);
    if (i < 20) {
      json += ",";
    }
  }
  json += "}";
  Serial.println(json);
  server.send(200, "application/json", json);
  Serial.println("Send measures");
}
#endif


void enableWebServer() {
  HTTPMethod getMethod = HTTP_GET;
#ifdef WITH_SPIFFS
  server.serveStatic("/js", SPIFFS, "/js");
  server.on("/controller", getMethod, handleController);
  server.on("/save", getMethod, handleSave);
  server.serveStatic("/", SPIFFS, "/index.html");
  server.serveStatic("/editor", SPIFFS, "/editor.html");
  server.serveStatic("/zero", SPIFFS, "/zero.html");
  server.serveStatic("/settings", SPIFFS, "/settings.html");
  server.on("/online", getMethod, handleOnLine);
  server.on("/settings.json", sendSettings);
#else
  server.on("/controller", getMethod, handleController);
  server.on("/save", getMethod, handleSave);
  server.on("/", getMethod, handleIndex);
  server.on("/editor", getMethod, handleEditor);
  server.on("/zero", getMethod, handleZero);
  server.on("/setting", getMethod, handleSetting);
  server.on("/online", getMethod, handleOnLine);
#endif
  server.begin();
}

const char *ssid[MAX_SSID];
const char *password[MAX_SSID];


boolean connectWifi() {
  Serial.println("Connecting...");
  int status;
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
	int n = WiFi.scanNetworks();
  int maxQualityId = -1;
  int maxQuality = -255;
 	if (n != 0) {
		for (int i = 0; i < n; ++i) {
		  for (int t = 0; t < MAX_SSID; t++) {
		    if (WiFi.SSID(i) == String(ssid[t])) {
    			int quality=0;
    			if (WiFi.RSSI(i) <= -100) {
    				quality = 0;
    			} else {
    				if (WiFi.RSSI(i) >= -50) {
    					quality = 100;
    				} else {
    					quality = 2 * (WiFi.RSSI(i) + 100);
    				}
    			}
    			if (quality > maxQuality || maxQuality == -255) {
    			  maxQuality = quality;
    			  maxQualityId = t;
    			}
		    }
		  }
		}
		// try to connect to the max quality known network
		if (maxQualityId != -1) {
      Serial.print("Trying ");
      Serial.print(ssid[maxQualityId]);
      status = WiFi.begin(ssid[maxQualityId], password[maxQualityId]);
      int retries = 0;
      while (((status = WiFi.status()) != WL_CONNECTED) && (retries < 20)) {
        retries++;
        delay(1000);
        Serial.print(".");
      }
      if (status != WL_CONNECTED) {
        Serial.println(" failed");
        return false;
      } else {
        Serial.print(" connected : ");
        Serial.println(WiFi.localIP());
        return true;
      }
		}
 	}
  Serial.println("Wifi connection failed");
 	return false;
}

void setup(void) {
  Serial.begin(115200);
  Serial.println("MiniPlan V6 Start");
  // Engineering Model
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  delay(100);
  pinMode(12, INPUT);
  delay(100);
  Engineering_Model = digitalRead(12);
  if (Engineering_Model == 1) {
    Servo_PROGRAM = 100;
  }

  // Initialize I2C
  Wire.begin(4, 5);
#if SSD1306
  // Initialize I2C OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, OLED_Display_Index);
  display.println("MiniPlan V6 Start !");
  display.display();
#endif

#if 0
  // AP SSID Name
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();

  String AP_NameString = "PLEN2-ESP8266-" + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i = 0; i < AP_NameString.length(); i++) {
    AP_NameChar[i] = AP_NameString.charAt(i);
  }

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
  IPAddress myIP = WiFi.softAPIP();
#endif
  // EEPROM
  EEPROM.begin(512);
  delay(10);

  // PWM Servo Driver
  pwm.begin();
  Get_PWM_Frequency();
  pwm.setPWMFreq(Set_PWM_Freq);
  // Software PWM PIN
  servo12.attach(12);
  servo13.attach(13);
  servo14.attach(14);
  servo16.attach(16);

  for ( unsigned int i = 0; i < DURATION_INDEX; i++) {
    Running_Servo_POS[i] = Servo_Act_1[i] + (int8_t)EEPROM.read(i);
  }
  Running_Servo_POS[DURATION_INDEX] = Servo_Act_1[DURATION_INDEX] + (int8_t)EEPROM.read(DURATION_INDEX);

  if (Engineering_Model == 0) {
#if 0    
    Serial.println("MiniPlan V6 SSID : " + AP_NameString);
    Serial.println("MiniPlan V6 IP : " + myIP.toString());
    Serial.println("MiniPlan V6 PW : 12345678");
    Serial.println("Please, connect !");
#if SSD1306
    display.println(AP_NameString);
    display.println("PW:12345678");
    display.println("Please, connect !");
    display.display();
#endif
    while (WiFi.softAPgetStationNum() == 0) {
      Serial.print(".");
      delay(500);
    }

    Serial.println("");
    Serial.println("Connected !");
#endif    
#if SSD1306
    display.println("Connected !");
    display.display();
    OLED_Display_Index = 24;
#endif
  }
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS Mount failed");        // Problème avec le stockage SPIFFS - Serious problem with SPIFFS 
  } else { 
    Serial.println("SPIFFS Mount succesfull");
  }
  enableWebServer();

#if MAX_SSID > 2
  ssid[2] = _SSID3_;
  password[2] = _WIFI_PASSWORD3_;
#endif

#if MAX_SSID > 1
  ssid[1] = _SSID2_;
  password[1] = _WIFI_PASSWORD2_;
#endif

#if MAX_SSID > 0
  ssid[0] = _SSID1_;
  password[0] = _WIFI_PASSWORD1_;
#endif
  
}

/*============================================================================*/
void loop(void) {
  if (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Not connected");
    Serial.println(_NODENAME_);
    Serial.println(_TAG_);
    if (connectWifi() == false) {
      delay(2000);
      return;
    }
    Serial.println("Connected");
  }
/*  
  Serial.print(F("WiFi Status: "));
  Serial.println(WiFi.status());
*/  
  server.handleClient();

  if (Servo_PROGRAM >= 1) {

    Serial.print("Servo_PROGRAM = ");
    Serial.println(Servo_PROGRAM);
#if SSD1306
    OLED_Display_Index = OLED_Display_Index + 8;
    if (OLED_Display_Index > 56) {
      OLED_Display_Index = 0;
      display.clearDisplay();
    }
    display.setCursor(0, OLED_Display_Index);
    display.print("Servo_PROGRAM = ");
    display.println(Servo_PROGRAM);
    display.display();
#endif
    switch (Servo_PROGRAM) {
      case 1:  
        Servo_PROGRAM_Run(Servo_Prg_1, Servo_Prg_1_Step);
        Servo_PROGRAM_Center();
        break;
      case 2:  
        Servo_PROGRAM_Run(Servo_Prg_2, Servo_Prg_2_Step);
        Servo_PROGRAM_Center();
        break;
      case 3:  
        Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
        Servo_PROGRAM_Center();
        break;
      case 4:  
        Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
        Servo_PROGRAM_Center();
        break;
      case 5:  
        Servo_PROGRAM_Run(Servo_Prg_5, Servo_Prg_5_Step);
        Servo_PROGRAM_Center();
        break;
      case 6:  
        Servo_PROGRAM_Run(Servo_Prg_6, Servo_Prg_6_Step);
        Servo_PROGRAM_Center();
        break;
      case 7:  
        Servo_PROGRAM_Run(Servo_Prg_7, Servo_Prg_7_Step);
        Servo_PROGRAM_Center();
        break;
      case 8:  
        Servo_PROGRAM_Run(Servo_Prg_8, Servo_Prg_8_Step);
        Servo_PROGRAM_Center();
        break;
      case 9:  
        Servo_PROGRAM_Run(Servo_Prg_9, Servo_Prg_9_Step);
        Servo_PROGRAM_Center();
        break;
      case 10: 
        Servo_PROGRAM_Run(Servo_Prg_10, Servo_Prg_10_Step);
        Servo_PROGRAM_Center();
        break;
      case 11: 
        Servo_PROGRAM_Run(Servo_Prg_11, Servo_Prg_11_Step);
        Servo_PROGRAM_Center();
        break;
      case 12: 
        Servo_PROGRAM_Run(Servo_Prg_12, Servo_Prg_12_Step);
        Servo_PROGRAM_Run(Servo_Prg_39_3, Servo_Prg_39_3_Step);
        Servo_PROGRAM_Center();
        break;
      case 99: 
        Servo_PROGRAM_Center();
        delay(300);
        break;
      case 100:  
        Servo_PROGRAM_Zero();
        delay(300);
        break;
    }
    Servo_PROGRAM = 0;
  }

  if (Servo_PROGRAM_Stack >= 1) {
    Serial.print("Servo_PRGRAM_Stack = ");
    Serial.println(Servo_PROGRAM_Stack);
#if SSD1306
    OLED_Display_Index = OLED_Display_Index + 8;
    if (OLED_Display_Index > 56) {
      OLED_Display_Index = 0;
      display.clearDisplay();
    }
    display.setCursor(0, OLED_Display_Index);
    display.print("Servo_PRG_Stack = ");
    display.println(Servo_PROGRAM_Stack);
    display.display();
#endif    

    switch (Servo_PROGRAM_Stack) {
      case 1:  
        Servo_PROGRAM_Run(Servo_Prg_31, Servo_Prg_31_Step);
        Servo_PROGRAM_Center();
        break;
      case 2:  
        Servo_PROGRAM_Run(Servo_Prg_32, Servo_Prg_32_Step);
        Servo_PROGRAM_Center();
        break;
      case 3:  
        Servo_PROGRAM_Run(Servo_Prg_33, Servo_Prg_33_Step);
        Servo_PROGRAM_Center();
        break;
      case 4:  
        Servo_PROGRAM_Run(Servo_Prg_34, Servo_Prg_34_Step);
        Servo_PROGRAM_Center();
        break;
      case 5:  
        Servo_PROGRAM_Run(Servo_Prg_35, Servo_Prg_35_Step);
        Servo_PROGRAM_Center();
        break;
      case 6:  
        Servo_PROGRAM_Run(Servo_Prg_36, Servo_Prg_36_Step);
        Servo_PROGRAM_Center();
        break;
      case 7:  
        Servo_PROGRAM_Run(Servo_Prg_37, Servo_Prg_37_Step);
        Servo_PROGRAM_Center();
        break;
      case 8:  
        Servo_PROGRAM_Run(Servo_Prg_38, Servo_Prg_38_Step);
        Servo_PROGRAM_Center();
        break;
      case 9:  
        Servo_PROGRAM_Run(Servo_Prg_39_1, Servo_Prg_39_1_Step);
        Servo_PROGRAM_Run(Servo_Prg_39_2, Servo_Prg_39_2_Step);
        Servo_PROGRAM_Run(Servo_Prg_39_2, Servo_Prg_39_2_Step);
        Servo_PROGRAM_Run(Servo_Prg_39_2, Servo_Prg_39_2_Step);
        Servo_PROGRAM_Run(Servo_Prg_39_3, Servo_Prg_39_3_Step);
        Servo_PROGRAM_Center();
        break;
      case 10: 
        Servo_PROGRAM_Run(Servo_Prg_40, Servo_Prg_40_Step);
        Servo_PROGRAM_Center();
        break;
      case 99: 
        Servo_PROGRAM_Run(Servo_Prg_32, Servo_Prg_32_Step);
        Servo_PROGRAM_Center();
        delay(500);
        Servo_PROGRAM_Run(Servo_Prg_36, Servo_Prg_36_Step);
        Servo_PROGRAM_Center();
        delay(500);
        Servo_PROGRAM_Run(Servo_Prg_34, Servo_Prg_34_Step);
        Servo_PROGRAM_Center();
        delay(500);
        Servo_PROGRAM_Run(Servo_Prg_35, Servo_Prg_35_Step);
        Servo_PROGRAM_Center();
        delay(500);
        Servo_PROGRAM_Run(Servo_Prg_37, Servo_Prg_37_Step);
        Servo_PROGRAM_Center();
        delay(500);
        Servo_PROGRAM_Run(Servo_Prg_31, Servo_Prg_31_Step);
        Servo_PROGRAM_Center();
        break;
    }
    Servo_PROGRAM_Stack = 0;
  }

#if 0
  if (WiFi.softAPgetStationNum() == 0) {
    Serial.println("Disconnect !");
#if SSD1306
    OLED_Display_Index = OLED_Display_Index + 8;
    if (OLED_Display_Index > 56) {
      OLED_Display_Index = 0;
      display.clearDisplay();
    }
    display.setCursor(0, OLED_Display_Index);
    display.println("Disconnect !");
    display.display();
#endif
    while (WiFi.softAPgetStationNum() == 0) {
      delay(200);
    }
    Serial.println("Connected !");
  }
#endif
}


