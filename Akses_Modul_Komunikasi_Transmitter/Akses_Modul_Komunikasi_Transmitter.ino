// PROGRAM AKSES MODUL KOMUNIKASI TRANSMITTER
// melakukan transmisi data menggunakan modul LoRa dengan menggunakan library SX126XLT

#define Program_Version "V1.0"
#include <SPI.h> 
#include <SX126XLT.h> 
SX126XLT LT; 
#define NSS PA15 
#define NRESET PB13 
#define RFBUSY PB12 
#define DIO1 PB10 
#define DIO2 -1 
#define DIO3 -1 
#define RXEN PB14
#define TXEN PB15 
#define SW -1 
#define LORA_DEVICE DEVICE_SX1268 
#define TXpower 22 
uint8_t TXPacketL;
uint32_t TXPacketCount;
uint8_t buff[] = "Hello World 1234567890"; 

void setup() {
  Serial1.begin(9600);
  while (!Serial1) {
    ; // Wait for Serial1 to be ready
  }

  if (LT.begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, RXEN, TXEN, SW, LORA_DEVICE)) {
    Serial1.println(F("LoRa Device found"));
  } else {
    Serial1.println(F("No device responding"));
    while (1);
  }

  LT.setupLoRa(434000000, 0, LORA_SF7, LORA_BW_125, LORA_CR_4_5, LDRO_AUTO);

  Serial1.println(F("Transmitter ready"));
}

void setup() {
  // Inisialisasi komunikasi serial pada Serial1 dengan kecepatan 9600 bps.
  Serial1.begin(9600);
  while (!Serial1) {
    ; // Wait for Serial1 to be ready
  }

  // Memeriksa apakah modul LoRa SX126XLT terdeteksi dan siap berkomunikasi.
  if (LT.begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, RXEN, TXEN, SW, LORA_DEVICE)) {
    Serial1.println(F("LoRa Device found"));
  } else {
    Serial1.println(F("No device responding"));
    while (1);
  }

  // Konfigurasi modul LoRa dengan frekuensi, spreading factor, bandwidth, coding rate, dan low data rate optimization yang telah ditentukan.
  LT.setupLoRa(434000000, 0, LORA_SF7, LORA_BW_125, LORA_CR_4_5, LDRO_AUTO);

  // Cetak pesan "Transmitter ready" ke Serial1.
  Serial1.println(F("Transmitter ready"));
}

void loop() {
  // Cetak tingkat daya transmisi ke Serial1.
  Serial1.print(TXpower);
  Serial1.print(F(" dBm "));
  Serial1.print(F("Packet> "));
  
  // Mengambil ukuran pesan dalam array buff.
  TXPacketL = sizeof(buff);
  // Menambahkan karakter "*" sebagai karakter akhir paket.
  buff[TXPacketL - 1] = '*';
  
  // Cetak isi array buff dalam format ASCII ke Serial1.
  LT.printASCIIPacket(buff, TXPacketL);
  
  // Mengirim data melalui modul LoRa.
  if (LT.transmit(buff, TXPacketL, 10000, TXpower, WAIT_TX)) {
    // Jika pengiriman berhasil, increment TXPacketCount dan panggil fungsi packet_is_OK().
    TXPacketCount++;
    packet_is_OK();
  } else {
    // Jika terjadi kesalahan dalam pengiriman, panggil fungsi packet_is_Error().
    packet_is_Error();
  }
  
  Serial1.println();
  // Jeda selama 1 detik sebelum loop berulang kembali.
  delay(1000);
}

void packet_is_OK() {
  // Cetak informasi jika pengiriman data berhasil.
  Serial1.print(F(" BytesSent, "));
  Serial1.print(TXPacketL);
  Serial1.print(F(" PacketsSent, "));
  Serial1.println(TXPacketCount);
}

void packet_is_Error() {
  // Mendapatkan status IRQ dari modul LoRa dan mencetak informasi jika terjadi kesalahan dalam pengiriman.
  uint16_t IRQStatus;
  IRQStatus = LT.readIrqStatus();
  
  Serial1.print(F(" SendError, Length, "));
  Serial1.print(TXPacketL);
  Serial1.print(F(", IRQreg, 0x"));
  Serial1.print(IRQStatus, HEX);
  
  LT.printIrqStatus();
}
