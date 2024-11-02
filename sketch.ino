#include <Arduino.h>

const int joyX = A0;      // Joystick X ekseni
const int joyY = A1;      // Joystick Y ekseni
const int buttonPin = 2;  // Joystick butonu
const int buzzerPin = 8;  // Buzzer pini
const int ledPin = 9;     // LED pini
const int potPin = A2;    // Potansiyometre pini

int lastSignalTime = 0;        // Son sinyalin gönderildiği zaman
String signalSequence = "";    // Sinyal dizisi
String currentWord = "";       // Yazılan kelimeyi tutan değişken
unsigned long lastButtonPress = 0;
unsigned long lastDisplayTime = 0;  // Mevcut kelimeyi ekrana yazdırma süresi

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int xValue = analogRead(joyX);
  int yValue = analogRead(joyY);
  int potValue = analogRead(potPin);  // Potansiyometre değerini oku

  // Joystick konumunu kontrol et - Y ekseni sinyalleri (yukarı veya aşağı)
  if (yValue < 200) {  // İleri hareket - Kısa sinyal
    if (millis() - lastSignalTime > 500) {  // Tekrarlanan sinyalleri önlemek için
      Serial.println("Kısa sinyal gönderildi.");
      signalSequence += "."; // Kısa sinyal (.)
      digitalWrite(ledPin, HIGH);
      tone(buzzerPin, map(potValue, 0, 1023, 300, 2000), 250);  // Potansiyometreye göre ses tonu ayarla
      delay(250);  // Çeyrek saniye bekle
      digitalWrite(ledPin, LOW);
      lastSignalTime = millis();
    }
  } else if (yValue > 800) {  // Aşağı hareket - Uzun sinyal
    if (millis() - lastSignalTime > 500) {  // Tekrarlanan sinyalleri önlemek için
      Serial.println("Uzun sinyal gönderildi.");
      signalSequence += "-"; // Uzun sinyal (-)
      digitalWrite(ledPin, HIGH);
      tone(buzzerPin, map(potValue, 0, 1023, 300, 2000), 500);  // Potansiyometreye göre ses tonu ayarla
      delay(500);  // Yarım saniye bekle
      digitalWrite(ledPin, LOW);
      lastSignalTime = millis();
    }
  } 
  // Joystick X ekseni - sola hareket ettirildiğinde boşluk bırak
  else if (xValue < 200) {  
    if (millis() - lastSignalTime > 1000) {
      Serial.println("Boşluk bırakıldı.");
      currentWord += " ";  // Kelimeye boşluk ekle
      Serial.print("Güncel kelime (boşluk eklenmiş): ");
      Serial.println(currentWord);  // Boşluk bırakıldığını göster
      lastSignalTime = millis();
    }
  }
  // Joystick sağa hareket ettirildiğinde "backspace" işlevi
  else if (xValue > 800) {  
    if (millis() - lastSignalTime > 750) {
      if (signalSequence.length() > 0) {
        // Eğer sinyal varsa, son sinyali sil
        Serial.println("Son sinyal silindi.");
        signalSequence.remove(signalSequence.length() - 1);  // Son karakteri sil
      } else if (currentWord.length() > 0) {
        // Eğer harf varsa, harfi sil
        currentWord.remove(currentWord.length() - 1);  // Son karakteri sil
        Serial.println("Backspace uygulandı.");
        Serial.print("Güncel kelime (silinmiş): ");
        Serial.println(currentWord);  // Silinmiş kelimeyi göster
      } else {
        Serial.println("Silinecek karakter yok.");  // Eğer kelime boşsa mesaj yaz
      }
      lastSignalTime = millis();
    }
  }
  else {  // Joystick merkezde
    if (digitalRead(buttonPin) == LOW) {
      unsigned long currentTime = millis();
      if (currentTime - lastButtonPress > 500) {  // Buton basma süresi
        lastButtonPress = currentTime;
        addCharacter();  // Harfi yazdır
      }
    }
  }

  // 5 saniyede bir mevcut kelimeyi ekrana yazdır
  if (millis() - lastDisplayTime > 5000) {
    Serial.print("Mevcut kelime: ");
    Serial.println(currentWord);
    lastDisplayTime = millis();
  }

  delay(100);  // Döngüyü biraz yavaşlat
}

void addCharacter() {
  Serial.print("Sinyal dizisi: ");
  Serial.println(signalSequence);

  if (signalSequence == ".-") {
    currentWord += "A";  // Kısa + Uzun (A)
  } else if (signalSequence == "-...") {
    currentWord += "B";  // Uzun + Kısa + Kısa + Kısa (B)
  } else if (signalSequence == "-.-.") {
    currentWord += "C";  // Uzun + Kısa + Uzun + Kısa (C)
  } else if (signalSequence == "-.-..") {
    currentWord += "Ç";  // Uzun + Kısa + Uzun + Kısa + Kısa (Ç)
  } else if (signalSequence == "-..") {
    currentWord += "D";  // Uzun + Kısa + Kısa (D)
  } else if (signalSequence == ".") {
    currentWord += "E";  // Kısa (E)
  } else if (signalSequence == "..-.") {
    currentWord += "F";  // Kısa + Kısa + Uzun + Kısa (F)
  } else if (signalSequence == "--.") {
    currentWord += "G";  // Uzun + Uzun + Kısa (G)
  } else if (signalSequence == "--.-.") {
    currentWord += "Ğ";  // Uzun + Uzun + Kısa + Uzun + Kısa (Ğ)
  } else if (signalSequence == "....") {
    currentWord += "H";  // Kısa + Kısa + Kısa + Kısa (H)
  } else if (signalSequence == "..") {
    currentWord += "I";  // Kısa + Kısa (I)
  } else if (signalSequence == ".---") {
    currentWord += "J";  // Kısa + Uzun + Uzun + Uzun (J)
  } else if (signalSequence == "-.-") {
    currentWord += "K";  // Uzun + Kısa + Uzun (K)
  } else if (signalSequence == ".-..") {
    currentWord += "L";  // Kısa + Uzun + Kısa + Kısa (L)
  } else if (signalSequence == "--") {
    currentWord += "M";  // Uzun + Uzun (M)
  } else if (signalSequence == "-.") {
    currentWord += "N";  // Uzun + Kısa (N)
  } else if (signalSequence == "---") {
    currentWord += "O";  // Uzun + Uzun + Uzun (O)
  } else if (signalSequence == "---.") {
    currentWord += "Ö";  // Uzun + Uzun + Uzun + KısaS (Ö)
  } else if (signalSequence == ".--.") {
    currentWord += "P";  // Kısa + Uzun + Uzun + Kısa (P)
  } else if (signalSequence == "--.-") {
    currentWord += "Q";  // Uzun + Uzun + Kısa + Uzun (Q)
  } else if (signalSequence == ".-.") {
    currentWord += "R";  // Kısa + Uzun + Kısa (R)
  } else if (signalSequence == "...") {
    currentWord += "S";  // Kısa + Kısa + Kısa (S)
  } else if (signalSequence == "-") {
    currentWord += "T";  // Uzun (T)
  } else if (signalSequence == "..-") {
    currentWord += "U";  // Kısa + Kısa + Uzun (U)
  } else if (signalSequence == "...-") {
    currentWord += "V";  // Kısa + Kısa + Kısa + Uzun (V)
  } else if (signalSequence == ".--") {
    currentWord += "W";  // Kısa + Uzun + Uzun (W)
  } else if (signalSequence == "-..-") {
    currentWord += "X";  // Uzun + Kısa + Kısa + Uzun (X)
  } else if (signalSequence == "-.--") {
    currentWord += "Y";  // Uzun + Kısa + Uzun + Uzun (Y)
  } else if (signalSequence == "--..") {
    currentWord += "Z";  // Uzun + Uzun + Kısa + Kısa (Z)
  } else if (signalSequence == ".----") {
    currentWord += "1";  // Kısa + Uzun + Uzun + Uzun + Uzun (1)
  } else if (signalSequence == "..---") {
    currentWord += "2";  // Kısa + Kısa + Uzun + Uzun + Uzun (2)
  } else if (signalSequence == "...--") {
    currentWord += "3";  // Kısa + Kısa + Kısa + Uzun + Uzun (3)
  } else if (signalSequence == "....-") {
    currentWord += "4";  // Kısa + Kısa + Kısa + Kısa + Uzun (4)
  } else if (signalSequence == ".....") {
    currentWord += "5";  // Kısa + Kısa + Kısa + Kısa + Kısa (5)
  } else if (signalSequence == "-....") {
    currentWord += "6";  // Uzun + Kısa + Kısa + Kısa + Kısa (6)
  } else if (signalSequence == "--...") {
    currentWord += "7";  // Uzun + Uzun + Kısa + Kısa + Kısa (7)
  } else if (signalSequence == "---..") {
    currentWord += "8";  // Uzun + Uzun + Uzun + Kısa + Kısa (8)
  } else if (signalSequence == "----.") {
    currentWord += "9";  // Uzun + Uzun + Uzun + Uzun + Kısa (9)
  } else if (signalSequence == "-----") {
    currentWord += "0";  // Uzun + Uzun + Uzun + Uzun + Uzun (0)
  } else {
    Serial.println("Geçersiz sinyal");  // Tanımsız sinyal kombinasyonu
  }

  Serial.print("Şu ana kadar yazılan kelime: ");
  Serial.println(currentWord);

  // Sinyal dizisini sıfırla
  signalSequence = "";
}
