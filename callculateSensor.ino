
void readSensor(){
  if(adzan) return;
  uint32_t currentMillis = millis();
  
  // Variabel Kontrol Tampilan LCD
  static uint32_t volumeDisplayMillis = 0;
  constexpr uint16_t displayDuration = 3000; // 3 detik
  constexpr uint8_t readInterval = 100;
  
  // Membaca potensiometer tanpa menghentikan program lain
  if (currentMillis - lastReadTime >= readInterval) {
    lastReadTime = currentMillis;

    uint32_t sum = 0;
    // 50 sampel tanpa delay di dalam loop for
    for (uint8_t i = 0; i < 60; i++) {
        sum += analogRead(potPin);
    }
    uint16_t averageRaw = sum / 60;

     currentVolume = map(averageRaw, 0, 4095, 0, 30);
    
    // Tetap gunakan pengecekan perubahan agar tidak spamming DFPlayer
    if (abs(currentVolume - lastVolume) >= 1) {
        lastVolume = currentVolume;
         dfplayer.volume(currentVolume);
   
//      Serial.print("Update Volume ke DFPlayer: ");
//      Serial.println(currentVolume);
      //volumeDFPlayer = currentVolume;
      // Aktifkan mode tampilan volume
      showingVolume = true;
      volumeDisplayMillis = currentMillis; 
      show = ANIM_VOLUME;
    }
  }

  // 2. LOGIKA TAMPILAN LCD
  if (showingVolume) {
    if (currentMillis - volumeDisplayMillis >= displayDuration) {
      showingVolume = false;
      //saveToEEPROM();
      show = ANIM_CLOCK;
      lcd.clear(); // Bersihkan bekas tampilan volume
      return;
    }
  } 
}
