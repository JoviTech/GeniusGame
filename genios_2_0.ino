#define led_vermelho 2 
#define led_amarelo 3 
#define led_verde 4 
#define indefinido -1

#define button_vermelho 10
#define button_amarelo 11
#define button_verde 12

#define um_segundo 1000
#define meio_segundo 500

#define tamanho_sequencia 10

#define BUZZER 13

int sequencia_luzes [tamanho_sequencia];
int rodada = 0;
int leds_respondidos = 0;

enum Estados {
  pronto_para_prox_rodada,
  usuario_respondendo,
  jogo_finalizado_sucesso,
  jogo_finalizado_falha

};
//------------------------------------------------------------------------------------------------------------------------------------------------------



void setup() {
  Serial.begin(9600);
  inicia_portas();
  inicia_jogo();

}

void loop() {
  switch (estado_atual()){
    case pronto_para_prox_rodada:
      Serial.println("Pronto para proxima rodada");
      prepara_nova_rodada();
      break;
    case usuario_respondendo:
      Serial.println("Jogador respondendo");
      processa_resposta_usuario();
      break;
    case jogo_finalizado_sucesso:
      Serial.println("Jogo finalizado com sucesso");
      jogoFinalizadoSucesso ();
      break;
    case jogo_finalizado_falha:
      Serial.println("Jogo finalizado com falha");
      jogoFinalizadoFalha();
      break;

  } 
  delay (meio_segundo);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------

void prepara_nova_rodada(){
  rodada++;
  leds_respondidos = 0;
  if (rodada <= tamanho_sequencia){
    toca_leds_rodada();
  }
}

void processa_resposta_usuario(){
  int resposta = checaRespostaJogador();

  if(resposta == indefinido){
    return;
  }

  if (resposta == sequencia_luzes[leds_respondidos]){
  leds_respondidos++;
  }else{
    Serial.println("resposta errada");
    rodada = tamanho_sequencia +2;
  }
}

int estado_atual(){
  if (rodada <= tamanho_sequencia){
    if (leds_respondidos == rodada){
      return pronto_para_prox_rodada;
    }else{
      return usuario_respondendo;
    }
  }else if(rodada == tamanho_sequencia +1){
    return jogo_finalizado_sucesso;
  }else{
    return jogo_finalizado_falha;
  }
}
void inicia_jogo(){
  int jogo = analogRead(0);//porta analogica 0 vai ler tensão dor ar aleatória e gerar um número
  randomSeed (jogo);
  for (int indicie = 0; indicie < tamanho_sequencia; indicie ++){
    sequencia_luzes [indicie]= sorteia_cor();
  }
}

int sorteia_cor(){
  return random (led_vermelho, led_verde + 1);
}

void toca_leds_rodada(){
  for(int indice = 0; indice < rodada; indice++){
    pisca_led(sequencia_luzes[indice]);  
  }

}

void inicia_portas(){

  pinMode (led_vermelho, OUTPUT);
  pinMode (led_amarelo, OUTPUT);
  pinMode (led_verde, OUTPUT);

  pinMode (button_vermelho, INPUT_PULLUP); //PULLUP faz um jumper do 5v até o botão com uma resistencia de 10k
  pinMode (button_amarelo, INPUT_PULLUP);
  pinMode (button_verde, INPUT_PULLUP);

  pinMode (BUZZER, OUTPUT);
}

void tocaSom(int frequencia) {
  tone(BUZZER, frequencia, 50);
}

int checaRespostaJogador() {

  if (digitalRead(button_vermelho) == LOW){
    return pisca_led (led_vermelho);
  }
  if (digitalRead(button_amarelo) == LOW){
    return pisca_led(led_amarelo);
  }
  if (digitalRead(button_verde) == LOW){
    return pisca_led(led_verde);
  }
  return indefinido;
}

int pisca_led (int portaLed) {

  tocaSom(2000);

  digitalWrite(portaLed, HIGH);
  delay (um_segundo);
  digitalWrite(portaLed, LOW);
  delay (meio_segundo);
  return portaLed;
}

int pisca_led_sucesso (int portaLedSucesso) {
  
  digitalWrite(portaLedSucesso, HIGH);
  delay(meio_segundo);
  digitalWrite(portaLedSucesso, LOW);
  //delay(meio_segundo);
  return portaLedSucesso;
}

void jogoFinalizadoSucesso () {

  tocaSom(2000);
  pisca_led_sucesso(led_vermelho);
  tocaSom(2000);
  pisca_led_sucesso(led_amarelo);
  tocaSom(2000);
  pisca_led_sucesso(led_verde);
  tocaSom(2000);

}

void jogoFinalizadoFalha (){
  digitalWrite(led_vermelho, HIGH);
  tocaSom(2000);
  digitalWrite(led_amarelo, HIGH);
  tocaSom(2000);
  digitalWrite(led_verde, HIGH);
  tocaSom(2000);
  delay(200);
  digitalWrite(led_vermelho, LOW);
  tocaSom(2000);
  digitalWrite(led_amarelo, LOW);
  tocaSom(2000);
  digitalWrite(led_verde, LOW);
  tocaSom(2000);
  delay(500);

}

 