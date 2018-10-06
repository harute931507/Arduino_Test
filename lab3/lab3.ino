const int Trig_Pins = 3;//trig 腳位
const int Echo_Pins = 2;// echo 腳位

float get_distance(int trig, int echo){  //計算距離
  float duration;
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);            //給予trig 10us TTL pulse,讓模組發射聲波
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH, 5000000);//紀錄echo電位從high到low的時間，就是超音波來回的時間，若5秒內沒收到超音波則回傳0
  return duration / 29 / 2;         // 聲速340m/s ，換算後約每29微秒走一公分，超音波來回所以再除2
}

void setup() {
  Serial.begin(9600);
  pinMode(Trig_Pins, OUTPUT);
  pinMode(Echo_Pins, INPUT);
}

void loop() {
    float result= get_distance( Trig_Pins, Echo_Pins);
    Serial.print(result);
    Serial.println("cm");
    delay(1000);
}
