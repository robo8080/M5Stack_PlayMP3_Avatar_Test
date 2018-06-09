# M5Stack_PlayMP3_Avatar_Test
M5Stackでmp3ファイル再生とAvator表示のテスト


mp3ファイル再生は、M5Stack Libraryのexample"PlayMP3FromSDToDAC"をベースにさせていただきました。<br><br>
Avator表示は、meganetaaanさんのm5stack-avatorをベースにさせていただきました。<br>
オリジナルはこちら。<br>
An M5Stack library for rendering avatar faces <https://github.com/meganetaaan/m5stack-avator><br>

---
### 必要な物 ###
* [M5Stack](http://www.m5stack.com/ "Title") (Grayで動作確認をしました。)<br>
* Arduino IDE (1.8.5で動作確認をしました。)<br>
* [Arduino core for the ESP32](https://github.com/espressif/arduino-esp32 "Title")
* [M5Stack Library](https://github.com/m5stack/M5Stack.git "Title")
* [ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio/ "Title")<br><br>

### ESP8266Audioライブラリの修正 ###
"AudioOutputI2S.h"に下図赤枠部分を追加します。
![画像1](images/image1.png)<br><br>
"AudioOutputI2S.cpp"に下図赤枠部分を追加します。
![画像2](images/image2.png)<br>
![画像3](images/image3.png)<br><br><br>

### 操作方法 ###
1. SDカードに"test.mp3"というファイル名で、mp3ファイルを入れておきます。
2. M5StackのAボタンで再生開始します。
3. M5StackのBボタンで再生停止します。
<br><br><br>
