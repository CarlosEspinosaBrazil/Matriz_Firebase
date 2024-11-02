//#include <arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseClient.h>

// Matriz
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW
#define MAX_DEVICES 1
#define CLK_PIN 14
#define DATA_PIN 13
#define CS_PIN 12

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

uint8_t scrollSpeed = 75; // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
uint16_t scrollPause = 0; // in milliseconds

#define BUF_SIZE 250
char curMessage[BUF_SIZE] = {"OK"};


#if defined(USANDO_D1) // Wemos D1
   // ESP12
   #define led 2
#else
   // ESP01
   #define led 1
#endif

// Geral
unsigned long ms     = 0;
bool Processa        = false;
String Path          = "";
String Valor         = "";

// Primeira Carga
String on_off        = "";

#define WIFI_SSID       "rede2"
#define WIFI_PASSWORD   "cacau123"
#define API_KEY         "AIzaSyDkIfIt2BjXdab3Ez8fVEm588BHmFzs8Zk"
#define USER_EMAIL      "isabela.ferreira.sp@gmail.com"
#define USER_PASSWORD   "isaferreira"
#define DATABASE_URL    "automacaoespinosa.firebaseio.com"

DefaultNetwork network(true); // initilize with boolean parameter to enable/disable network reconnection
UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);
FirebaseApp app;
WiFiClient basic_client1, basic_client2;
ESP_SSLClient ssl_client1, ssl_client2;

using AsyncClient = AsyncClientClass;
AsyncClient aClient1(ssl_client1, getNetwork(network)), aClient2(ssl_client2, getNetwork(network));

RealtimeDatabase Database;

// Funcoes
void Inicializa_WiFi();
void asyncCB(AsyncResult &aResult);
void printResult(AsyncResult &aResult);
void Pisca_BUILTIN();
void Processar(String Path, String Valor);
