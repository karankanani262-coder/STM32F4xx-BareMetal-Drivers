#include "ADC.h"

void ADC_PeriClockControl(ADC_TypeDef *pADCx, uint8_t EnorDi)
{
    if (EnorDi == 1)
    {
        if (pADCx == ADC1)
            RCC->APB2ENR |= (1 << 8);
        else if (pADCx == ADC2)
            RCC->APB2ENR |= (1 << 9);
        else if (pADCx == ADC3)
            RCC->APB2ENR |= (1 << 10);
    }
    else
    {
        if (pADCx == ADC1)
            RCC->APB2ENR &= ~(1 << 8);
        else if (pADCx == ADC2)
            RCC->APB2ENR &= ~(1 << 9);
        else if (pADCx == ADC3)
            RCC->APB2ENR &= ~(1 << 10);
    }
}

void ADC_Init(ADC_Handle_t *pADCHandle)
{
    ADC_PeriClockControl(pADCHandle->pADCx, 1);

    /* Enable ADC */
    pADCHandle->pADCx->CR2 |= (1 << 0); // ADON

    /* Resolution */
    pADCHandle->pADCx->CR1 &= ~(3 << 24);
    pADCHandle->pADCx->CR1 |= (pADCHandle->ADC_Config.Res << 24);

    /* Alignment */
    if (pADCHandle->ADC_Config.ALIGN == left)
        pADCHandle->pADCx->CR2 |= (1 << 11);
    else
        pADCHandle->pADCx->CR2 &= ~(1 << 11);

    /* Continuous mode */
    if (pADCHandle->ADC_Config.Mode == Continuous)
        pADCHandle->pADCx->CR2 |= (1 << 1);
    else
        pADCHandle->pADCx->CR2 &= ~(1 << 1);

    /* Enable SCAN mode if more than 1 channel */
    if (pADCHandle->ADC_Config.RegConv > 1)
        pADCHandle->pADCx->CR1 |= (1 << 8);   // SCAN
    else
        pADCHandle->pADCx->CR1 &= ~(1 << 8);

    /* Sampling time for all channels */
    for (uint8_t i = 0; i < pADCHandle->ADC_Config.RegConv; i++)
    {
        uint8_t ch = pADCHandle->ADC_Config.Chan[i];

        if (ch <= 9)
        {
            pADCHandle->pADCx->SMPR2 &= ~(7 << (ch * 3));
            pADCHandle->pADCx->SMPR2 |= (pADCHandle->ADC_Config.SAMP << (ch * 3));
        }
        else
        {
            uint8_t c = ch - 10;
            pADCHandle->pADCx->SMPR1 &= ~(7 << (c * 3));
            pADCHandle->pADCx->SMPR1 |= (pADCHandle->ADC_Config.SAMP << (c * 3));
        }
    }

    /* Conversion sequence length */
    pADCHandle->pADCx->SQR1 &= ~(0xF << 20);
    pADCHandle->pADCx->SQR1 |= ((pADCHandle->ADC_Config.RegConv - 1) << 20);

    /* Sequence order */
    pADCHandle->pADCx->SQR3 = 0;
    for (uint8_t i = 0; i < pADCHandle->ADC_Config.RegConv && i < 6; i++)
    {
        pADCHandle->pADCx->SQR3 |= (pADCHandle->ADC_Config.Chan[i] << (i * 5));
    }

    /* Multi ADC mode */
    ADC->CCR &= ~(0x1F << 0);
    ADC->CCR |= (pADCHandle->ADC_Config.MultiMode << 0);

    /* DMA */
    if (pADCHandle->ADC_Config.DMAEnable)
    {
        pADCHandle->pADCx->CR2 |= (1 << 8);   // DMA
        pADCHandle->pADCx->CR2 |= (1 << 9);   // DDS
    }
    else
    {
        pADCHandle->pADCx->CR2 &= ~((1 << 8) | (1 << 9));
    }
}

uint16_t ADC_Read(ADC_TypeDef *pADCx)
{
    pADCx->CR2 |= (1 << 30); // SWSTART
    // Wait for EOC
    while (!(pADCx->SR & (1 << 1)));
    return (uint16_t)(pADCx->DR & 0xFFFF);
}


