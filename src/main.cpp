#include <LCD_DISCO_F429ZI.h>
#include <TS_DISCO_F429ZI.h>
#include <mbed.h>
#include <stdint.h>

#include "kiss_fft.h"
#include "kiss_fftndr.h"


LCD_DISCO_F429ZI lcd;
TS_DISCO_F429ZI ts;

AnalogIn aIn(PA_0);
DigitalOut dOut(PG_13);

static const uint16_t SAMPLE_LENGTH = 1024 / 2;
kiss_fft_scalar adcSamples[SAMPLE_LENGTH]{0};
uint16_t sampleIndex = 0;
char buffer[64];
using namespace std;

void initLcd() {
  TS_StateTypeDef TS_State;

  uint8_t status;

  BSP_LCD_SetFont(&Font20);

  status = ts.Init(lcd.GetXSize(), lcd.GetYSize());

  if (status != TS_OK) {
    lcd.Clear(LCD_COLOR_RED);
    lcd.SetBackColor(LCD_COLOR_RED);
    lcd.SetTextColor(LCD_COLOR_WHITE);
    lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"TOUCHSCREEN", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"INIT FAIL", CENTER_MODE);
  } else {
    lcd.Clear(LCD_COLOR_GREEN);
    lcd.SetBackColor(LCD_COLOR_GREEN);
    lcd.SetTextColor(LCD_COLOR_WHITE);
    lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"TOUCHSCREEN", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"INIT OK", CENTER_MODE);
  }

  lcd.Clear(LCD_COLOR_BLUE);
  lcd.SetBackColor(LCD_COLOR_BLUE);
  lcd.SetTextColor(LCD_COLOR_WHITE);
}

int main() {

  kiss_fftr_cfg cfg = kiss_fftr_alloc(SAMPLE_LENGTH, 0, NULL, NULL);

  kiss_fft_cpx *bufout;
  bufout = (kiss_fft_cpx *)malloc(sizeof(kiss_fft_cpx) * SAMPLE_LENGTH);

  // put your setup code here, to run once:
  initLcd();
  while (1) {
    uint64_t dInit =  us_ticker_read();
    uint16_t input_adc0 = aIn.read_u16();
    while ((us_ticker_read() - dInit) < 20 );
    auto usDiff =  us_ticker_read() - dInit;

    adcSamples[sampleIndex] = input_adc0;
    //wait(0.00002f);
    double sampleRate =  1.0f / (usDiff * 1e-6);

    ++sampleIndex;
    float max = 0;
    if (sampleIndex >= SAMPLE_LENGTH) {
      lcd.Clear(LCD_COLOR_BLUE);
      kiss_fftr(cfg, adcSamples, bufout);

      auto maxSamplePosition = 0;
      for (auto i = 1; i < (SAMPLE_LENGTH / 2) + 1; ++i) {
        auto absValue = sqrt(pow(bufout[i].r, 2) + pow(bufout[i].i, 2));
        if (max < absValue) {
          max = absValue;
          maxSamplePosition = i;
        }

        uint32_t p = sqrt(pow(bufout[i].r, 2) + pow(bufout[i].i, 2));
        uint32_t scaled = (p * 280) / max;

        lcd.DrawLine(i, 320, i, 320 - scaled);
      }

      double freq = (maxSamplePosition * sampleRate) / SAMPLE_LENGTH;
      sprintf(buffer, "%lu Hz", static_cast<uint32_t>(freq));
      lcd.DisplayStringAt(0, LINE(0), (uint8_t *)buffer, CENTER_MODE);
      sprintf(buffer, "Fs: %lu Hz", static_cast<uint32_t>(sampleRate));
      lcd.DisplayStringAt(0, LINE(1), (uint8_t *)buffer, CENTER_MODE);

      sampleIndex = 0;

      /*for (auto i = 0; i < (SAMPLE_LENGTH / 2) + 1 ; ++i) {


        uint32_t p = sqrt(pow(bufout[i].r, 2) + pow(bufout[i].i, 2));
        uint32_t scaled = (p * 280) / max;

        lcd.DrawLine(i, 320, i, 320 - scaled);
      }*/
    }

    // put your main code here, to run repeatedly:
    /*dOut = 1;
    wait(0.500f);
    dOut = 0;
    wait(0.500f);
    char buffer[32];
    sprintf(buffer,"%u",aIn.read_u16());
    lcd.DisplayStringAt(0, LINE(5), (uint8_t *)buffer, CENTER_MODE);*/
  }
}
