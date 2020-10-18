#ifndef __W25QXX_H
#define __W25QXX_H

#include "stm32f1xx.h"
#include "spi.h"

/** 
  * @brief  W25Q128FV Configuration  
  */  

/* This Value should be configured */
#define W25QXX_FLASH_SIZE                  0x1000000 /* 128 MBits => 16 MBytes */

#define W25QXX_SECTOR_SIZE                 0x10000   /* 64 KBytes */
#define W25QXX_SUBSECTOR_SIZE              0x1000    /* 4 KBytes */
#define W25QXX_PAGE_SIZE                   0x100     /* 256 Bytes */

#define W25QXX_DUMMY_CYCLES_READ           4
#define W25QXX_DUMMY_CYCLES_READ_QUAD      10

#define W25QXX_BULK_ERASE_MAX_TIME         250000
#define W25QXX_SECTOR_ERASE_MAX_TIME       3000
#define W25QXX_SUBSECTOR_ERASE_MAX_TIME    800
#define W25Qx_TIMEOUT_VALUE 1000

/* HAL_Lib Configuration */
#define W25Qx_Enable() 			HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET)
#define W25Qx_Disable() 		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET)
#define W25Qxx_SPI_Port		                    hspi1

/** 
  * @brief  W25Q128FV Commands  
  */  
/* Reset Operations */
#define RESET_ENABLE_CMD                      0x66
#define RESET_MEMORY_CMD                      0x99

#define ENTER_QPI_MODE_CMD                    0x38
#define EXIT_QPI_MODE_CMD                     0xFF

/* Identification Operations */
#define READ_ID_CMD                           0x90
#define DUAL_READ_ID_CMD                      0x92
#define QUAD_READ_ID_CMD                      0x94
#define READ_JEDEC_ID_CMD                     0x9F

/* Read Operations */
#define READ_CMD                              0x03
#define FAST_READ_CMD                         0x0B
#define DUAL_OUT_FAST_READ_CMD                0x3B
#define DUAL_INOUT_FAST_READ_CMD              0xBB
#define QUAD_OUT_FAST_READ_CMD                0x6B
#define QUAD_INOUT_FAST_READ_CMD              0xEB

/* Write Operations */
#define WRITE_ENABLE_CMD                      0x06
#define WRITE_DISABLE_CMD                     0x04

/* Register Operations */
#define READ_STATUS_REG1_CMD                  0x05
#define READ_STATUS_REG2_CMD                  0x35
#define READ_STATUS_REG3_CMD                  0x15

#define WRITE_STATUS_REG1_CMD                 0x01
#define WRITE_STATUS_REG2_CMD                 0x31
#define WRITE_STATUS_REG3_CMD                 0x11


/* Program Operations */
#define PAGE_PROG_CMD                         0x02
#define QUAD_INPUT_PAGE_PROG_CMD              0x32


/* Erase Operations */
#define SECTOR_ERASE_CMD                      0x20
#define CHIP_ERASE_CMD                        0xC7

#define PROG_ERASE_RESUME_CMD                 0x7A
#define PROG_ERASE_SUSPEND_CMD                0x75

/* Flag Status Register */
#define W25QXX_FSR_BUSY                     ((uint8_t)0x01)    /*!< busy */
#define W25QXX_FSR_WREN                     ((uint8_t)0x02)    /*!< write enable */
#define W25QXX_FSR_QE                       ((uint8_t)0x02)    /*!< quad enable */

/* W25Qxx Status */
#define W25Qx_OK                              ((uint8_t)0x00)
#define W25Qx_ERROR                           ((uint8_t)0x01)
#define W25Qx_BUSY                            ((uint8_t)0x02)
#define W25Qx_TIMEOUT				                  ((uint8_t)0x03)

/* Functions */

/**
  * @brief  Initializes the W25Q128FV interface.
  * @retval None
  */
uint8_t BSP_W25Qx_Init(void);

/**
  * @brief  This function reset the W25Qx.
  * @retval None
  */
static void	BSP_W25Qx_Reset(void);

/**
  * @brief  Reads current status of the W25Q128FV.
  * @retval W25Q128FV memory status
  */
static uint8_t BSP_W25Qx_GetStatus(void);

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @retval None
  */
uint8_t BSP_W25Qx_WriteEnable(void);

/**
  * @brief  Read Manufacture/Device ID.
	* @param  return value address
  * @retval None
  */
void BSP_W25Qx_Read_ID(uint8_t *ID);

/**
  * @brief  Reads an amount of data from the QSPI memory.
  * @param  pData: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read    
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);


/**
  * @brief  Writes an amount of data to the QSPI memory.
  * @param  pData: Pointer to data to be written
  * @param  WriteAddr: Write start address
  * @param  Size: Size of data to write,No more than 256byte.    
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);

/**
  * @brief  Erases the specified block of the QSPI memory. 
  * @param  BlockAddress: Block address to erase  
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Erase_Block(uint32_t Address);

/**
  * @brief  Erases the entire QSPI memory.This function will take a very long time.
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Erase_Chip(void);

#endif /* __W25Qx_H */
