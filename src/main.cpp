#include <Arduino.h>
#include <WiFi.h>
#include <AsyncUDP.h>
#include <OSCMessage.h>
#include <SPI.h>
// ------------------ Configuración de Red ------------------
const char *ssid = "Personal-A1D";
const char *password = "00435538115";

//IPAddress thisDevice(192, 168, 1, 111);
//IPAddress gateway(192, 168, 1, 254);
//IPAddress subnet(255, 255, 255, 0);
unsigned int oscRXPort = 5555;

// Replace with your network credentials
const char* ap_ssid = "ESP32-OSC";
const char* ap_password = "";

AsyncUDP udp;

// ------------------ MCP4822 DAC Config ------------------
#define DAC_CS 10  // Chip Select
#define SPI_SPEED 20000000  // 20 MHz

// ------------------ GPIO para GATES ------------------
#define GATE_CLOCK 2
#define GATE_CH1   3
#define GATE_CH2   4

// Define the LED pin (built-in LED is usually GPIO 2 on ESP32)
const int LED_PIN = 8;

void setupGates() {
  pinMode(GATE_CLOCK, OUTPUT);
  pinMode(GATE_CH1, OUTPUT);
  pinMode(GATE_CH2, OUTPUT);
  digitalWrite(GATE_CLOCK, LOW);
  digitalWrite(GATE_CH1, LOW);
  digitalWrite(GATE_CH2, LOW);
}

// ------------------ Enviar a DAC ------------------
void writeDAC(uint8_t channel, uint16_t value) {
  // 12-bit value, canal A=0, B=1
  uint16_t command = 0x3000 | (channel << 15) | (value & 0x0FFF);
  digitalWrite(DAC_CS, LOW);
  SPI.transfer16(command);
  digitalWrite(DAC_CS, HIGH);
}

// ------------------ Pulso de GATE breve ------------------
void pulseGate(uint8_t pin, uint16_t durationMs = 20) {
  digitalWrite(pin, HIGH);
  delay(durationMs);
  digitalWrite(pin, LOW);
}

// ------------------ Interpretar mensajes OSC ------------------
void handleOSC(OSCMessage &msg) {
  if (msg.fullMatch("/cv1") && msg.size() > 0) {
    int val = msg.getInt(0);
    val = constrain(val, 0, 4095);
    writeDAC(0, val);  // canal A
    pulseGate(GATE_CH1);
    printf("[CV1] => %d\n", val);

  } else if (msg.fullMatch("/cv2") && msg.size() > 0) {
    int val = msg.getInt(0);
    val = constrain(val, 0, 4095);
    writeDAC(1, val);  // canal B
    pulseGate(GATE_CH2);
    printf("[CV2] => %d\n", val);

  } else if (msg.fullMatch("/clock")) {
    pulseGate(GATE_CLOCK, 5);
    printf("[CLOCK] Pulse");

  } else {
    Serial.printf("Mensaje OSC no reconocido.");
  }

}

// ------------------ Setup WiFi ------------------
void setupWiFi() {
  //WiFi.mode(WIFI_STA);
  //WiFi.config(thisDevice, gateway, subnet);
    // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);

  delay(500);  // Espera para estabilizar el sistema

  Serial.print("Setting AP (Access Point)...");

    // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ap_ssid, ap_password);

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  //WiFi.begin(ssid, password);
  //Serial.print("Conectando a WiFi ");
  //while (WiFi.status() != WL_CONNECTED) {
  //  delay(100);
  //  Serial.print(".");
  //}
  //Serial.println("\nWiFi Conectado");
  //Serial.println(WiFi.localIP());

  // Parpadeo del LED para indicar conexión
  for (int i=0;i<3;i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  } 
}

long rssi = WiFi.RSSI();

// ------------------ Setup ------------------
void setup() {
  Serial.begin(115200);
  delay(500);

  setupWiFi();
  setupGates();

  SPI.begin(6, -1, 7, DAC_CS);  // SCK, -, MOSI, CS
  pinMode(DAC_CS, OUTPUT);
  digitalWrite(DAC_CS, HIGH);

  if (udp.listen(oscRXPort)) {
    Serial.printf("OSC escuchando en puerto %d\n", oscRXPort);
    udp.onPacket([](AsyncUDPPacket packet) {
      OSCMessage msg;
      msg.fill(packet.data(), packet.length());
      if (!msg.hasError()) {
        handleOSC(msg);
      } else {
        printf("Error de parsing OSC");
      }
    });
  }

}

// ------------------ Loop ------------------
void loop() {
  delay(1);  // no bloqueante


  //Serial.println(WiFi.status());  
  //Serial.println(WiFi.localIP());
  //Serial.println("Esperando OSC...");

  //rssi = WiFi.RSSI();
  //Serial.print("signal strength (RSSI):");
  //Serial.print(rssi);
  //Serial.println(" dBm");  

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  delay(5000);  // Espera 5 segundos antes de volver a imprimir la IP del AP
}

