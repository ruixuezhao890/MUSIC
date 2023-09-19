/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : Flash.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/26 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_FLASH_H
#define MY_LIBARARY_FLASH_H
#include "main.h"
#include "MySPI.h"
#define W25Q80      0XEF13          /* W25Q80   Ð¾Æ¬ID */
#define W25Q16      0XEF14          /* W25Q16   Ð¾Æ¬ID */
#define W25Q32      0XEF15          /* W25Q32   Ð¾Æ¬ID */
#define W25Q64      0XEF16          /* W25Q64   Ð¾Æ¬ID */
#define W25Q128     0XEF17          /* W25Q128  Ð¾Æ¬ID */
#define W25Q256     0XEF18          /* W25Q256  Ð¾Æ¬ID */
#define BY25Q64     0X6816          /* BY25Q64  Ð¾Æ¬ID */
#define BY25Q128    0X6817          /* BY25Q128 Ð¾Æ¬ID */
#define NM25Q64     0X5216          /* NM25Q64  Ð¾Æ¬ID */
#define NM25Q128    0X5217          /* NM25Q128 Ð¾Æ¬ID */

/* Ö¸Áî±í */
#define FLASH_WriteEnable           0x06
#define FLASH_WriteDisable          0x04
#define FLASH_ReadStatusReg1        0x05
#define FLASH_ReadStatusReg2        0x35
#define FLASH_ReadStatusReg3        0x15
#define FLASH_WriteStatusReg1       0x01
#define FLASH_WriteStatusReg2       0x31
#define FLASH_WriteStatusReg3       0x11
#define FLASH_ReadData              0x03
#define FLASH_FastReadData          0x0B
#define FLASH_FastReadDual          0x3B
#define FLASH_FastReadQuad          0xEB
#define FLASH_PageProgram           0x02
#define FLASH_PageProgramQuad       0x32
#define FLASH_BlockErase            0xD8
#define FLASH_SectorErase           0x20
#define FLASH_ChipErase             0xC7
#define FLASH_PowerDown             0xB9
#define FLASH_ReleasePowerDown      0xAB
#define FLASH_DeviceID              0xAB
#define FLASH_ManufactDeviceID      0x90
#define FLASH_JedecDeviceID         0x9F
#define FLASH_Enable4ByteAddr       0xB7
#define FLASH_Exit4ByteAddr         0xE9
#define FLASH_SetReadParam          0xC0
#define FLASH_EnterQPIMode          0x38
#define FLASH_ExitQPIMode           0xFF
class Flash:public MySPI{
public:
    Flash();
#ifdef HAL_SPI_ERROR_NONE
    Flash(SPI_HandleTypeDef* SPISelect,GPIO *CS);
#endif
    Flash(GPIO* CS,GPIO* SCK,GPIO * MISO ,GPIO * MOSI);
    void norflash_wait_busy(void);               /* µÈ´ý¿ÕÏÐ */
    void norflash_send_address(uint32_t address);/* ·¢ËÍµØÖ· */
    void norflash_write_page(char *pbuf, uint32_t addr, uint16_t datalen);    /* Ð´Èëpage */
    void norflash_write_nocheck(char *pbuf, uint32_t addr, uint16_t datalen); /* Ð´flash,²»´ø²Á³ý */
    void norflash_init(void);                   /* ³õÊ¼»¯25QXX */
    uint16_t norflash_read_id(void);            /* ¶ÁÈ¡FLASH ID */
    void norflash_write_enable(void);           /* Ð´Ê¹ÄÜ */
    uint8_t norflash_read_sr(uint8_t regno);    /* ¶ÁÈ¡×´Ì¬¼Ä´æÆ÷ */
    void norflash_write_sr(uint8_t regno,uint8_t sr);   /* Ð´×´Ì¬¼Ä´æÆ÷ */

    void norflash_erase_chip(void);             /* ÕûÆ¬²Á³ý */
    void norflash_erase_sector(uint32_t saddr); /* ÉÈÇø²Á³ý */
    void norflash_read(char *pbuf, uint32_t addr, uint16_t datalen);     /* ¶ÁÈ¡flash */
    void norflash_write(char *pbuf, uint32_t addr, uint16_t datalen);    /* Ð´Èëflash */
private:
    uint16_t g_norflash_type = W25Q128;
   char g_norflash_buf[4096];   /* ÉÈÇø»º´æ */
};
#endif //MY_LIBARARY_FLASH_H
