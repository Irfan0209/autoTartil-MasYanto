char* Jadwal[] = {"IMSAK ","SUBUH ", "TERBIT", "DZUHUR", "ASHAR ", "MAGRIB", "ISYA' "};
char* jadwalAzzan[] = {"SUBUH","DZUHUR", "ASHAR", "MAGRIB", "ISYA'"};
char* Hari[]  = {"MINGGU","SENIN","SELASA","RABU","KAMIS","JUM'AT","SABTU"};
char* pasar[]  = {"WAGE", "KLIWON", "LEGI", "PAHING", "PON"}; 
char * namaBulanHijriah[] = {
    "MUHARRAM", "SHAFAR", "RABIUL AWAL",
    "RABIUL AKHIR", "JUMADIL AWAL", 
    "JUMADIL AKHIR", "RAJAB",
    "SYA'BAN", "RAMADHAN", "SYAWAL",
    "DZULQA'DAH", "DZULHIJAH"
};

void showDisplay(){
  static uint32_t saveTimer = 0;
  static uint32_t saveDate = 0;
  static uint8_t sNum,dNum;
  static uint32_t sv = 0;

  RtcDateTime now = Rtc.GetDateTime();
  
  float sholatT[]={JWS.floatImsak,JWS.floatSubuh,JWS.floatTerbit,JWS.floatDzuhur,JWS.floatAshar,JWS.floatMaghrib,JWS.floatIsya};

  if(millis() - saveTimer > 3000){
    saveTimer = millis();
    sNum = (sNum + 1) % 7;
  }
  // ===== SHOLAT =====
  float st = sholatT[sNum];
  uint8_t hh = (uint8_t)st;
  uint8_t mm = (uint8_t)((st - hh) * 60);

  char timeBuf[15];
  snprintf(timeBuf, sizeof(timeBuf), "%s %02d:%02d", Jadwal[sNum], hh, mm);

  lcd.setCursor(0,1);
  lcd.print(timeBuf);
  //dwCtr(0,1,timeBuf);
  
  if(millis() - saveDate > 1500){
   saveDate= millis();
   dNum = (dNum + 1) % 3;
   lcd.setCursor(6,0);
   lcd.print(F("          "));
  }

  switch(dNum){
    case 0 :
     char dt[10];
     snprintf(dt, sizeof(dt), "%02d/%02d/%02d",now.Day(), now.Month(), (now.Year()-2000));
     lcd.setCursor(6,0);
     dwCtr(6,0,dt);
    break;

    case 1 :
     char hr[7];
     snprintf(hr, sizeof(hr), "%s" , Hari[now.DayOfWeek()]);
     lcd.setCursor(6,0);
     dwCtr(6,0,hr);
    break;

    case 2 :
     char ps[7];
     snprintf(ps, sizeof(ps), "%s" , pasar[jumlahhari() % 5]);
     lcd.setCursor(6,0);
     dwCtr(6,0,ps);
    break;
  };
  
  char tm[6];
  snprintf(tm, sizeof(tm), (now.Second()%2)?"%02d:%02d":"%02d %02d",now.Hour(), now.Minute());
     
  lcd.setCursor(0,0); lcd.print(tm);
  
  lcd.setCursor(5,0); lcd.print(F("|"));

}

void drawAzzan()
{
    const char *sholat = jadwalAzzan[sholatNow]; 
    static uint8_t ct = 0;
    static uint32_t lsRn = 0;
    uint32_t Tmr = millis();
    const uint8_t limit = config.durasiadzan;

    if (Tmr - lsRn > 500 && ct <= limit)
    {
        lsRn = Tmr;
        if (!(ct & 1))  // Lebih cepat dibandingkan ct % 2 == 0
        {
          dwCtr(0, 0, "ADZAN");
          dwCtr(0, 1, sholat);
          Buzzer(1);
           
        }
        else
        {
          lcd.clear();
          Buzzer(0);
        }
        ct++;
    }
    
    if ((Tmr - lsRn) > 1500 && (ct > limit))
    {
        show = ANIM_CLOCK;
        ct = 0;
        Buzzer(0);
        lcd.clear();
    }
}


void dwCtr(int8_t x, int8_t y, String Msg){
   uint16_t   tw = Msg.length();
   uint16_t   c = int8_t((DWidth-x-tw)/2);
   lcd.setCursor(x+c,y);
   lcd.print(Msg);
}



void animateSpectrum() {
  // 1. Cetak logo speaker di kolom 14
//  lcd.setCursor(13, 1);
//  lcd.write(byte(0));
  if(adzan) return;
  
  if (isTartilPlaying) {
    static uint32_t prevSpectrumMillis = 0;
  
    // Update animasi setiap 250 milidetik
    if (millis() - prevSpectrumMillis >= 150) { 
      prevSpectrumMillis = millis();
      
      // Cetak 3 bar acak di indeks 13, 14, dan 15
      for (int i = 13; i <= 15; i++) {
        lcd.setCursor(i, 1);
        // Pilih tinggi bar secara acak dari karakter 1 (Low), 2 (Mid), atau 3 (High)
        lcd.write(byte(random(2, 4))); 
      }

    }
  } else {
    // Bersihkan 2 kotak tersebut jika tartil mati
    lcd.setCursor(13, 1);
    lcd.print("   ");
  }
}

// Fungsi menampilkan bar volume
void updateVolumeLCD(uint8_t vol) {
  lcd.setCursor(0, 0);
  lcd.print("Volume: ");
  lcd.print(vol);
  lcd.print(" / 30  "); // Spasi untuk hapus sisa angka lama

  // Membuat Progress Bar sederhana [##########-----]
  lcd.setCursor(0, 1);
  uint8_t barLength = map(vol, 0, 30, 0, 16); // Map ke 16 karakter LCD
  for (uint8_t i = 0; i < 16; i++) {
    if (i < barLength) lcd.print((char)255); // Karakter blok penuh
    else lcd.print("-");
  }
}
