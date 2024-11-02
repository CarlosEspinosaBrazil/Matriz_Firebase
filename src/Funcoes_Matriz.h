// ──────────────────────────────────────────────────────────────────────────────────────────────────────────
//                             Inicializa WiFi
// ──────────────────────────────────────────────────────────────────────────────────────────────────────────
void Inicializa_WiFi(){
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   while (WiFi.status() != WL_CONNECTED){
      //Serial.print(".");
      delay(250);
   }
   // WiFi.setAutoReconnect(true);
   // WiFi.persistent(true);

   //Serial.println("");
   //Serial.print("Device Host Name: ");
   //Serial.println(WiFi.hostname());
   //Serial.print("Device MAC: ");
   //Serial.println(WiFi.macAddress());
   //Serial.print("IP address: ");
   //Serial.println(WiFi.localIP());
   //Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);
   //Serial.println("Initializing app...");
}
//──────────────────────────────────────────────────────────────────────────────────────────────────────────
//                            Pisca BUILTIN
//──────────────────────────────────────────────────────────────────────────────────────────────────────────
void Pisca_BUILTIN(){
   #ifndef  USAR_SERIAL
      for (int i = 0; i < 3; i++)   {
         digitalWrite(led, LOW);
         delay(50);
         digitalWrite(led, HIGH);
         delay(50);
      }
   #endif
}
