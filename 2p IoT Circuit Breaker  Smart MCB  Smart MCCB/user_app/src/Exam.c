//   HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);
//   TS_DELAY(50); 
  /*##-2- Start the transmission process #####################################*/  
  /* While the UART in reception process, user can transmit data through 
     "aTxBuffer" buffer */
//  if(HAL_UART_Transmit_IT(&huart1, (uint8_t*)aTxBuffer, TXBUFFERSIZE)!= HAL_OK)
//  {
//   Error_Handler();
//  }
  
//  while (UartReady != SET)
// {
//  }
  
  /* Reset transmission flag */
//  UartReady = RESET;
//Get_test();


//  Flash_PageErase(WORK_PARA_SECTOR_ADDR);
//  FlashWrite(WORK_PARA_SECTOR_ADDR,test,strlen((char *)test));
//  FlashRead(WORK_PARA_SECTOR_ADDR,temp,strlen((char *)test));


//        if((p_fifo->rpos==p_fifo->wpos) ||(p_fifo->pBuffer[p_fifo->rpos]==0x7E)){
//          p_fifo->rpos=0;
//          p_fifo->wpos=0;
//        }

//        buf[0] = FIFO_Get(&uartrecfifo);
//        buf[1] = FIFO_Get(&uartrecfifo);
//        FIFO_Read(&uartrecfifo, &buf[2], 4);
//        buf[6] = FIFO_Get(&uartrecfifo);
//        buf[7] = FIFO_Get(&uartrecfifo);
//        if (buf[7] > 0 && buf[7] < 100) {
//          FIFO_Read(&uartrecfifo, &buf[8], buf[7]);
//        }
//        len = buf[7] + 8;
//        FIFO_Read(&uartrecfifo, &buf[len], 3);
//        len += 3;

