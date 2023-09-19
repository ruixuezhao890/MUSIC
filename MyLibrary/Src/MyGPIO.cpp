/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MyGPIO.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/11 
*********************************************************************
*********
*/
//

#include "MyGPIO.h"

using namespace stm32f407;
GPIO_TypeDef * Port[]{
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
};
void GPIO::RCCEnable(stm32f407::PORT_INDEX portIndex) {
    switch (portIndex) {
        case 0:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case 1:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case 2:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        case 3:
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
        case 4:
            __HAL_RCC_GPIOE_CLK_ENABLE();
            break;
        case 5:
            __HAL_RCC_GPIOF_CLK_ENABLE();
            break;
        case 6:
            __HAL_RCC_GPIOG_CLK_ENABLE();
            break;
    }
}

GPIO::GPIO(PORT_INDEX portIndex, uint16_t indexPin, uint32_t p_mode, uint32_t p_speed,uint32_t p_Alternate) {
    Initialize(portIndex,Port[portIndex],(uint16_t)1<<indexPin,p_mode,p_speed,p_Alternate);
}

GPIO::GPIO(PORT_INDEX p_periph, GPIO_TypeDef *p_port, uint16_t p_pins, uint32_t p_mode, uint32_t p_speed) {
    Initialize(p_periph,Port[p_periph],p_pins,p_mode,p_speed);
}

void GPIO::Initialize(uint32_t p_mode) {
    if (m_gpio.mode==p_mode)
        return;
    Initialize(m_gpio.PinGroup,m_gpio.port,m_gpio.pin,p_mode,m_gpio.speed);
    m_gpio.mode=p_mode;
}

void GPIO::Initialize(PORT_INDEX p_periph, GPIO_TypeDef *p_port, uint16_t p_pins, uint32_t p_mode, uint32_t p_speed,uint32_t p_Alternate) {
    m_gpio.PinGroup=p_periph;
    m_gpio.port=p_port;
    m_gpio.pin=p_pins;
    m_gpio.mode=p_mode;
    m_gpio.speed=p_speed;

    GPIO_InitTypeDef GPIO_InitStruct={0};
    GPIO_InitStruct.Pin= m_gpio.pin;
    GPIO_InitStruct.Mode=m_gpio.mode;
    GPIO_InitStruct.Speed=m_gpio.speed;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    RCCEnable(m_gpio.PinGroup);
    if (p_Alternate!=0){
        m_gpio.alternate=p_Alternate;
        GPIO_InitStruct.Alternate=p_Alternate;
    }
    HAL_GPIO_Init(m_gpio.port,&GPIO_InitStruct);
    High();
}

void GPIO::InitializeAF(uint32_t p_Alternate) {
    if (m_gpio.alternate==p_Alternate)
        return;
    Initialize(m_gpio.PinGroup,m_gpio.port,m_gpio.pin,m_gpio.mode,m_gpio.speed,m_gpio.alternate);
    m_gpio.alternate=p_Alternate;
}
