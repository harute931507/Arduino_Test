char receive_num;
void setup() {
   Serial.begin(9600);          // opens serial port, sets data rate to 9600 bps
   pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  if(Serial.available()>0){     //有收到數值時，會回傳>0的值
    receive_num=Serial.read();  //讀取接收到資料的第一個位元組
    Serial.println(receive_num);
  }

  if(receive_num == '1')
      digitalWrite(LED_BUILTIN,HIGH);
  else if(receive_num == '2')
      digitalWrite(LED_BUILTIN,LOW);
}
