
//unsigned long tempo_inicio;
//unsigned long tempo_fim;

#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <arduinoFFT.h>

//FFT
arduinoFFT FFT = arduinoFFT();
//#define CHANNEL A0
const uint16_t samples = 4096; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 1500; //Hz, must be less than 10000 due to ADC

unsigned int sampling_period_us;
unsigned long microseconds;

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

int ch1[samples];
int ch2[samples];
int ch3[samples];
int ch4[samples];

//Variáveis para MAX ABS
double mxAbs1, mxAbs2, mxAbs3, mxAbs4;

//Variáveis para RMS
double rms1, rms2, rms3, rms4;

//Variáveis para ARV
double arv1, arv2, arv3, arv4;

//Variáveis para MNF
double mnf1, mnf2, mnf3, mnf4;

//Calcular o máximo valor absoluto do sinal
double mxAbs(int *ch,uint16_t n){
  double mxAbs = 0;
  for(uint16_t i = 0; i < n; i++){
    if(abs(ch[i]) > mxAbs){
      mxAbs = abs(ch[i]);
    }
  }
  return mxAbs;
}

//Calcular a RMS (Root Mean Square)
double rms(int *ch, uint16_t n)
{
  double sum = 0;
  for (uint16_t i = 0; i < n; i++)
    sum += ch[i] * ch[i];
  return sqrt(sum / n);
}

//Calcular a ARV (Root Mean Square)
double arv(int *ch, uint16_t n)
{
  double sum = 0;
  for (uint16_t i = 0; i < n; i++)
    sum += abs(ch[i]);
  return (sum / n);
}

//Calcular a MNF (Mean Frequency)
double mnf(int *ch, uint16_t n){

  double vReal[n];
  double vImag[n] = {0};
  double sum = 0;

  for (uint16_t i = 0; i < n; i++){
        vReal[i] = ch[i];
  }
      FFT.Windowing(vReal, n, FFT_WIN_TYP_HAMMING, FFT_FORWARD); //Janelamento
      FFT.Compute(vReal, vImag, n, FFT_FORWARD); //Calculando Partes Real e Imaginária
      FFT.ComplexToMagnitude(vReal, vImag, n); //Calculando a Densidade Espectral de Potência (PSD)
      //Calculando a Frequência Média
      for (uint16_t i = 0; i < n; i++){
        sum += vReal[i];
      }
      return (sum/n);
}

/*void sendValues(){
  Serial.write(mxAbs1, 4);
  Serial.write(rms1, 4);
  Serial.write(arv1, 4);
  Serial.write(mnf1, 4);
  Serial.write(mxAbs2, 4);
  Serial.write(rms2, 4);
  Serial.write(arv2, 4);
  Serial.write(mnf2, 4);
  Serial.write(mxAbs3, 4);
  Serial.write(rms3, 4);
  Serial.write(arv3, 4);
  Serial.write(mnf3, 4);
  Serial.write(mxAbs4, 4);
  Serial.write(rms4, 4);
  Serial.write(arv4, 4);
  Serial.write(mnf4, 4);
}*/

void setup() {
  sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
  Serial.begin(115200);
}

void loop() {

  char comando = 0;

  if (Serial.available() > 0) {

    comando = Serial.read();

    if (comando == '1') {

      int i = 0;

      /*SAMPLING*/
      microseconds = micros();
      for (int i = 0; i < samples; i++)
      {
        ch1[i] = analogRead(4);
        ch2[i] = analogRead(5);
        ch3[i] = analogRead(10);
        ch4[i] = analogRead(11);
        
        while (micros() - microseconds < sampling_period_us) {
        }
        microseconds += sampling_period_us;
      }

      rms1 = rms(ch1, samples);
      rms2 = rms(ch2, samples);
      rms3 = rms(ch3, samples);
      rms4 = rms(ch4, samples);

      arv1 = arv(ch1, samples);
      arv2 = arv(ch2, samples);
      arv3 = arv(ch3, samples);
      arv4 = arv(ch4, samples);

      mnf1 = mnf(ch1, samples);
      mnf2 = mnf(ch2, samples);
      mnf3 = mnf(ch3, samples);
      mnf4 = mnf(ch4, samples);

      mxAbs1 = mxAbs(ch1, samples);
      mxAbs2 = mxAbs(ch2, samples);
      mxAbs3 = mxAbs(ch3, samples);
      mxAbs4 = mxAbs(ch4, samples);

    }
  }
}
