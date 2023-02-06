/*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "../inc/main.h"


/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void RTC_Configuration(void);
void ADC_Configuration(void);
void Sleep_Configuration(void);
void delay(uint16_t ms);
void RTC_IRQHandlerCallBack(void);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint32_t flag=0;
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample toggle GPIOA.00
 **
 ******************************************************************************/

/**
 * @brief  RTC时钟测试，初始化日历，使用间隔中断0.5秒通过Log输出日期时间
 * @return int32_t
 */
int32_t main(void)
{
    uint32_t cnt;
    /* System Clocks Configuration */
    RCC_Configuration();

    /* GPIO Configuration */
    GPIO_Configuration();
    
    RTC_Configuration();
    ADC_Configuration();
    Sleep_Configuration();

    /* NVIC Configuration */
    NVIC_Configuration();
    while(1)
    {
        if (flag & bv0)
        {
            flag &= ~bv0;
            ADC_Enable();    // 低功耗模式下开启了ADC的自动关闭功能，再次使用ADC时，需要置EN位
            ADC_SoftwareStartConvCmd(ENABLE);
            while(!CW_ADC->ISR_f.EOC);
            for (cnt = 0; cnt < 5; cnt++)
            {
                PB08_TOG();
                PB09_TOG();
                delay(100);
            }       
        }
        if (flag & bv1)
        {
            flag &= ~bv1;
            PB09_SETHIGH();
            delay(500);            
        }
        if (flag & bv2)
        {
            flag &= ~bv2;
            PB08_SETHIGH();
            delay(500); 
        }
        PB08_SETLOW();
        PB09_SETLOW();
        PWR_GotoLpmMode();

    }
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    RCC_HSI_Enable(RCC_HSIOSC_DIV6);    // 8MHz主频
    RCC_LSI_Enable();    // 内部低速时钟作为RTC时钟源，精度较差
    // 如需要RTC走时精度高，需要使用外部高精度晶体作为时钟源
//    RCC_LSE_Enable(RCC_LSE_MODE_OSC, RCC_LSE_AMP_NORMAL, RCC_LSE_DRIVER_NORMAL);
    RCC_APBPeriphClk_Enable1(RCC_APB1_PERIPH_RTC, ENABLE);
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    // PA4 PB4唤醒
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    GPIO_InitStructure.IT = GPIO_IT_FALLING |GPIO_IT_RISING;    // 双沿触发中断
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;    // 输入上拉
    GPIO_InitStructure.Pins = GPIO_PIN_4;
    
    __RCC_GPIOA_CLK_ENABLE();    // 开GPIOA模块的时钟
    __RCC_GPIOB_CLK_ENABLE();    // 开GPIOB模块的时钟
    
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
    
    GPIO_ConfigFilter(CW_GPIOA, GPIO_PIN_4, GPIO_FLTCLK_LSI);    // 休眠时，中断唤醒通过LSI时钟去抖动。
    GPIO_ConfigFilter(CW_GPIOB, GPIO_PIN_4, GPIO_FLTCLK_LSI);
    
    CW_GPIOA->ICR = 0x0000;
    CW_GPIOB->ICR = 0x0000;    // 清中断标志
    
    GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pins = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
    
    PB08_SETLOW();
    PB09_SETHIGH();    
}

void RTC_Configuration(void)
{
    RTC_InitTypeDef RTC_InitStruct = {0};
    RTC_AlarmTypeDef RTC_AlarmStruct = {0};
    
    __RCC_RTC_CLK_ENABLE();    // 开RTC模块的时钟    
    
    RCC_LSI_Enable();          // 选择LSI为RTC时钟，在配置RTC时钟前，时钟源必须有效

    RTC_InitStruct.DateStruct.Day = 0x21;             //设置日期，DAY、MONTH、YEAR必须为BCD方式，星期为0~6，代表星期日，星期一至星期六
    RTC_InitStruct.DateStruct.Month = RTC_Month_July;
    RTC_InitStruct.DateStruct.Week = RTC_Weekday_Thursday;
    RTC_InitStruct.DateStruct.Year = 0x22;    // 2022-7-21    

    RTC_InitStruct.TimeStruct.Hour = 0x2;         //设置时间，HOUR、MINIUTE、SECOND必须为BCD方式，用户须保证HOUR、AMPM、H24之间的关联正确性
    RTC_InitStruct.TimeStruct.Minute = 0x58;
    RTC_InitStruct.TimeStruct.Second = 0x55;
    RTC_InitStruct.TimeStruct.AMPM = RTC_H12_PM;
    RTC_InitStruct.TimeStruct.H24 = 0;    //     12小时制 PM 2：58：55
    
    RTC_InitStruct.RTC_ClockSource = RTC_RTCCLK_FROM_LSI;
    RTC_Init(&RTC_InitStruct);    //  用户需选定需要使用的时钟源

    
    /* 设置闹钟在30秒闹一次 */
    RTC_AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_All & (~RTC_AlarmMask_Seconds);    // 和秒位匹配
    RTC_AlarmStruct.RTC_AlarmTime.Hour = 0x01;
    RTC_AlarmStruct.RTC_AlarmTime.Minute = 0x01;
    RTC_AlarmStruct.RTC_AlarmTime.Second = 0x30;
    RTC_SetAlarm(RTC_Alarm_A, &RTC_AlarmStruct);
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
   
    RTC_ITConfig(RTC_IT_ALARMA, ENABLE);
}

void ADC_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure ={0};
    ADC_WdtTypeDef ADC_WdtStructure = {0};
    ADC_SingleChTypeDef ADC_SingleChStructure = {0};
    
    __RCC_ADC_CLK_ENABLE();     // ADC模块时钟打开    

    //ADC默认值初始化
    ADC_StructInit(&ADC_InitStructure);
    //ADC工作时钟配置

    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div2;    // ADCCLK= 4MHz
    ADC_InitStructure.ADC_VrefSel = ADC_Vref_VDDA;


    //ADC模拟看门狗通道初始化
    ADC_WdtInit(&ADC_WdtStructure);

    //配置单通道转换模式
    ADC_SingleChStructure.ADC_DiscardEn = ADC_DiscardNull;
    ADC_SingleChStructure.ADC_Chmux = ADC_VddaDiV3Input;    //选择ADC转换通道 VDDA/3
    ADC_SingleChStructure.ADC_InitStruct = ADC_InitStructure;
    ADC_SingleChStructure.ADC_WdtStruct = ADC_WdtStructure;
    ADC_SingleChOneModeCfg(&ADC_SingleChStructure);
    
    ADC_AutoStop(ADC_AutoStopEnable);    // 低功耗，使能ADC的自动关闭模式
   
    ADC_ClearITPendingAll();

    //ADC使能
    ADC_Enable();    
    ADC_SoftwareStartConvCmd(ENABLE);
}

void Sleep_Configuration(void)
{
    PWR_InitTypeDef PWR_InitStructure;
    
    PWR_InitStructure.PWR_Sevonpend = PWR_Sevonpend_Disable;
    PWR_InitStructure.PWR_SleepDeep = PWR_SleepDeep_Enable;
    PWR_InitStructure.PWR_SleepOnExit = PWR_SleepOnExit_Disable;
    PWR_Config(&PWR_InitStructure);

}


/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(RTC_IRQn);
    NVIC_EnableIRQ(GPIOA_IRQn);
    NVIC_EnableIRQ(GPIOB_IRQn);
    __enable_irq();
}

void delay(uint16_t ms)
{
    uint32_t i;
    for( ; ms >0; ms--)
    {
        i = 796;
        while(i--);
    }
}



void RTC_IRQHandlerCallBack(void)
{
    if (RTC_GetITState(RTC_IT_ALARMA))
    {
        RTC_ClearITPendingBit(RTC_IT_ALARMA); 
        flag |= 1;        
    }    
}

void GPIOA_IRQHandlerCallBack()
{
    CW_GPIOA->ICR = 0x0000;
    flag |= 1<<1;
}
void GPIOB_IRQHandlerCallBack()
{
    CW_GPIOB->ICR = 0x0000;
    flag |= 1<<2;
}



/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
