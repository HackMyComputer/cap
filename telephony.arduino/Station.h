struct Station 
{
  const char* nom;
  float lat;
  float lon;
};

const char s0[] PROGMEM = "Total MANJA";
const char s1[] PROGMEM = "Isoanifanaovana";
const char s2[] PROGMEM = "Valiha";
const char s3[] PROGMEM = "Jovena DOMOINA";
const char s4[] PROGMEM = "Fitiavana";
const char s5[] PROGMEM = "Nirina";
const char s6[] PROGMEM = "Voanio";
const char s7[] PROGMEM = "Galana";
const char s8[] PROGMEM = "Tsarafaritra";
const char s9[] PROGMEM = "Havozo";
const char s10[] PROGMEM = "Fierenana";
const char s11[] PROGMEM = "Galana AVANA";
const char s12[] PROGMEM = "Mahavoky";
const char s13[] PROGMEM = "Ravinala";
const char s14[] PROGMEM = "Anosisoa";
const char s15[] PROGMEM = "Hezaka";
const char s16[] PROGMEM = "Total VOLOMBATO";
const char s17[] PROGMEM = "Railovy";
const char s18[] PROGMEM = "Taxi brousse";
const char s19[] PROGMEM = "Shell Amboditsiry";
const char s20[] PROGMEM = "Galana Analamahitsy";
const char s21[] PROGMEM = "Tatsinanana";
const char s22[] PROGMEM = "Ampasika";
const char s23[] PROGMEM = "Anjoma";
const char s24[] PROGMEM = "Tselatra";
const char s25[] PROGMEM = "Total Fivoarana";
const char s26[] PROGMEM = "Jovena VONJY";
const char s27[] PROGMEM = "Diamondra";
const char s28[] PROGMEM = "Total Fitaratra";
const char s29[] PROGMEM = "Station d'Essence Ambatoroka";
const char s30[] PROGMEM = "JOVENA Antaninandro";
const char s31[] PROGMEM = "Total KINININA";
const char s32[] PROGMEM = "Shell Bakoly";
const char s33[] PROGMEM = "Total";
const char s34[] PROGMEM = "Galana VEROMANITRA";
const char s35[] PROGMEM = "Valasoa";
const char s36[] PROGMEM = "Galana NOFY";
const char s37[] PROGMEM = "Mevasoa";
const char s38[] PROGMEM = "Imaitsoanala";
const char s39[] PROGMEM = "Manakambahiny";
const char s40[] PROGMEM = "Jovenna";
const char s41[] PROGMEM = "Total";
const char s42[] PROGMEM = "Galana KINTANA";
const char s43[] PROGMEM = "Tsiferana";
const char s44[] PROGMEM = "Galana IARIVO Soanierana";
const char s45[] PROGMEM = "Station Service TOTAL";
const char s46[] PROGMEM = "Galana Imahitsy";
const char s47[] PROGMEM = "Galana";
const char s48[] PROGMEM = "Shell";
const char s49[] PROGMEM = "Total Andohatapenaka";
const char s50[] PROGMEM = "Galana Taratra Andrefanambohijanahary";
const char s51[] PROGMEM = "Galana Taratra";
const char s52[] PROGMEM = "TOTAL Service";
const char s53[] PROGMEM = "Vulca";
const char s54[] PROGMEM = "JOVENA Tanikely";
const char s55[] PROGMEM = "Jovena Masay";
const char s56[] PROGMEM = "Total Ankadimbahoaka";
const char s57[] PROGMEM = "Shell Anosizato";

Station stations[] = {
  {s0, -18.9120706, 47.5179075},
  {s1, -18.9067212, 47.5118894},
  {s2, -18.9073071, 47.5129964},
  {s3, -18.9084370, 47.5199352},
  {s4, -18.9072151, 47.5173277},
  {s5, -18.9057675, 47.5210491},
  {s6, -18.9077265, 47.5216411},
  {s7, -18.9458733, 47.5246889},
  {s8, -18.9356137, 47.5288088},
  {s9, -18.9016519, 47.5252294},
  {s10, -18.9177984, 47.5388532},
  {s11, -18.9018461, 47.5275523},
  {s12, -18.8989693, 47.5325129},
  {s13, -18.9017355, 47.5394947},
  {s14, -18.8693731, 47.4917893},
  {s15, -18.8655969, 47.4860427},
  {s16, -18.8866191, 47.5108261},
  {s17, -18.8939145, 47.5283796},
  {s18, -18.8927057, 47.5311704},
  {s19, -18.8885806, 47.5394855},
  {s20, -18.8763412, 47.5470497},
  {s21, -18.8937157, 47.5493230},
  {s22, -18.9098134, 47.4992557},
  {s23, -18.9070195, 47.5267435},
  {s24, -18.9036859, 47.5235575},
  {s25, -18.8970461, 47.5184126},
  {s26, -18.8947673, 47.5166638},
  {s27, -18.8851035, 47.5095691},
  {s28, -18.8850628, 47.5225430},
  {s29, -18.9211567, 47.5404713},
  {s30, -18.9035682, 47.5291913},
  {s31, -18.9014222, 47.5285317},
  {s32, -18.8992367, 47.5259894},
  {s33, -18.9399092, 47.5221401},
  {s34, -18.8713921, 47.5188159},
  {s35, -18.8913513, 47.5377459},
  {s36, -18.8898590, 47.5225271},
  {s37, -18.9241584, 47.5202678},
  {s38, -18.9122431, 47.5285290},
  {s39, -18.9277524, 47.5369713},
  {s40, -18.8998968, 47.5203923},
  {s41, -18.9169559, 47.5357095},
  {s42, -18.9105799, 47.5045549},
  {s43, -18.9212900, 47.5083778},
  {s44, -18.9370530, 47.5225780},
  {s45, -18.9036692, 47.5173755},
  {s46, -18.8712155, 47.5424822},
  {s47, -18.9161579, 47.4749176},
  {s48, -18.9114618, 47.4957949},
  {s49, -18.8974744, 47.4953952},
  {s50, -18.9206976, 47.5213073},
  {s51, -18.9207607, 47.5212849},
  {s52, -18.8927646, 47.5309380},
  {s53, -18.8799909, 47.5055132},
  {s54, -18.9394400, 47.4898636},
  {s55, -18.8814314, 47.5413743},
  {s56, -18.9482070, 47.5285453},
  {s57, -18.9457170, 47.5124670}
};

char nomBuffer[40];

void afficherStation(Station s)
{
  strcpy_P(nomBuffer,(PGM_P)s.nom);
  Serial.print("Station: "); Serial.println(nomBuffer);
}

const int stationCount = sizeof(stations)/sizeof(stations[0]);

float deg2rad(float deg)
{ 
  return deg * 3.14159265 / 180; 
}

float distanceKm(float lat1, float lon1, float lat2, float lon2) 
{
  float R = 6371;
  float dLat = deg2rad(lat2 - lat1);
  float dLon = deg2rad(lon2 - lon1);
  float a = sin(dLat/2) * sin(dLat/2) +
            cos(deg2rad(lat1)) * cos(deg2rad(lat2)) *
            sin(dLon/2) * sin(dLon/2);
  float c = 2 * atan2(sqrt(a), sqrt(1-a));
  return R * c;
}

Station findNearest(float lat, float lon) 
{
  Station nearest = stations[0];
  float minDist = distanceKm(lat, lon, stations[0].lat, stations[0].lon);
  for(int i = 1; i < stationCount; i++) 
  {
    float d = distanceKm(lat, lon, stations[i].lat, stations[i].lon);
    if(d < minDist)
    {
      minDist = d;
      nearest = stations[i];
    }
  }
  return nearest;
}