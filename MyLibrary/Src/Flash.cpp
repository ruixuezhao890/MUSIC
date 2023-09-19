/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : Flash.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/26 
*********************************************************************
*********
*/
//

#include "Flash.h"

#ifdef HAL_SPI_ERROR_NONE
Flash::Flash(SPI_HandleTypeDef *SPISelect, GPIO *CS) : MySPI(SPISelect, CS) {
    norflash_init();
//    this->SetSelectMode(CMD);
}
#endif
void Flash::norflash_init(void) {
    uint8_t temp;
    this->SoftSPIModeSet(SPIMode3);
    g_norflash_type=norflash_read_id();
    if (g_norflash_type==W25Q256){
        temp = norflash_read_sr(3);
        if ((temp & 0X01) == 0)             /* 如果不是4字节地址模式,则进入4字节地址模式 */
        {
            norflash_write_enable();        /* 写使能 */
            temp |= 1 << 1;                 /* ADP=1, 上电4位地址模式 */
            norflash_write_sr(3, temp);     /* 写SR3 */

            print(FLASH_Enable4ByteAddr,CMD);

//            NORFLASH_CS(0);
//            spi1_read_write_byte(FLASH_Enable4ByteAddr);    /* 使能4字节地址指令 */
//            NORFLASH_CS(1);
        }
    }

}

uint16_t Flash::norflash_read_id(void) {
    uint16_t deviceId=0;
    uint8_t temp;
    SetCS(0);
    print(FLASH_ManufactDeviceID,CMD);
    for (int i = 0; i < 3; ++i) {
        print(0);
    }
    scan(temp,CMD);deviceId=temp<<8;
    scan(temp,CMD);deviceId|=temp;
    SetCS(1);
    return deviceId;

}

uint8_t Flash::norflash_read_sr(uint8_t regno) {
    uint8_t byte = 0, command = 0;

    switch (regno)
    {
        case 1:
            command = FLASH_ReadStatusReg1;  /* 读状态寄存器1指令 */
            break;

        case 2:
            command = FLASH_ReadStatusReg2;  /* 读状态寄存器2指令 */
            break;

        case 3:
            command = FLASH_ReadStatusReg3;  /* 读状态寄存器3指令 */
            break;

        default:
            command = FLASH_ReadStatusReg1;
            break;
    }

    SetCS(0);
    print(command,CMD);      /* 发送读寄存器命令 */
    scan(byte,CMD);         /* 读取一个字节 */
    SetCS(1);

    return byte;
}

void Flash::norflash_write_enable(void) {
    SetCS(0);
    print(FLASH_WriteEnable,CMD);
    SetCS(1);
}

void Flash::norflash_write_sr(uint8_t regno, uint8_t sr) {
    uint8_t command = 0;

    switch (regno)
    {
        case 1:
            command = FLASH_WriteStatusReg1;  /* 写状态寄存器1指令 */
            break;

        case 2:
            command = FLASH_WriteStatusReg2;  /* 写状态寄存器2指令 */
            break;

        case 3:
            command = FLASH_WriteStatusReg3;  /* 写状态寄存器3指令 */
            break;

        default:
            command = FLASH_WriteStatusReg1;
            break;
    }

    SetCS(0);
    print(command,CMD);  /* 发送读寄存器命令 */
    print(sr,CMD);       /* 写入一个字节 */
    SetCS(1);
}

void Flash::norflash_wait_busy(void) {
    while ((norflash_read_sr(1) & 0x01) == 0x01);
}

void Flash::norflash_send_address(uint32_t address) {
    if (g_norflash_type == W25Q256)                     /* 只有W25Q256支持4字节地址模式 */
    {
        print((uint8_t)((address)>>24),CMD); /* 发送 bit31 ~ bit24 地址 */
    }
    print((uint8_t)((address)>>16),CMD);     /* 发送 bit23 ~ bit16 地址 */
    print((uint8_t)((address)>>8),CMD);      /* 发送 bit15 ~ bit8  地址 */
    print((uint8_t)address,CMD);             /* 发送 bit7  ~ bit0  地址 */
}

Flash::Flash() {

}

Flash::Flash(GPIO *CS, GPIO *SCK, GPIO *MISO, GPIO *MOSI) : MySPI(CS, SCK, MISO, MOSI){
    norflash_init();
}

void Flash::norflash_write_page(char *pbuf, uint32_t addr, uint16_t datalen) {
    uint16_t i;

    norflash_write_enable();                    /* 写使能 */
    SetCS(0);
    print(FLASH_PageProgram,CMD);    /* 发送写页命令 */
    norflash_send_address(addr);                /* 发送地址 */

    for (i = 0; i < datalen; i++)
    {
        print(pbuf[i]);          /* 循环读取 */
    }

    SetCS(1);
    norflash_wait_busy();       /* 等待写入结束 */
}

void Flash::norflash_write_nocheck(char *pbuf, uint32_t addr, uint16_t datalen) {
    uint16_t pageremain;
    pageremain = 256 - addr % 256;  /* 单页剩余的字节数 */

    if (datalen <= pageremain)      /* 不大于256个字节 */
    {
        pageremain = datalen;
    }

    while (1)
    {
        /* 当写入字节比页内剩余地址还少的时候, 一次性写完
         * 当写入直接比页内剩余地址还多的时候, 先写完整个页内剩余地址, 然后根据剩余长度进行不同处理
         */
        norflash_write_page(pbuf, addr, pageremain);

        if (datalen == pageremain)      /* 写入结束了 */
        {
            break;
        }
        else                            /* datalen > pageremain */
        {
            pbuf += pageremain;         /* pbuf指针地址偏移,前面已经写了pageremain字节 */
            addr += pageremain;         /* 写地址偏移,前面已经写了pageremain字节 */
            datalen -= pageremain;      /* 写入总长度减去已经写入了的字节数 */

            if (datalen > 256)          /* 剩余数据还大于一页,可以一次写一页 */
            {
                pageremain = 256;       /* 一次可以写入256个字节 */
            }
            else                        /* 剩余数据小于一页,可以一次写完 */
            {
                pageremain = datalen;   /* 不够256个字节了 */
            }
        }
    }
}

void Flash::norflash_write(char *pbuf, uint32_t addr, uint16_t datalen) {
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    char *norflash_buf;

    norflash_buf = g_norflash_buf;
    secpos = addr / 4096;       /* 扇区地址 */
    secoff = addr % 4096;       /* 在扇区内的偏移 */
    secremain = 4096 - secoff;  /* 扇区剩余空间大小 */

    //printf("ad:%X,nb:%X\r\n", addr, datalen); /* 测试用 */
    if (datalen <= secremain)
    {
        secremain = datalen;    /* 不大于4096个字节 */
    }

    while (1)
    {
        norflash_read(norflash_buf, secpos * 4096, 4096);   /* 读出整个扇区的内容 */

        for (i = 0; i < secremain; i++)     /* 校验数据 */
        {
            if (norflash_buf[secoff + i] != 0XFF)
            {
                break;                      /* 需要擦除, 直接退出for循环 */
            }
        }

        if (i < secremain)                  /* 需要擦除 */
        {
            norflash_erase_sector(secpos);  /* 擦除这个扇区 */

            for (i = 0; i < secremain; i++) /* 复制 */
            {
                norflash_buf[i + secoff] = pbuf[i];
            }

            norflash_write_nocheck(norflash_buf, secpos * 4096, 4096);  /* 写入整个扇区 */
        }
        else    /* 写已经擦除了的,直接写入扇区剩余区间. */
        {
            norflash_write_nocheck(pbuf, addr, secremain);              /* 直接写扇区 */
        }

        if (datalen == secremain)
        {
            break;  /* 写入结束了 */
        }
        else        /* 写入未结束 */
        {
            secpos++;               /* 扇区地址增1 */
            secoff = 0;             /* 偏移位置为0 */

            pbuf += secremain;      /* 指针偏移 */
            addr += secremain;      /* 写地址偏移 */
            datalen -= secremain;   /* 字节数递减 */

            if (datalen > 4096)
            {
                secremain = 4096;   /* 下一个扇区还是写不完 */
            }
            else
            {
                secremain = datalen;/* 下一个扇区可以写完了 */
            }
        }
    }
}

void Flash::norflash_erase_chip(void) {
    norflash_write_enable();    /* 写使能 */
    norflash_wait_busy();       /* 等待空闲 */
    SetCS(0);
    print(FLASH_ChipErase,CMD);  /* 发送读寄存器命令 */
    SetCS(1);
    norflash_wait_busy();       /* 等待芯片擦除结束 */
}

void Flash::norflash_erase_sector(uint32_t saddr) {
    saddr *= 4096;
    norflash_write_enable();        /* 写使能 */
    norflash_wait_busy();           /* 等待空闲 */

   SetCS(0);
    print(FLASH_SectorErase,CMD);    /* 发送写页命令 */
    norflash_send_address(saddr);   /* 发送地址 */
    SetCS(1);
    norflash_wait_busy();           /* 等待扇区擦除完成 */
}

void Flash::norflash_read(char *pbuf, uint32_t addr, uint16_t datalen) {
    uint16_t i;

    SetCS(0);
    print(FLASH_ReadData,CMD);       /* 发送读取命令 */
    norflash_send_address(addr);                /* 发送地址 */
//    scan(pbuf);
    for (i = 0; i < datalen; i++)
    {
        scan(pbuf[i]);   /* 循环读取 */
    }

    SetCS(1);
}
