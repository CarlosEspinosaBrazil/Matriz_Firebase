#include <Header_Matriz.hpp>
#include <Funcoes_Matriz.h>

String childPath[3];

int anima   = 0;
int intensidade  = 0;
int Vezes   = 0;

#define USAR_SERIAL           //// ATENÇÃO  ////         Comentar se for para produção
// ───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
//                             SETUP
// ───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
void setup(){
   #ifdef USAR_SERIAL
   Serial.begin(115200); Serial.println(F("")); Serial.println(F(""));
   #endif

   //#ifndef USAR_SERIAL
   pinMode(led,OUTPUT);
   digitalWrite(led, HIGH);
   //#endif
   
   Inicializa_WiFi();

   childPath[0] = "/Mensagem";
   childPath[1] = "/intensidade";
   childPath[2] = "/anima";

   // FireBase
   ssl_client1.setClient(&basic_client1);
   ssl_client2.setClient(&basic_client2);
   ssl_client1.setInsecure();
   ssl_client2.setInsecure();
   ssl_client1.setBufferSizes(512,512);
   ssl_client2.setBufferSizes(512,512);  // minimo 1024,512
   ssl_client1.setDebugLevel(1);
   ssl_client2.setDebugLevel(1);
   
   initializeApp(aClient2, app, getAuth(user_auth), asyncCB, "authTask");

   ms = millis();
   while (app.isInitialized() && !app.ready() && millis() - ms < 120 * 1000);
   
   app.getApp<RealtimeDatabase>(Database);
   Database.url(DATABASE_URL);

   Database.setSSEFilters("get,put,patch,keep-alive,cancel,auth_revoked"); // Since v1.2.1

   Database.get(aClient1, "/Espinosa/Matriz", asyncCB, true, "streamTask1");

   // Primeira Carga
   intensidade = Database.get<int>(aClient2, "/Espinosa/Matriz/intensidade");   
   anima = Database.get<int>(aClient2, "/Espinosa/Matriz/anima");   

   // Matriz   
   P.begin();
   P.setIntensity(intensidade);
   P.displayText(curMessage,scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
   //P.displayText("NOVO MUNDO",scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);

   delay(1000); // espinosa
   //P.displayScroll()  / espinosa
   Pisca_BUILTIN();
}
// ───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
//                             LOOP
// ───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
void loop(){
   app.loop();
   Database.loop();

   // Matriz
   if (Vezes < anima)   {
      if (P.displayAnimate()){ // se terminou
         P.displayReset();
         Vezes++;
      }
   }

   if (Processa){
      Processa = false;
      Processar(Path, Valor);
   }
}
// ───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
//                             STREAM CALL BACK
// ───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
void asyncCB(AsyncResult &aResult){
   printResult(aResult);
}
// ───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
//                             PROCESSA STREAM CALL BACK
// ───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
void printResult(AsyncResult &aResult){

   //if (aResult.isEvent())   Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.appEvent().message().c_str(), aResult.appEvent().code());
   //if (aResult.isDebug())   Firebase.printf("Debug msg: %s\n", aResult.debug().c_str());
   //if (aResult.isError()){
   //   Firebase.printf("Error msg: %s, code: %d\n", aResult.error().message().c_str(), aResult.error().code());
   //      ESP.resetHeap();
   //   ESP.reset();      
   //}

   if (aResult.available()){
      RealtimeDatabaseResult &RTDB = aResult.to<RealtimeDatabaseResult>();
      if (RTDB.isStream()){
         //Serial.println(F("----------------------------"));
         ////////Firebase.printf("task : %s\n", aResult.uid().c_str());
         //Firebase.printf("event: %s\n", RTDB.event().c_str());
         //Firebase.printf("path : %s\n", RTDB.dataPath().c_str());
         //Firebase.printf("data : %s\n", RTDB.to<const char *>());
         ////////Firebase.printf("type : %d\n", RTDB.type());

         // ─────────────────────────────────────────────────────  Mudança  ──────────────────────────────────────────────────────────────────────────────────────────────────
         if(RTDB.type() > 0 && RTDB.type() < 6){
            Processa = true;
            Path  = RTDB.dataPath().c_str();
            Valor = RTDB.to<String>();
         }
      }
   }
}
//──────────────────────────────────────────────────────────────────────────────────────────────────────────
//                            Processar
//──────────────────────────────────────────────────────────────────────────────────────────────────────────
void Processar(String Path, String Valor){
   Serial.println(Path);
   Serial.println(Valor);
   Serial.println("---------");

   // Mensagem
   if (childPath[0] == Path){
      strncpy(curMessage, Valor.c_str(), BUF_SIZE - 1);
      curMessage[BUF_SIZE - 1] = '\0';  // Garante que a string seja terminada corretamente
      Vezes = 0;
   }
   // intensidadeidade
   if(childPath[1] == Path) intensidade = Valor.toInt();
   P.setIntensity(intensidade);
      
   // Vezes de Exibicao
   if(childPath[2] == Path) anima = Valor.toInt();


}