/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MyKey.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/31 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_MYKEY_H
#define MY_LIBARARY_MYKEY_H
#include "main.h"
#include "MyGPIO.h"
#include "vector"
#include "map"
#define KEY0_PRES    1              /* KEY0按下 */
#define KEY1_PRES    2              /* KEY1按下 */
#define KEY2_PRES    3              /* KEY2按下 */
#define WKUP_PRES    4              /* KEY_UP按下(即WK_UP) */
#define KEY0_GPIO_PORT                  GPIOE
#define KEY0_GPIO_PIN                   GPIO_PIN_4
#define KEY0_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */

#define KEY1_GPIO_PORT                  GPIOE
#define KEY1_GPIO_PIN                   GPIO_PIN_3
#define KEY1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */

#define KEY2_GPIO_PORT                  GPIOE
#define KEY2_GPIO_PIN                   GPIO_PIN_2
#define KEY2_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */

#define WKUP_GPIO_PORT                  GPIOA
#define WKUP_GPIO_PIN                   GPIO_PIN_0
#define WKUP_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

/******************************************************************************************/

#define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN)     /* 读取KEY0引脚 */
#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN)     /* 读取KEY1引脚 */
#define KEY2        HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN)     /* 读取KEY2引脚 */
#define WK_UP       HAL_GPIO_ReadPin(WKUP_GPIO_PORT, WKUP_GPIO_PIN)     /* 读取WKUP引脚 */
#define TICKS_INTERVAL    5	//ms
#define DEBOUNCE_TICKS    3	//MAX 7 (0 ~ 7)
#define SHORT_TICKS       (300 /TICKS_INTERVAL)
#define LONG_TICKS        (1000 /TICKS_INTERVAL)
using namespace stm32f407;
using namespace std;
namespace stm32f407 {

    typedef enum {
        PRESS_DOWN = 0,
        PRESS_UP,
        PRESS_REPEAT,
        SINGLE_CLICK,
        DOUBLE_CLICK,
        LONG_PRESS_START,
        LONG_PRESS_HOLD,
        number_of_event,
        NONE_PRESS
    }PressEvent;
    class MyButton;

    class ButtonManage {
    public:

        void button_handler();
        void buttonTick();
        uint8_t TaskSelect(uint8_t i,PressEvent Event);


    protected:
        uint16_t time=0;

    };

    typedef void (*BtnCallBack)(void);
    class MyButton:public ButtonManage{
    private:


        GPIO KeyPin;

    public:
        MyButton();
        MyButton(uint16_t );
        uint8_t Ready;
        uint16_t ticks;
        uint8_t  repeat : 4;
        uint8_t  event : 4;
        uint8_t  state : 3;
        uint8_t  debounce_cnt : 3;
        uint8_t  active_level : 1;
        uint8_t  button_id;
        bool  button_level : 1;
        // uint8_t  (*hal_button_Level)(uint8_t button_id_);
        BtnCallBack taskFunction[number_of_event];
        MyButton(PORT_INDEX portIndex,uint16_t indexPin,uint8_t ButtonId,uint8_t activeLeve=0);

        void MyButtonTaskRegister(PressEvent event,BtnCallBack taskFunction);
        void MyButtonEnable();
        void MyButtonDisEnable();
        PressEvent GetNowEvent();
        uint8_t key_scan(uint8_t mode);


        GPIO *GetKeyPin();
        ~MyButton();
    };
}
extern vector<MyButton *> SaveButtonId;
//extern  map<uint8_t,MyButton *> RunButton;
extern ButtonManage buttonManage;
#endif //MY_LIBARARY_MYKEY_H
