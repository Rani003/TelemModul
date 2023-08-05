// PROGRAM AKSES DISPLAY
// menampilkan pesan teks pada layar OLED

#include <Wire.h>
#include <Adafruit GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display lebar, in pixels
#define SCREEN_HEIGHT 64 // OLED display tinggi, in pixels

// Deklarasi untuk SSD1306 display koneksi menggunakan I2C
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

  // inisialisasi OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) {}
  }
}

void loop() {
  display.clearDisplay();

  // Display Teks
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Menampilkan pesan
  display.setCursor(0, 0);
  display.println("Do Your Best");
  display.setCursor(0, 12);
  display.println("And Beat");
  display.setCursor(0, 24);
  display.println("The Challenge");

  // Menganimasikan perpindahan teks dari bawah ke atas
  for (int y = SCREEN_HEIGHT; y >= -36; y--) {
    display.display();
    delay(50); // Kecepatan animasi

    // Hapus tampilan lama dengan menggambar persegi hitam di atasnya
    display.fillRect(0, y, SCREEN_WIDTH, 36, BLACK);

    // Menampilkan pesan lagi dengan posisi yang baru
    display.setCursor(0, y);
    display.println("Do Your Best");
    display.setCursor(0, y + 12);
    display.println("And Beat");
    display.setCursor(0, y + 24);
    display.println("The Challenge");
  }

  delay(2000); // Menahan pesan selama 2 detik sebelum tampilan berubah
}
