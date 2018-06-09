#pragma mark - Depend ESP8266Audio and ESP8266_Spiram libraries
/* 
cd ~/Arduino/libraries
git clone https://github.com/earlephilhower/ESP8266Audio
git clone https://github.com/Gianbacchio/ESP8266_Spiram
*/


#include <M5Stack.h>
#include <WiFi.h>
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "avator.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

Avator *avator;

void breath(void *args)
{
  int c = 0;
  for(;;)
  {
    c = c + 1 % 100;
    float f = sin(c * 2 * PI / 100.0);
    avator->setBreath(f);
    delay(33);
  }
}

void drawLoop(void *args)
{
  for(;;)
  {
    if (mp3->isRunning()) {
      int level = out->getLevel();
//    Serial.print("level:");
//    Serial.println(abs(level));
      level = abs(level);
      if(level > 20000) level = 20000;
      float open = (float)level/20000.0;
//    avator->setMouthOpen(1.0);
      avator->setMouthOpen(open);
    }
    else
    {
      avator->setMouthOpen(0.0);
    }
    avator->draw();
//    delay(33);
    delay(100);
  }
}

void saccade(void *args)
{
  for(;;)
  {
    float vertical = (float)rand()/(float)(RAND_MAX / 2) - 1;
    float horizontal = (float)rand()/(float)(RAND_MAX / 2) - 1;
    avator->setGaze(vertical, horizontal);
    delay(500 + 100 * random(20));
  }
}

void blink(void *args)
{
  for(;;)
  {
    avator->setEyeOpen(1);
    delay(2500 + 100 * random(20));
    avator->setEyeOpen(0);
    delay(300 + 10 * random(20));
  }
}

void startAvatar()
{
  avator = new Avator();
  xTaskCreatePinnedToCore(
                    drawLoop,     /* Function to implement the task */
                    "drawLoop",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    1,         /* Priority of the task */
                    NULL,      /* Task handle. */
                    0);        /* Core where the task should run */
  xTaskCreatePinnedToCore(
                    saccade,     /* Function to implement the task */
                    "saccade",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    3,         /* Priority of the task */
                    NULL,      /* Task handle. */
                    1);        /* Core where the task should run */
  xTaskCreatePinnedToCore(
                    breath,     /* Function to implement the task */
                    "breath",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    2,         /* Priority of the task */
                    NULL,      /* Task handle. */
                    1);        /* Core where the task should run */
  xTaskCreatePinnedToCore(
                    blink,     /* Function to implement the task */
                    "blink",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    2,         /* Priority of the task */
                    NULL,      /* Task handle. */
                    1);        /* Core where the task should run */
}

void setup()
{
  M5.begin();
  M5.Lcd.setBrightness(30);
  WiFi.mode(WIFI_OFF); 
  delay(500);
  
//  M5.Lcd.setTextFont(2);
//  M5.Lcd.printf("Sample MP3 playback begins...\n");
//  Serial.printf("Sample MP3 playback begins...\n");

  // pno_cs from https://ccrma.stanford.edu/~jos/pasp/Sound_Examples.html
//  file = new AudioFileSourceSD("/pno-cs.mp3");
  out = new AudioOutputI2S(0, 1); // Output to builtInDAC
  out->SetOutputModeMono(true);
  float volume = 20.0;
  out->SetGain(volume/100.0);
  mp3 = new AudioGeneratorMP3();
  
  startAvatar();
}

void stopPlaying()
{
  if (mp3) {
    mp3->stop();
  }
  if (id3) {
    id3->close();
    delete id3;
    id3 = NULL;
  }
  if (file) {
    file->close();
    delete file;
    file = NULL;
  }
}

void loop()
{
  M5.update();
  if (M5.BtnA.wasPressed())
  {
    if (mp3->isRunning()) {
      stopPlaying();
    }
//    M5.Lcd.printf("Sample MP3 playback begins...\n");
    Serial.printf("Sample MP3 playback begins...\n");
    file = new AudioFileSourceSD("/test.mp3");
    id3 = new AudioFileSourceID3(file);
    mp3->begin(id3, out);    
  }
  if (M5.BtnB.wasPressed())
  {
//    M5.Lcd.printf("MP3 done\n");
    Serial.printf("MP3 done\n");
    stopPlaying();
  }
  if (mp3->isRunning()) {
    if (!mp3->loop()) {
      stopPlaying();
    }
  } else {
//    Serial.printf("MP3 done\n");
    delay(100);
  }
}
