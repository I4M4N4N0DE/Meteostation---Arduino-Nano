#include <DHT.h> // knihovna pro senzor teploty a vlhkosti
#include <U8glib.h> // knihovna pro displej 
#define dhtpin 9 // deklarace proměnné pro vstupní pin senzoru
#define dhttype DHT22 // deklarace typu senzoru pod proměnnou dhttype

// senzor DHT22: pravý vývod ukostřen, levý na 5V bez odporu, druhý zleva na data, PIN č.9
DHT dht(9, dhttype);

// Piny displeje jsou: E=12, RW=11, RS=10, BLA = 3V, BLK=ukostřena
U8GLIB_ST7920_128X64 u8g(12, 11, 10, U8G_PIN_NONE); 

// u charakterů (předem kapacitně definovaným Stringů) je dobré mít rezervu místa, aby nemohlo dojít k přečerpání limitu znaků - pro nás stačí 5 znaků
char temperature [5]; // deklarace pětiprvkových řetězcových (String) charakterů - zde pro teplotu
char humidity [5]; // a zde pro vlhkost vzduchu
const char DEGREE_SYMBOL[] = { 0xB0, '\0' }; // deklarace statického pole charakteru do paměti, symbol stupně °

// nastavení sériové komunikace, fontu a barvy
void setup() {  

  // spuštění funkce senzoru dht
  dht.begin();

  // započetí sériové komunikace s displejem
  Serial.begin(9600); 

  // spuštění funkce pro displej
  u8g.begin(); 

  // nastavení fontu pro displej
  u8g.setFont(u8g_font_unifont); 

  // nastavení barvy pro displej (1 = bílá)
  u8g.setColorIndex(1);
}

// nekonečná funkce - volání funkce meteo() dokola na dvě stránky displeje, které se obnovují
void loop() {   
  u8g.firstPage(); // zobrazení první "strany"

  // smyčka, která spustí funkci meteo(), dokud se nezobrazí druhá "strana", která je nastavena za 20 sekund
  do { 
    meteo();
  }
    while (u8g.nextPage());
    delay(20); // čas mezi "stranami"
  }

// funkce zjištění teploty
void readTemperature() 
{
  // detekce teploty pod proměnnou t = desetinné číslo
  float t = dht.readTemperature(); 

  // vytvoření řetězce (Stringu) z čísla a jeho uložení pod charakter temperature
  dtostrf(t, 3, 1, temperature); 
}

//funkce zjištění vlhkosti
void readHumidity() 
{
  // detekce vlhkosti pod proměnnou h = desetinné číslo
  float h = dht.readHumidity(); 

  // vytvoření řetězce (Stringu) z čísla a jeho uložení pod charakter humidity
  dtostrf(h, 3, 1, humidity);
}

// meteo funkce
void meteo() { 
   
  // zobrazení rámečků
  u8g.drawFrame(0,0,128,31);       
  u8g.drawFrame(0,33,128,31);   

  // spuštění funkcí detekce teploty a vlhkosti
  readTemperature(); 
  readHumidity(); 
           
  // tisk teploty
  u8g.drawStr(3, 13, "Teplota");  
  u8g.drawStr(70, 28, temperature); 
  u8g.drawStr(103, 28, DEGREE_SYMBOL);
  u8g.drawStr(107, 28, "C");  

  // tisk vlhkosti
  u8g.drawStr(3,46, "Vlhkost");     
  u8g.drawStr(70, 61, humidity); 
  u8g.drawStr(105,61, "%");  
}
