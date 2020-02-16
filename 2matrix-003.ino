// adjusting RTC clock inspired by https://www.hackster.io/garysat/arduino-cheap-minimal-parts-digital-clock-0d750a
// adaptded by Nicu FLORICA (niq_ro)
// http://www.arduinotehniq.com/
// https://nicuflorica.blogspot.com/
// https://arduinotehniq.blogspot.com/


#include <LedControl.h>
#include "RTClib.h"  // https://github.com/adafruit/RTClib
RTC_DS1307 rtc;
/* DS3231/DS1307 RTC module to Arduino bord:
  VCC connects to pin 5V
  GND connets to pin GND
  SCL connects to pin A5 (CL)
  SDA connects to pin A4 (SDA)
 */


/* MAX7219 led display module to Arduino board:
  VCC connects to pin 5V
  GND connets to pin GND
  DIN connects to pin 12
  CLK connects to pin 10
  CS connects to pin 11
*/

LedControl lc = LedControl(12, 10, 11, 2);  // DIN, CLK, CS, numbers of matrix

byte mm1, mm5, h, m, s;
byte ultM = 10, ultH = 10;
byte secunda = 0;
byte secunda0 = 0;
byte hPin = 2, mPin = 3;   // pins for adjust hours amd minutes
boolean ajustaH = true, ajustaM = true;
byte h0, m0;

void setup() {
  while (!Serial); // for Leonardo/Micro/Zero

  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // manual adjust (uncomment line, upload, comment line, upload again)

  pinMode(hPin, INPUT_PULLUP);
  pinMode(mPin, INPUT_PULLUP);
  
  lc.shutdown(0, false);      // power-saving mode on startup
  lc.setIntensity(0, 2);     // Set the brightness to maximum value
  lc.clearDisplay(0);         // and clear the display
  lc.shutdown(1, false);      // power-saving mode on startup
  lc.setIntensity(1, 2);     // Set the brightness to maximum value
  lc.clearDisplay(1);         // and clear the display
}  // end setup

void Display() {
  byte one0[8] = {0x24,0x00,0xff,0x02,0x00,0x00,0x00,0x00};  
  byte two0[8] = {0x24,0x00,0x86,0x89,0xf2,0x00,0x00,0x00};
  byte three0[8] = {0x24,0x00,0xff,0x89,0x81,0x00,0x00,0x00};
  byte four0[8] = {0x24,0x00,0xfc,0x08,0x0f,0x00,0x00,0x00};
  byte five0[8] = {0x24,0x00,0x71,0x89,0x4f,0x00,0x00,0x00};
  byte six0[8] = {0x24,0x00,0x72,0x89,0x7e,0x00,0x00,0x00};
  byte seven0[8] = {0x24,0x00,0x07,0xf9,0x01,0x00,0x00,0x00};
  byte eight0[8] = {0x24,0x00,0x76,0x89,0x76,0x00,0x00,0x00};
  byte nine0[8] = {0x24,0x00,0x7e,0x89,0x46,0x00,0x00,0x00};
  byte ten0[8] = {0x24,0x00,0x7e,0x81,0x7e,0x00,0xff,0x02};
  byte eleven0[8] = {0x24,0x00,0xff,0x02,0x00,0x00,0xff,0x02};
  byte twelve0[8] = {0x24,0x00,0x86,0x89,0xf2,0x00,0xff,0x02};

  byte one1[8] = {0x18,0x00,0xff,0x02,0x00,0x00,0x00,0x00};
  byte two1[8] = {0x18,0x00,0x86,0x89,0xf2,0x00,0x00,0x00};
  byte three1[8] = {0x18,0x00,0xff,0x89,0x81,0x00,0x00,0x00};
  byte four1[8] = {0x18,0x00,0xfc,0x08,0x0f,0x00,0x00,0x00};
  byte five1[8] = {0x18,0x00,0x71,0x89,0x4f,0x00,0x00,0x00};
  byte six1[8] = {0x18,0x00,0x72,0x89,0x7e,0x00,0x00,0x00};
  byte seven1[8] = {0x18,0x00,0x07,0xf9,0x01,0x00,0x00,0x00};
  byte eight1[8] = {0x18,0x00,0x76,0x89,0x76,0x00,0x00,0x00};
  byte nine1[8] = {0x18,0x00,0x7e,0x89,0x46,0x00,0x00,0x00};
  byte ten1[8] = {0x18,0x00,0x7e,0x81,0x7e,0x00,0xff,0x02};
  byte eleven1[8] = {0x18,0x00,0xff,0x02,0x00,0x00,0xff,0x02};
  byte twelve1[8] = {0x18,0x00,0x86,0x89,0xf2,0x00,0xff,0x02};

  // 0-9
  byte zero2[8] = {0x7e,0x81,0x7e,0x00,0x7e,0x81,0x7e,0x00}; //00
  byte one2[8] = {0xff,0x02,0x00,0x00,0x7e,0x81,0x7e,0x00}; //01
  byte two2[8] = {0x86,0x89,0xf2,0x00,0x7e,0x81,0x7e,0x00}; //02
  byte three2[8] = {0xff,0x89,0x89,0x00,0x7e,0x81,0x7e,0x00}; //03
  byte four2[8] = {0xfc,0x08,0x0f,0x00,0x7e,0x81,0x7e,0x00}; //04
  byte five2[8] = {0x71,0x89,0x4f,0x00,0x7e,0x81,0x7e,0x00}; //05
  byte six2[8] = {0x72,0x89,0x7e,0x00,0x7e,0x81,0x7e,0x00}; //06
  byte seven2[8] = {0x07,0xf9,0x01,0x00,0x7e,0x81,0x7e,0x00}; //07
  byte eight2[8] = {0x76,0x89,0x76,0x00,0x7e,0x81,0x7e,0x00}; //08
  byte nine2[8] = {0x7e,0x89,0x46,0x00,0x7e,0x81,0x7e,0x00}; //09
  // 10-19
  byte ten2[8] = {0x7e,0x81,0x7e,0x00,0xff,0x02,0x00,0x00}; //10
  byte eleven2[8] = {0xff,0x02,0x00,0x00,0xff,0x02,0x00,0x00}; //11
  byte twelve2[8] = {0x86,0x89,0xf2,0x00,0xff,0x02,0x00,0x00}; //12
  byte thirteen2[8] = {0xff,0x89,0x89,0x00,0xff,0x02,0x00,0x00}; //13
  byte fourteen2[8] = {0xfc,0x08,0x0f,0x00,0xff,0x02,0x00,0x00}; //14
  byte fifteen2[8] = {0x71,0x89,0x4f,0x00,0xff,0x02,0x00,0x00}; //15
  byte sixteen2[8] = {0x72,0x89,0x7e,0x00,0xff,0x02,0x00,0x00}; //16
  byte seventeen2[8] = {0x07,0xf9,0x01,0x00,0xff,0x02,0x00,0x00}; //17
  byte eighteen2[8] = {0x76,0x89,0x76,0x00,0xff,0x02,0x00,0x00}; //18
  byte nineteen2[8] = {0x7e,0x89,0x46,0x00,0xff,0x02,0x00,0x00}; //19

  // 20-29
  byte twenty2[8] = {0x7e,0x81,0x7e,0x00,0x86,0x89,0xf2,0x00}; //20
  byte twentyone2[8] = {0xff,0x02,0x00,0x00,0x86,0x89,0xf2,0x00}; //21
  byte twentytwo2[8] = {0x86,0x89,0xf2,0x00,0x86,0x89,0xf2,0x00}; //22
  byte twentythree2[8] = {0xff,0x89,0x89,0x00,0x86,0x89,0xf2,0x00}; //23
  byte twentyfour2[8] = {0xfc,0x08,0x0f,0x00,0x86,0x89,0xf2,0x00}; //24
  byte twentyfive2[8] = {0x71,0x89,0x4f,0x00,0x86,0x89,0xf2,0x00}; //25
  byte twentysix2[8] = {0x72,0x89,0x7e,0x00,0x86,0x89,0xf2,0x00}; //26
  byte twentyseven2[8] = {0x07,0xf9,0x01,0x00,0x86,0x89,0xf2,0x00}; //27
  byte twentyeight2[8] = {0x76,0x89,0x76,0x00,0x86,0x89,0xf2,0x00}; //28
  byte twentynine2[8] = {0x7e,0x89,0x46,0x00,0x86,0x89,0xf2,0x00}; //29

  // 30-39
  byte thirty2[8] = {0x7e,0x81,0x7e,0x00,0xff,0x89,0x89,0x00}; //30
  byte thirtyone2[8] = {0xff,0x02,0x00,0x00,0xff,0x89,0x89,0x00}; //31
  byte thirtytwo2[8] = {0x86,0x89,0xf2,0x00,0xff,0x89,0x89,0x00}; //32
  byte thirtythree2[8] = {0xff,0x89,0x89,0x00,0xff,0x89,0x89,0x00}; //33
  byte thirtyfour2[8] = {0xfc,0x08,0x0f,0x00,0xff,0x89,0x89,0x00}; //34
  byte thirtyfive2[8] = {0x71,0x89,0x4f,0x00,0xff,0x89,0x89,0x00}; //35
  byte thirtysix2[8] = {0x72,0x89,0x7e,0x00,0xff,0x89,0x89,0x00}; //36
  byte thirtyseven2[8] = {0x07,0xf9,0x01,0x00,0xff,0x89,0x89,0x00}; //37
  byte thirtyeight2[8] = {0x76,0x89,0x76,0x00,0xff,0x89,0x89,0x00}; //38
  byte thirtynine2[8] = {0x7e,0x89,0x46,0x00,0xff,0x89,0x89,0x00}; //39


  // 40-49
  byte forty2[8] = {0x7e,0x81,0x7e,0x00,0xfc,0x08,0x0f,0x00}; //40
  byte fortyone2[8] = {0xff,0x02,0x00,0x00,0xfc,0x08,0x0f,0x00}; //41
  byte fortytwo2[8] = {0x86,0x89,0xf2,0x00,0xfc,0x08,0x0f,0x00}; //42
  byte fortythree2[8] = {0xff,0x89,0x89,0x00,0xfc,0x08,0x0f,0x00}; //43
  byte fortyfour2[8] = {0xfc,0x08,0x0f,0x00,0xfc,0x08,0x0f,0x00}; //44
  byte fortyfive2[8] = {0x71,0x89,0x4f,0x00,0xfc,0x08,0x0f,0x00}; //45
  byte fortysix2[8] = {0x72,0x89,0x7e,0x00,0xfc,0x08,0x0f,0x00}; //46
  byte fortyseven2[8] = {0x07,0xf9,0x01,0x00,0xfc,0x08,0x0f,0x00}; //47
  byte fortyeight2[8] = {0x76,0x89,0x76,0x00,0xfc,0x08,0x0f,0x00}; //48
  byte fortynine2[8] = {0x7e,0x89,0x46,0x00,0xfc,0x08,0x0f,0x00}; //49

  // 50-59
  byte fifty2[8] = {0x7e,0x81,0x7e,0x00,0x71,0x89,0x4f,0x00}; //50
  byte fiftyone2[8] = {0xff,0x02,0x00,0x00,0x71,0x89,0x4f,0x00}; //51
  byte fiftytwo2[8] = {0x86,0x89,0xf2,0x00,0x71,0x89,0x4f,0x00}; //52
  byte fiftythree2[8] = {0xff,0x89,0x89,0x00,0x71,0x89,0x4f,0x00}; //53
  byte fiftyfour2[8] = {0xfc,0x08,0x0f,0x00,0x71,0x89,0x4f,0x00}; //54
  byte fiftyfive2[8] = {0x71,0x89,0x4f,0x00,0x71,0x89,0x4f,0x00}; //55
  byte fiftysix2[8] = {0x72,0x89,0x7e,0x00,0x71,0x89,0x4f,0x00}; //56
  byte fiftyseven2[8] = {0x07,0xf9,0x01,0x00,0x71,0x89,0x4f,0x00}; //57
  byte fiftyeight2[8] = {0x76,0x89,0x76,0x00,0x71,0x89,0x4f,0x00}; //58
  byte fiftynine2[8] = {0x7e,0x89,0x46,0x00,0x71,0x89,0x4f,0x00}; //59

  // now set hour
  if (h == 1)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, one0[n]);
    }
  }

if (secunda % 2 == 0)
 {
  if (h == 2)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, two0[n]);
    }
  }
  if (h == 3)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, three0[n]);
    }

  }

  if (h == 4)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, four0[n]);
    }

  }
  if (h == 5)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, five0[n]);
    }

  }

  if (h == 6)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, six0[n]);
    }

  }

  if (h == 7)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, seven0[n]);
    }

  }

  if (h == 8)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, eight0[n]);
    }

  }
  if (h == 9)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, nine0[n]);
    }

  }

  if (h == 10)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, ten0[n]);
    }

  }

  if (h == 11)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, eleven0[n]);
    }
  }

  if (h == 12)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, twelve0[n]);
    }
  }
}
else
{
  if (h == 1)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, one1[n]);
    }
  }

  if (h == 2)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, two1[n]);
    }
  }
  if (h == 3)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, three1[n]);
    }

  }

  if (h == 4)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, four1[n]);
    }

  }
  if (h == 5)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, five1[n]);
    }

  }

  if (h == 6)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, six1[n]);
    }

  }

  if (h == 7)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, seven1[n]);
    }

  }

  if (h == 8)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, eight1[n]);
    }

  }
  if (h == 9)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, nine1[n]);
    }

  }

  if (h == 10)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, ten1[n]);
    }

  }

  if (h == 11)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, eleven1[n]);
    }

  }

  if (h == 12)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(0, 7-n, twelve1[n]);
    }

  }
}



  // now set minute
  if (m == 0)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, zero2[n]);
    }

  }

  if (m == 1)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, one2[n]);
    }

  }

  if (m == 2)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, two2[n]);
    }

  }

  if (m == 3)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, three2[n]);
    }

  }

  if (m == 4)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, four2[n]);
    }

  }

  if (m == 5)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, five2[n]);
    }

  }

  if (m == 6)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, six2[n]);
    }

  }

  if (m == 7)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, seven2[n]);
    }

  }

  if (m == 8)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, eight2[n]);
    }

  }
  if (m == 9)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, nine2[n]);
    }

  }

  if (m == 10)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, ten2[n]);
    }

  }

  if (m == 11)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, eleven2[n]);
    }

  }

  if (m == 12)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, twelve2[n]);
    }

  }

  if (m == 13)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, thirteen2[n]);
    }

  }

  if (m == 14)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fourteen2[n]);
    }

  }
  if (m == 15)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fifteen2[n]);
    }

  }
  if (m == 16)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, sixteen2[n]);
    }

  }

  if (m == 17)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, seventeen2[n]);
    }

  }

  if (m == 18)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, eighteen2[n]);
    }

  }

  if (m == 19)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, nineteen2[n]);
    }

  }

  if (m == 20)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, twenty2[n]);
    }

  }

  if (m == 21)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, twentyone2[n]);
    }

  }

  if (m == 22)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, twentytwo2[n]);
    }

  }

  if (m == 23)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, twentythree2[n]);
    }

  }

  if (m == 24)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, twentyfour2[n]);
    }

  }

  if (m == 25)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, twentyfive2[n]);
    }
  }

  if (m == 26)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, twentysix2[n]);
    }

  }

  if (m == 27)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, twentyseven2[n]);
    }

  }

  if (m == 28)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, twentyeight2[n]);
    }

  }

  if (m == 29)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, twentynine2[n]);
    }

  }

  if (m == 30)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, thirty2[n]);
    }

  }

  if (m == 31)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, thirtyone2[n]);
    }

  }

  if (m == 32)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, thirtytwo2[n]);
    }

  }

  if (m == 33)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, thirtythree2[n]);
    }

  }

  if (m == 34)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, thirtyfour2[n]);
    }

  }

  if (m == 35)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, thirtyfive2[n]);
    }

  }

  if (m == 36)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, thirtysix2[n]);
    }

  }

  if (m == 37)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, thirtyseven2[n]);
    }

  }

  if (m == 38)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, thirtyeight2[n]);
    }

  }

  if (m == 39)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, thirtynine2[n]);
    }

  }

  if (m == 40)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, forty2[n]);
    }

  }

  if (m == 41)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fortyone2[n]);
    }

  }

  if (m == 42)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fortytwo2[n]);
    }

  }

  if (m == 43)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fortythree2[n]);
    }

  }

  if (m == 44)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fortyfour2[n]);
    }

  }

  if (m == 45)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fortyfive2[n]);
    }

  }

  if (m == 46)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fortysix2[n]);
    }

  }

  if (m == 47)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fortyseven2[n]);
    }

  }

  if (m == 48)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fortyeight2[n]);
    }

  }

  if (m == 49)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fortynine2[n]);
    }

  }

  if (m == 50)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fifty2[n]);
    }

  }

  if (m == 51)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fiftyone2[n]);
    }
  }

  if (m == 52)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fiftytwo2[n]);
    }

  }

  if (m == 53)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fiftythree2[n]);
    }

  }
  if (m == 54)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fiftyfour2[n]);
    }

  }

  if (m == 55)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fiftyfive2[n]);
    }

  }

  if (m == 56)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fiftysix2[n]);
    }

  }

  if (m == 57)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fiftyseven2[n]);
    }

  }
  if (m == 58)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fiftyeight2[n]);
    }

  }

  if (m == 59)
  {
    for (int n = 0; n < 8; n++) {
      lc.setRow(1, 7-n, fiftynine2[n]);
    }

  }
}


void loop() {
  // Setup of Hours & Minutes
  ajustaH = digitalRead(hPin);
  ajustaM = digitalRead(mPin);

  if (!ajustaH) {
    h = h + 1;
    if ( h > 23) h = 0; 
    rtc.adjust(DateTime(2020, 2, 16, h, m, 0));
  }

  if (!ajustaM) {
    m = m + 1;
    if (m > 59) m = 0; 
    rtc.adjust(DateTime(2020, 2, 16, h, m, 0));
  }
  
  DateTime now = rtc.now();  // read RTC
   h0 = now.hour(), DEC;
   m0 = now.minute(), DEC;
    Serial.print(h0);
    Serial.print(':');
    Serial.print(m0);
    Serial.print(" ->");
  if (h > 12) h = h0 - 12; 
  else
  h = h0;
  if ( h == 0) h = 12;  
  m = m0;
    Serial.print(h);
    Serial.print(':');
    Serial.print(m);
    Serial.println();

// secunda++;
  secunda = millis() / 1000;

if ( secunda != secunda0)
secunda0 = secunda;

  Display();
  delay(250);   
} // end main loop
 
