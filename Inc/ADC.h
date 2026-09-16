#include <stm32f407xx.h>
#ifndef INC_ADC_H_
#define INC_ADC_H_

/*--------------------------@Modes------------------------------*/
#define Single            0
#define Continuous        1

/*--------------------------@Resolution------------------------------*/
#define res_12                 0
#define res_10                 1
#define res_8                  2
#define res_6                  3

/*--------------------------@Alignment------------------------------*/
#define right            0
#define left             1

/*--------------------------@Sample_Time------------------------------*/
#define st_3                   0
#define st_15                  1
#define st_28                  2
#define st_56                  3
#define st_84                  4
#define st_112                 5
#define st_144                 6
#define st_480                 7

/*--------------------------@conversions--------------------------------
-> select the number of channels you want to convert on the given ADC.
----------------------------------------------------------------------*/
#define conv_1         1
#define conv_2         2
#define conv_3         3
#define conv_4         4
#define conv_5         5
#define conv_6         6
#define conv_7         7
#define conv_8         8
#define conv_9         9
#define conv_10        10
#define conv_11        11
#define conv_12        12
#define conv_13        13
#define conv_14        14
#define conv_15        15
#define conv_16        16

/*--------------------------@Multi_ADC_Modes--------------------------------
-> Multi ADC mode select in CCR register..
----------------------------------------------------------------------*/
#define INDEPENDENT     0x00
#define SIMULTANEOUS    0x01
#define INTERLEAVED     0x06
#define ALT_TRIGGER     0x07

// ADC Configuration Structure
typedef struct {
    uint8_t Mode;
    uint8_t Res;
    uint8_t ALIGN;
    uint8_t Chan[16];
    uint8_t SAMP;
    uint8_t RegConv;
    uint8_t MultiMode;
    uint8_t DMAEnable; // 0 = disabled, 1 = enabled
} ADC_Config_t;

// ADC Handle Structure
typedef struct {
    ADC_TypeDef *pADCx;
    ADC_Config_t ADC_Config;
} ADC_Handle_t;


void ADC_PeriClockControl(ADC_TypeDef *pADCx, uint8_t EnorDi);
void ADC_Init(ADC_Handle_t *pADCHandle);
uint16_t ADC_Read(ADC_TypeDef *pADCx);

#endif /* INC_ADC_H_ */
