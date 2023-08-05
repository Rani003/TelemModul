//AKSES PIN SPI MIKROKONTROLLER
//membaca data tekanan dari sensor BMP280 dan 
//menampilkannya di layar LCD serta melalui koneksi serial;
//data tekanan dalam 10 detik akan disimpan dalam array dan ditampilkan di serial

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#define BMP_SCL PB3 //52 // SCK/scl
#define BMP_SDO PB4 //50 // MISO/sdo
#define BMP_SDA PB5 //51 // MOSI/sda
#define BMP_CSB1 PA15 //53 // Slave Select/csb
Adafruit_BMP280 bmp1(BMP_CSB1, BMP_SDA, BMP_SDO, BMP_SCL);

const int DATA_INTERVAL = 10; // Interval penyimpanan data (detik)
const int DATA_COUNT = DATA_INTERVAL * 1000 / 5000; // Jumlah data dalam interval

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.backlight();
  Serial.println("Starting BMP280 device 1...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Start BMP280 1");
  if (!bmp1.begin()) {
    Serial.println("Sensor BMP280 device 1 was not found.");
    lcd.setCursor(0, 1);
    lcd.print("not found");
    while (1);
  }
  Serial.println("Initialize BMP280 1 completed.");
  lcd.setCursor(0, 1);
  lcd.print("found");
  delay(2000);
}

void loop() {
  static float pressure_data[DATA_COUNT]; // Array untuk menyimpan data tekanan
  static int data_index = 0; // Indeks untuk menyimpan data tekanan

  float pressure = bmp1.readPressure();
  pressure_data[data_index] = pressure;
  data_index = (data_index + 1) % DATA_COUNT; // Melakukan loop pada array

  Serial.print("Pressure 1 = ");
  Serial.print(pressure);
  Serial.print(" Pa, ");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pressure: ");
  lcd.print(pressure);
  lcd.print(" Pa");

  // Tampilkan data tekanan setelah 10 detik
  if (data_index == 0) {
    Serial.println();
    Serial.println("Data Pressure dalam 10 detik:");
    for (int i = 0; i < DATA_COUNT; i++) {
      Serial.print(pressure_data[i]);
      Serial.print(" ");
    }
  }

  Serial.println();
  delay(5000); // Tunggu 5 detik
}


  //kondisi ketika sensor tidak ditemukan
  if(!bmp1.begin()){
    Serial.println("Sensor BMP280 device 1 was not found");
    lcd.setCursor(0,1);
    lcd.print("not found");
    while(1);
  }
  //kondisi jika sensor ditemukan
  Serial.println("Initialized BMP280 1 completed");
  lcd.setCursor(0,1);
  lcd.print("found");
  delay(2000);
}

void loop() {
  //membaca nilai tekanan sensor BMP280 dihitung dalam satuan pascal (Pa)
  float pressure = bmp1.readPressure()/100.0;
  //menampilkan nilai di Serial monitor
  Serial.print("Pressure 1 = ");  
  Serial.print(pressure);
  Serial.print("Pa, ");
  //menampilkan nilai di lcd
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print(pressure); //
  Serial.println();
  delay(100);

}
