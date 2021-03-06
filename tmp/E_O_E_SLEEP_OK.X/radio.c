#include "header.h"

uint8_t	status = 0;     //hold last status register (updated each time sending command)
int8_t rx_buffer[32];	//Buffer holding data to be send or get with a command
int8_t tx_buffer[32];	//Buffer holding data to be send or get with a command

int64_t test = 0;

void	radio_check_reg(void)
{
	int64_t reg = 0;
	
	reg = radio_read_reg(CONFIG_REG);
	reg = radio_read_reg(EN_AA_REG);
	reg = radio_read_reg(EN_RXADDR_REG);
	reg = radio_read_reg(SETUP_AW_REG);
	reg = radio_read_reg(SETUP_RETR_REG);
	reg = radio_read_reg(RF_CH_REG);
	reg = radio_read_reg(RF_SETUP_REG);
	reg = radio_read_reg(STATUS_REG);
	reg = radio_read_reg(OBSERVE_TX_REG);
	reg = radio_read_reg(CD_REG);
	reg = radio_read_reg(RX_ADDR_P0_REG);
	reg = radio_read_reg(RX_ADDR_P1_REG);
	reg = radio_read_reg(RX_ADDR_P2_REG);
	reg = radio_read_reg(RX_ADDR_P3_REG);
	reg = radio_read_reg(RX_ADDR_P4_REG);
	reg = radio_read_reg(RX_ADDR_P5_REG);
	reg = radio_read_reg(TX_ADDR_REG);
	reg = radio_read_reg(RX_PW_P0_REG);
	reg = radio_read_reg(RX_PW_P1_REG);
	reg = radio_read_reg(RX_PW_P2_REG);
	reg = radio_read_reg(RX_PW_P3_REG);
	reg = radio_read_reg(RX_PW_P4_REG);
	reg = radio_read_reg(RX_PW_P5_REG);
	reg = radio_read_reg(FIFO_STATUS_REG);
}

void	init_radio()
{	
	radio_write_reg(CONFIG_REG,	0x0B);              //CRC, PWR UP, PRX
	radio_write_reg(EN_RXADDR_REG,	0x03);          //Data pipe     1 and 0
	radio_write_reg(SETUP_AW_REG,	0x03);          //Bytes         5    
	radio_write_reg(SETUP_RETR_REG,	0x03);          //Retransmit    3
	radio_write_reg(RF_CH_REG,	0x00);              //Channel       0
	radio_write_reg(RF_SETUP_REG,	0x00);          //-18dBm, -1Mbps
//	radio_write_reg(STATUS_REG,	0x00);              //Status
//	radio_write_reg(OBSERVE_TX_REG,	0x00);          //Status
//	radio_write_reg(CD_REG,		0x00);              //Status
	radio_write_reg(RX_ADDR_P0_REG, 0x3232323232);	//Equal to TX_ADDR for enabled auto-ACK
	radio_write_reg(RX_ADDR_P1_REG, 0x3131313131);  //Receive address data pipe 1
	radio_write_reg(RX_ADDR_P2_REG, 0xc3);          //
	radio_write_reg(RX_ADDR_P3_REG, 0xc4);          //
	radio_write_reg(RX_ADDR_P4_REG, 0xc5);          //
	radio_write_reg(RX_ADDR_P5_REG, 0xc6);          //
	radio_write_reg(TX_ADDR_REG,	0x3232323232);  //Transmit address
	radio_write_reg(RX_PW_P0_REG,	0x02);          //Number of bytes   2
	radio_write_reg(RX_PW_P1_REG,	0x02);          //Number of bytes   2
	radio_write_reg(RX_PW_P2_REG,	0x00);          //
	radio_write_reg(RX_PW_P3_REG,	0x00);          //
	radio_write_reg(RX_PW_P4_REG,	0x00);          //
	radio_write_reg(RX_PW_P5_REG,	0x00);          //
//	radio_write_reg(FIFO_STATUS_REG,0x11);          //Status
	radio_write_reg(STATUS_REG,0x70);               //CLR interrupt flags
//	radio_write_reg(FEATURE_REG, 0x00);             //disable all features
//	radio_write_reg(DYNPD_REG, 0x00);               //disable
	radio_flush_tx();                               //Flush Tx pipe
	radio_flush_rx();                               //Flush Rx pipe

//  radio_check_reg();                              //Verify all registers    
}

int64_t	radio_command(int8_t command, int64_t data, int8_t data_len)
{
	int32_t		count;
	int64_t		ret = 0;
	
	LATBCLR = CSN_PIN;                              //CSN Active LOW - nrf listen
	status = spi_transfer(command);					//Send command && Get Status 
	for(count = 0; count < data_len * 8; count += 8)//LSB to MSB
	{
		ret += (int64_t)spi_transfer(data >> count) << count;	//Send or Get Data by triggering the SCK
	}
	LATBSET = CSN_PIN;                              //CSN HIGH - End of transaction
	return(ret);
}

/** COMMANDS **/
int64_t	radio_read_reg(int8_t reg_addr)
{
	int64_t	ret = 0;
	
	if (reg_addr == RX_ADDR_P0_REG | reg_addr == RX_ADDR_P1_REG | reg_addr == TX_ADDR_REG)	//5 bytes data
	{
		ret = radio_command(R_REGISTER(reg_addr), 0x00ll, 5);
	}
	else                                                                                    //1 byte data
	{
		ret = radio_command(R_REGISTER(reg_addr), 0x00ll, 1);
	}
	return(ret);
}

void	radio_write_reg(int8_t reg_addr, int64_t data)
{
	if (reg_addr == RX_ADDR_P0_REG | reg_addr == RX_ADDR_P1_REG | reg_addr == TX_ADDR_REG)  //5 bytes data
	{
		radio_command(W_REGISTER(reg_addr), data, 5);
	}
	else                                                                                    //1 byte
	{
		radio_command(W_REGISTER(reg_addr), data, 1);
	}
}

void	radio_read_payload()
{
	radio_command(R_RX_PAYLOAD, 0x00ll, 32);
}

void	radio_write_payload()
{
	radio_command(W_TX_PAYLOAD, 0x00ll, 32);
}

void	radio_flush_tx()
{
	radio_command(FLUSH_TX, 0x00ll, 0);
}

void	radio_flush_rx()
{
	radio_command(FLUSH_RX, 0x00ll, 0);
}

void	radio_reuse_payload()
{
	radio_command(REUSE_TX_PL, 0x00ll, 0);
}

void	radio_activate()
{
	radio_command(ACTIVATE, 0x00ll, 1);
}

void	radio_read_payload_wid()
{
	radio_command(R_RX_PL_WID, 0x00ll, 1);
}

void	radio_w_ack_payload(int8_t pipe)
{
	radio_command(W_ACK_PAYLOAD(pipe), 0x00ll, 32);
}

void	radio_write_payload_no_ack()
{
	radio_command(W_TX_PAYLOAD_NO_ACK, 0x00ll, 32);
}

void	radio_nop()
{
	radio_command(NOP, 0x00ll, 0);
}

void    radio_tx_mode()
{
	radio_write_reg(CONFIG_REG, 0x0a);	//PWR_UP = 1, PRIM_RX = 0
}

void    radio_rx_mode()
{
    radio_write_reg(CONFIG_REG, 0x0b);  //PWR_UP = 1,   PRIM_RX = 1
}

void radio_ce_pulse(void)
{
	//CE_PIN HIGH (10us pulse)
	LATBSET = CE_PIN;
	delay_micro(10);
	LATBCLR = CE_PIN;	
}

void	radio_send(int32_t payload, int8_t len)
{
        radio_tx_mode();
	//payload in TX FIFO
	radio_command(W_TX_PAYLOAD, payload, len);      //Write in TX_PAYLOAD
	radio_ce_pulse();                               //Trigger CE for sending
	radio_write_reg(STATUS_REG, 0x70);              //Clear Status
}

int32_t	radio_receive(void)
{
	int32_t	ret = 0;

	radio_rx_mode();
	LATBSET = CE_PIN;                               //CE HIGH - Enable reception
	while((status & 0x40) == 0x00)                  //Wait for RX_PAYLOAD
		radio_nop();
    	LATBCLR = CE_PIN;                               //CE LOW - Disable reception
	ret = radio_command(R_RX_PAYLOAD, 0x00ll, 8);   //Get Data from RX_PAYLOAD
	radio_write_reg(STATUS_REG, 0x40);              //Clear Status
	return(ret);
}

/** TESTS **/

void	spi_test(void)                              //Simple Test for SPI - Working
{
	int64_t val = 0;
    
	int reg = RX_ADDR_P0_REG;
	val = radio_read_reg(reg);
	radio_write_reg(reg, 0xeeddccbbaa);
	val = radio_read_reg(reg);
}

void		radio_test(void)                        //Simple Test for TX/RX - [2/2]
{
//      int32_t val = 0;

 	radio_send(0x2a2a, 2);                          //send 42 255 
//	val = radio_receive();                          //
//	display_write_dec(val, 0, 0);                   //
}
