#include <IRremote.h> // Includere la libreria IRremote

int R1 = 6 ;//Definisco il Rele1
int R2 = 7 ;//Definisco il Rele2
int temp = 0; // analog pin
int receiver = 11; // Inizializzare il ricevitore al pin 11
int pirPin = 3; //pin del sensore di movimento
int ledPin = 13;//pin del led di stato
int Touch = 12;//pin del sensore di tocco
float celsius = 0, farhenheit = 0; // temperature variables
float millivolts; //dichiarazione di variabile tensione (float è per i numeri con la virgola)
int sensor;
IRrecv irrecv(receiver); // Creare un istanza irrecv
decode_results results;
int calibrationTime = 30;
long unsigned int lowIn;
long unsigned int pause = 5000;
boolean lockLow = true;
boolean takeLowTime;

int TouchVal = 0;
void setup()
{
	Serial.begin(9600);
	pinMode(Touch, INPUT);
	pinMode(pirPin, INPUT);
	pinMode(ledPin, OUTPUT);
	pinMode(R1, OUTPUT); //Setto i relé In OUTPUT prima di avviare il codice
	pinMode(R2, OUTPUT); //Setto i relé In OUTPUT prima di avviare il codice
	digitalWrite(R1, HIGH);//Setto i relé spenti prima di avviare il codice
	digitalWrite(R2, HIGH);//Setto i relé spenti prima di avviare il codice
	digitalWrite(pirPin, LOW);
	Serial.print("calibrating sensor ");
	for(int i = 0; i < calibrationTime; i++)
	{
		Serial.print(".");
		delay(500);
	}
	Serial.println(" done");
	Serial.println("SENSOR ACTIVE");
		lcd.print("SENSOR ACTIVE");

	delay(50);
	Serial.begin(9600); // Avvia la comunicazione seriale
	irrecv.enableIRIn(); // Avvia il ricevitore


}

void loop()
{
	TouchVal = digitalRead(Touch);
	Serial.print("Tocchi:");
	Serial.println(TouchVal);

	if(digitalRead(pirPin) == HIGH)
	{
		digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
		if(lockLow)
		{
			//makes sure we wait for a transition to LOW before any further output is made:
			lockLow = false;
		}
		takeLowTime = true;
	}

	if(digitalRead(pirPin) == LOW)
	{
		digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

		if(takeLowTime)
		{
			lowIn = millis();          //save the time of the transition from high to LOW
			takeLowTime = false;       //make sure this is only done at the start of a LOW phase
		}
	}

	if (irrecv.decode(&results)) // Verificare se il codice é stato ricevuto
	{
		Serial.print("Codice:");
		Serial.println(results.value, HEX); //Visualizza il codice IR in esadecimale
		irrecv.resume();// Ricevi il prossimo valore
	}

	if ( results.value == 0x28D7 ||  results.value == 0xC03F28D7 )  //Volume+
	{
		digitalWrite(R1, LOW);   // Relé1 Acceso

	}
	if ( results.value == 0xF807 ||  results.value == 0xC03FF807 )  //Volume-
	{
		digitalWrite(R1, HIGH );   // Relé1 Spento

	}
	if ( results.value == 0xA857  ||  results.value == 0xC03FA857 )  //ChannelUp
	{
		digitalWrite(R2, LOW );   // Relé2 Acceso
	}
	if ( results.value == 0xA05F  ||  results.value == 0xC03FA05F )  //ChannelDown
	{
		digitalWrite(R2, HIGH);   // Relé2 Spento
	}
	if ( results.value == 0x906F ||  results.value == 0xC03F906F )  //0= 1MinutoTimer
	{
		delay(60000);
		digitalWrite(R1, HIGH);
		digitalWrite(R2, HIGH);      // Tutti i Relé Spenti dopo 1 minuto
	}

	sensor = analogRead(temp); //lettura valore del sensore LM35 messo sull'ingresso analogico A0
	millivolts = ( sensor / 1024.0) * 5000; //formula per ottenere la tensione di uscita dell'LM35 in millivolts
	celsius = millivolts / 10; // valore espresso in gradi Celsius (l'out del sensore è 10mv per grado)

	Serial.print("Temperatura:");
	Serial.println(celsius);

}
