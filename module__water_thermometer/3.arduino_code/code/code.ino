//for thermistor TDC210, in water, R1 = 0.992 kOhm,

#define ADC_PIN A0
#define SAMPLING_PERIOD 500 //(ms)

#define COEFF3 -2.36343e-7
#define COEFF2 0.000397784
#define COEFF1 -0.312020
#define COEFF0 112.279

#define TAU 3.0 //(s)


void setup() {
  pinMode(ADC_PIN, INPUT);
  Serial.begin(9600);  
}


void loop() {
  static float T_pre = -1, T_prepre = -1;
  static short flat_len = 0;
  
  // convert ADC value to temperature
  int v = analogRead(A0);
  float T = ((COEFF3*v + COEFF2)*v + COEFF1)*v + COEFF0; //(deg-C)

  if(T_pre > 0 && T_prepre > 0)
  {
    float diff = T - T_prepre;
    float T_dot_pre = diff/(SAMPLING_PERIOD*2.0)*1e3; //(deg-C/s)
    
    float T_corr_pre = T_pre; //corrected T
    float dT_cri = T_pre < 55 ? 0.17 : 0.29;
    if(true)
    //if(diff > dT_cri || diff < -dT_cri)
      T_corr_pre += T_dot_pre*TAU;

//    Serial.println(T_corr_pre);
//    Serial.println(T);
    Serial.print(T);
    Serial.print('\t');
    Serial.print(T_corr_pre);
//    Serial.print('\t');
//    Serial.print(flat_len);
//    Serial.print('\t');
//    Serial.print(flat_len_prev);
    Serial.print('\n');

    if(T == T_pre)
      flat_len ++;
    else
      flat_len = 0;
  }

  //
  T_prepre = T_pre;
  T_pre = T;
  delay (SAMPLING_PERIOD);
}
