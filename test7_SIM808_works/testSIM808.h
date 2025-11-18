
class SIM808: public Adafruit_FONA
{
  public: 
    SIM808(SoftwareSerial *__SIM808Serial): 
      Adafruit_FONA(__SIM808Serial),
      opName({0}),
      selfNumber(nullptr),
      personalNumber(nullptr),
      imei({0}) {}
    SIM808(SoftwareSerial *__SIM808Serial, char* pNumber): 
      Adafruit_FONA(__SIM808Serial),
      imei({0}) {
        strncpy(personalNumber, pNumber, strlen(pNumber)); 
      } 
    ~SIM808(); 

    void sendMessage(char *content);
    void sendMessage(char* sendto, char* content); 
    void makeCall(char *sendto);
    void makeCall(char *sendto, uint8_t __dl);
    void getImei();
    void getRssi();
    void getOperatorIdentifier();

    typedef __FlashStringHelper* __StrPtr; 
    typedef char *               __StrPtr_t; 
    // if defined __GNUC__ or some related macro
    // I don't think so this code will be compiled with GCC
    // But we hope it does. 
    //typedef char *restrict       __StrPtrRestr_t; 
  
  private:
    int8_t r; 
    char imei[16];
    char opName[50];  
    __StrPtr_t selfNumber; 
    __StrPtr_t personalNumber;  

    bool __getOperatorName(uint16_t maxLen); 
};

SIM808::~SIM808() {

}

void SIM808::sendMessage(char *content) 
{
  this->sendSMS(personalNumber, content);
}

void SIM808::sendMessage(char *sendto, char *content) 
{
  this->sendSMS(sendto, content);
}

void SIM808::makeCall(char *sendto) {
  // Maybe not defined here 
  //flushSerial();
  Serial.print(F("Calling #"));
  Serial.print(sendto);
  if (!this->callPhone(sendto)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("Sent!"));
  }
}

// The function expects that __dl is in ms 
void SIM808::makeCall(char *sendto, uint8_t __dl) {
  this->makeCall(sendto); 
  delay(__dl);
}

void SIM808::getImei() {
  uint8_t imeiLen = this->getIMEI(imei);
  if  (imeiLen > 0)  {
    Serial.print("Number IMEI of the module: ");
    Serial.println(imei);
    Serial.println();
  }
}

void SIM808::getRssi() {
  Serial.println(F("Reading RSSI:"));
  uint8_t n = this->getRSSI();

  Serial.print(F("RSSI =")); 
  Serial.print(n); 
  Serial.print(": ");
  if  (n == 0) r = -115;
  if  (n == 1) r = -111;
  if  (n == 31) r = -52;
  if  ((n >= 2) && (n <= 30) ) {
    r = map(n, 2, 30, -110, -54);
  }

  Serial.print(r); 
  Serial.println(F(" dBm"));
  
  delay(3000);
  Serial.println();
}
 
bool SIM808::__getOperatorName(uint16_t maxLen) {
  if (!this->sendCheckReply(F("AT+COPS?"), F("OK"), 2000)) {
    return false;
  }

  if (!this->readline(2000)) {
    return false; 
  }

  char *ptr = strstr(this->replybuffer, "\""); 
  if (ptr) {
    ptr++; 
    char *mFirstQuote = strchr(ptr, '"'); 
    if (mFirstQuote) {
      *mFirstQuote = 0; 
      strncpy(opName, ptr, maxLen); 
      opName[maxLen - 1] = 0;  
      return true; 
    }
  }

  return false;
}

void SIM808::getOperatorIdentifier() {
  if (__getOperatorName(sizeof(opName))) {
    
    Serial.print(F("Operator: "));
    Serial.println(opName);
  } else {
    Serial.println(F("Failed to get operator name"));
  }
}