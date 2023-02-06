/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2021-06-25
 *
 * @copyright Copyright (c) 2021
 *
 */
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
#include "main.h"
#include "cw32f030_crc.h"

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


/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

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
 ** 计算CRC16 / CRC32
 **
 ******************************************************************************/

//=========================================================================

uint32_t tmp1, tmp2, tmp3;

//=========================================================================

int32_t main(void)
{
    uint8_t  Raw08[8] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 };
    uint16_t Raw16[4] = { 0x1100, 0x3322, 0x5544, 0x7766 };
    uint32_t Raw32[2] = { 0x33221100, 0x77665544 };

    //=========================================================================
    //enable CRC CLK
    CW_SYSCTRL->AHBEN_f.CRC = 1;

    //=========================================================================
    //CRC16_IBM 采用三种位宽计算CRC16值，其结果应相同
    tmp1 = CRC16_Calc_8bit(  CRC16_IBM, Raw08, 8 );      //按Byte计算CRC16
    tmp2 = CRC16_Calc_16bit( CRC16_IBM, Raw16, 4 );      //按HalfWord计算CRC16
    tmp3 = CRC16_Calc_32bit( CRC16_IBM, Raw32, 2 );      //按Word计算CRC16
    __asm( "BKPT  0" ); //请在此处观察tmp1/tmp2/tmp3的值是否相等



    //=========================================================================
    //CRC16_MAXIM 采用三种位宽计算CRC16值，其结果应相同
    tmp1 = CRC16_Calc_8bit(  CRC16_MAXIM, Raw08, 8 );      //按Byte计算CRC16
    tmp2 = CRC16_Calc_16bit( CRC16_MAXIM, Raw16, 4 );      //按HalfWord计算CRC16
    tmp3 = CRC16_Calc_32bit( CRC16_MAXIM, Raw32, 2 );      //按Word计算CRC16
    __asm( "BKPT  0" );  //请在此处观察tmp1/tmp2/tmp3的值是否相等


    //=========================================================================
    //CRC16_USB 采用三种位宽计算CRC16值，其结果应相同
    tmp1 = CRC16_Calc_8bit(  CRC16_USB, Raw08, 8 );      //按Byte计算CRC16
    tmp2 = CRC16_Calc_16bit( CRC16_USB, Raw16, 4 );      //按HalfWord计算CRC16
    tmp3 = CRC16_Calc_32bit( CRC16_USB, Raw32, 2 );      //按Word计算CRC16
    __asm( "BKPT  0" );  //请在此处观察tmp1/tmp2/tmp3的值是否相等


    //=========================================================================
    //CRC16_MODBUS 采用三种位宽计算CRC16值，其结果应相同
    tmp1 = CRC16_Calc_8bit(  CRC16_MODBUS, Raw08, 8 );      //按Byte计算CRC16
    tmp2 = CRC16_Calc_16bit( CRC16_MODBUS, Raw16, 4 );      //按HalfWord计算CRC16
    tmp3 = CRC16_Calc_32bit( CRC16_MODBUS, Raw32, 2 );      //按Word计算CRC16
    __asm( "BKPT  0" );  //请在此处观察tmp1/tmp2/tmp3的值是否相等


    //=========================================================================
    //CRC16_CCITT 采用三种位宽计算CRC16值，其结果应相同
    tmp1 = CRC16_Calc_8bit(  CRC16_CCITT, Raw08, 8 );      //按Byte计算CRC16
    tmp2 = CRC16_Calc_16bit( CRC16_CCITT, Raw16, 4 );      //按HalfWord计算CRC16
    tmp3 = CRC16_Calc_32bit( CRC16_CCITT, Raw32, 2 );      //按Word计算CRC16
    __asm( "BKPT  0" );  //请在此处观察tmp1/tmp2/tmp3的值是否相等


    //=========================================================================
    //CRC16_CCITTFALSE 采用三种位宽计算CRC16值，其结果应相同
    tmp1 = CRC16_Calc_8bit(  CRC16_CCITTFALSE, Raw08, 8 );      //按Byte计算CRC16
    tmp2 = CRC16_Calc_16bit( CRC16_CCITTFALSE, Raw16, 4 );      //按HalfWord计算CRC16
    tmp3 = CRC16_Calc_32bit( CRC16_CCITTFALSE, Raw32, 2 );      //按Word计算CRC16
    __asm( "BKPT  0" );  //请在此处观察tmp1/tmp2/tmp3的值是否相等


    //=========================================================================
    //CRC16_X25 采用三种位宽计算CRC16值，其结果应相同
    tmp1 = CRC16_Calc_8bit(  CRC16_X25, Raw08, 8 );      //按Byte计算CRC16
    tmp2 = CRC16_Calc_16bit( CRC16_X25, Raw16, 4 );      //按HalfWord计算CRC16
    tmp3 = CRC16_Calc_32bit( CRC16_X25, Raw32, 2 );      //按Word计算CRC16
    __asm( "BKPT  0" );  //请在此处观察tmp1/tmp2/tmp3的值是否相等


    //=========================================================================
    //CRC16_XMODEM 采用三种位宽计算CRC16值，其结果应相同
    tmp1 = CRC16_Calc_8bit(  CRC16_XMODEM, Raw08, 8 );      //按Byte计算CRC16
    tmp2 = CRC16_Calc_16bit( CRC16_XMODEM, Raw16, 4 );      //按HalfWord计算CRC16
    tmp3 = CRC16_Calc_32bit( CRC16_XMODEM, Raw32, 2 );      //按Word计算CRC16
    __asm( "BKPT  0" );  //请在此处观察tmp1/tmp2/tmp3的值是否相等


    //=========================================================================
    //CRC32_DEFAULT 采用三种位宽计算CRC32值，其结果应相同
    tmp1 = CRC32_Calc_8bit(  CRC32_DEFAULT, Raw08, 8 );      //按Byte计算CRC32
    tmp2 = CRC32_Calc_16bit( CRC32_DEFAULT, Raw16, 4 );      //按HalfWord计算CRC32
    tmp3 = CRC32_Calc_32bit( CRC32_DEFAULT, Raw32, 2 );      //按Word计算CRC32
    __asm( "BKPT  0" );  //请在此处观察tmp1/tmp2/tmp3的值是否相等


    //=========================================================================
    //CRC32_MPEG2 采用三种位宽计算CRC32值，其结果应相同
    tmp1 = CRC32_Calc_8bit(  CRC32_MPEG2, Raw08, 8 );      //按Byte计算CRC32
    tmp2 = CRC32_Calc_16bit( CRC32_MPEG2, Raw16, 4 );      //按HalfWord计算CRC32
    tmp3 = CRC32_Calc_32bit( CRC32_MPEG2, Raw32, 2 );      //按Word计算CRC32
    __asm( "BKPT  0" );  //请在此处观察tmp1/tmp2/tmp3的值是否相等

    while( 1 )
    {
        ;
    }
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
  * @return None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

