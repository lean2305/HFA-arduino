#include <ESPAsyncWebSrv.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <IPAddress.h>

IPAddress ip(10, 0, 5, 124); // Endereço IP fixo
IPAddress subnet(255, 255, 0, 0); // Máscara de sub-rede
IPAddress gateway(10, 0, 0, 1); // Gateway padrão

const char* ssid = "Hfa"; // Nome da rede
const char* password = "ascoteloffice20"; // Senha da rede

#define buzzer 13 // Pino em que o buzzer está ligado
#define button 12 // Pino em que o botão está ligado

AsyncWebServer server(80); // Abre um servidor na porta 80
// Notas da melodia animada

// Melody notes
int melody[] = {
  659, 659, 659, 523, 659, 784, 523, 392,
  523, 659, 784, 523, 659, 880, 698, 659, 523, 587,
  494, 523, 392, 330, 440, 494, 392, 659, 784,
  880, 698, 659, 523, 587, 494, 523, 392, 330, 440, 494,
  392, 659, 784, 880, 698, 659, 523, 587, 494, 523, 392, 330, 440, 494, 392,
  392, 784, 880, 932, 880, 784, 659, 587, 523, 494, 523, 392, 659, 784, 880,
  698, 659, 523, 587, 494, 523, 392, 330, 440, 494, 392
};

// Note durations
int noteDurations[] = {
  8, 8, 8, 4, 8, 8, 4, 4,
  8, 8, 8, 4, 8, 8, 8, 8, 8, 8,
   4, 8, 8, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 8, 8, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8,
  8, 8, 8, 4, 8, 8, 4, 4, 4, 4, 4, 4, 4, 8, 8,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
};


volatile bool playMelody = false;

void playHappyBirthday() {
  while (true) {
    for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(melody[0]); thisNote++) {
      int noteDuration = 500 / noteDurations[thisNote];
      tone(buzzer, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;

      // Verifica se o botão foi pressionado antes de tocar a próxima nota
      while (digitalRead(button) == HIGH && noteDuration > 0) {
        delay(10);
        noteDuration -= 10;
      }

      // Se o botão foi pressionado, para a melodia e sai do loop
      if (digitalRead(button) == LOW) {
        noTone(buzzer);
        return;
      }

      delay(pauseBetweenNotes);
      noTone(buzzer);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Define o que cada Pino será responsável por fazer
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT_PULLUP); // INPUT_PULLUP é para usar a resisência do próprio arduino em vez de uma resistência externa

  // Conectar à rede WIFI
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(WiFi.localIP());
  }

  // Recebe o request do website para o IP/start e ativa o buzzer
  server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Endpoint /start accessed");
    playMelody = true;
    request->send(200, "text/plain", "Melody started");
  });

  server.begin();
}

void loop() {
  // Se playMelody for verdadeiro, toca a melodia "Happy Birthday"
  if (playMelody) {
    playHappyBirthday();
    playMelody = false;
  }
  delay(100);
}
