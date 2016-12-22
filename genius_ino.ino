#define INDEFINIDO -1 // quando o user não apertar nenhum botão

/*	LEDS	*/

#define LED_VERDE 2
#define LED_AMARELO 3
#define LED_VERMELHO 4
#define LED_AZUL 5


/*	BOTÕES	*/

#define BOTAO_VERDE 8
#define BOTAO_AMARELO 9
#define BOTAO_VERMELHO 10
#define BOTAO_AZUL 11

#define TAMANHO_SEQUENCIA 4

/*	POSSÍVEIS ESTADOS DO JOGO	*/
enum Estados{
	PROX_RODADA,
	USER_RESPONDENDO,
	FIM_JOGO_FALHA,
	FIM_JOGO_SUCESSO
};

int g_ledsRespondidos;
int g_rodada = 0;
int g_sequencia[TAMANHO_SEQUENCIA];

void setup() {
  	Serial.begin(9600);
  	init_portas();
  	init_jogo();
}

void init_portas(){
  	pinMode(LED_AMARELO, OUTPUT);
  	pinMode(LED_AZUL, OUTPUT);
  	pinMode(LED_VERDE, OUTPUT);
  	pinMode(LED_VERMELHO, OUTPUT);

  	pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  	pinMode(BOTAO_AZUL, INPUT_PULLUP);
  	pinMode(BOTAO_VERDE, INPUT_PULLUP);
  	pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
}

void init_jogo(){
	int rand = analogRead(0); // resultado de acordo com a variação de energia presente no ar
	randomSeed(rand);

	for (int i = 0; i < TAMANHO_SEQUENCIA; i++)
		g_sequencia[i] = random_led();
}

int random_led(){
	return random(LED_VERDE, LED_AZUL + 1); // random(2, 6)
}

void loop() {
	switch (estado_atual()){
		case PROX_RODADA:
			init_rodada();
			break;
		
		case USER_RESPONDENDO:
			proc_respostaUser();
			break;
		
		case FIM_JOGO_FALHA:
			fim_jogoFalha();
			break;
		
		case FIM_JOGO_SUCESSO:
			fim_jogoSucesso();
			break;
	}
	delay(250);
}

void toca_leds(){
	for (int i = 0; i < g_rodada; i++)
		pisca_led(g_sequencia[i]);
}

int pisca_led(int portaLed){
	digitalWrite(portaLed, HIGH); // HIGH == 1 == true
  	delay(500); // tempo em ms
  	
  	digitalWrite(portaLed, LOW); // LOW == 0 == false 
	delay(400); // tempo em ms

	return portaLed;
}

int estado_atual(){
	if (g_rodada <= TAMANHO_SEQUENCIA){
		if (g_ledsRespondidos == g_rodada)
			return PROX_RODADA;

		else
			return USER_RESPONDENDO;
	}
	
	else if (g_rodada == TAMANHO_SEQUENCIA + 1)
		return FIM_JOGO_SUCESSO;

	else
		return FIM_JOGO_FALHA;
}

void init_rodada(){
	g_rodada++;
	g_ledsRespondidos = 0;

	if (g_rodada <= TAMANHO_SEQUENCIA)
		toca_leds();
}

void proc_respostaUser(){
	int res = ver_resposta();

	if (res == INDEFINIDO)
		return;

	else if (res == g_sequencia[g_ledsRespondidos])
		g_ledsRespondidos++;
	
	else
		g_rodada = TAMANHO_SEQUENCIA + 2;
}

int ver_resposta(){
	if (digitalRead(BOTAO_AZUL) == LOW)
		return pisca_led(LED_AZUL);	
	
	if (digitalRead(BOTAO_AMARELO) == LOW)
		return pisca_led(LED_AMARELO);	
	
	if (digitalRead(BOTAO_VERMELHO) == LOW)
		return pisca_led(LED_VERMELHO);
	
	if (digitalRead(BOTAO_VERDE) == LOW)
		return pisca_led(LED_VERDE);

	return INDEFINIDO;
}

void fim_jogoFalha(){
	digitalWrite(LED_AZUL, HIGH);
	digitalWrite(LED_AMARELO, HIGH);
	digitalWrite(LED_VERDE, HIGH);
	digitalWrite(LED_VERMELHO, HIGH);

	delay(250);

	digitalWrite(LED_AZUL, LOW);
	digitalWrite(LED_AMARELO, LOW);
	digitalWrite(LED_VERDE, LOW);
	digitalWrite(LED_VERMELHO, LOW);
}

void fim_jogoSucesso(){
	digitalWrite(LED_AZUL, HIGH);
	delay(250);
	digitalWrite(LED_AZUL, LOW);
	
	digitalWrite(LED_AMARELO, HIGH);
	delay(250);
	digitalWrite(LED_AMARELO, LOW);
	
	digitalWrite(LED_VERDE, HIGH);
	delay(250);
	digitalWrite(LED_VERDE, LOW);
	
	digitalWrite(LED_VERMELHO, HIGH);
	delay(250);
	digitalWrite(LED_VERMELHO, LOW);
}