#include <WiFi.h>
#include <HTTPClient.h>

#define led_azul 25 // Pino utilizado para controle do led azul
#define led_verde 26 // Pino utilizado para controle do led verda
#define led_vermelho 8// Pino utilizado para controle do led vermelho
#define led_amarelo 3 // Pino utilizado para controle do led azul

const int buttonPin = 46;  // the number of the pushbutton pin
int buttonState = 0;  // variable for reading the pushbutton status

const int ldr = 4;  // the number of the pushbutton pin
int threshold=600;

// Variável de controle se está de noite (Ou seja, qual led deve acender)
bool noite = false;
int valLDR = 0;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_azul,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_amarelo,OUTPUT);


  // Inicialização das entradas
  pinMode(buttonPin, INPUT); // Initialize the pushbutton pin as an input

  digitalWrite(led_azul, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_amarelo, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  // Read LDR value
  valLDR = analogRead(ldr);
  Serial.print("LDR valor: ");
  Serial.println(valLDR);
  
 // Determine se é dia ou noite
  noite = (valLDR < threshold);

  if (noite) {
    // modo noturno tem q piscar led amarelo 1s
    digitalWrite(led_verde, LOW);
    digitalWrite(led_azul, LOW);
    digitalWrite(led_vermelho, LOW);
    digitalWrite(led_amarelo, HIGH);
    delay(1000);
    digitalWrite(led_amarelo, LOW);
    delay(1000);
  } else {
    
    // se não é dia 
    digitalWrite(led_vermelho, LOW);
    digitalWrite(led_verde, HIGH);
    delay(3000);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarelo, HIGH);
    delay(2000);
    digitalWrite(led_amarelo, LOW);
    digitalWrite(led_vermelho, HIGH);
    delay(5000);
    }
}