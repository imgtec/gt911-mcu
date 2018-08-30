/**************************************************************
 * FileName:       car_touchpanel_tsc2003.c: 
 * Author:         aisubin  Version: 001  Date: 2007-11-28
 * Description:    polling_touch panel x&y coordinate ,
                   only four_line_resistense touch panel
 * Function List:  
 *                 1.
 * History:        
 *     <author>       <time>    <version >   <desc>
      chuanyue.li    2006-07-10             creating
      shenyj         2007-11-09             Updating
 **************************************************************/
 #define BUS_I2C_GPIO
//#define CONFIG_TP_TSC2003
#define CONFIG_TP_GT9XX
//#define CONFIG_TP_GT9XX_POLL_INT
#define CONFIG_TP_GT9XX_POLL_I2C
#define CONFIG_TP_GT9XX_FILTER
#if defined(CONFIG_TP_GT9XX_POLL_INT) && defined(CONFIG_TP_GT9XX_POLL_I2C)
#error "poll...."
#endif

#ifdef CONFIG_TP_GT9XX
char gt9xx_cfg[] = {
0x41,0x79,0x04,0x58,0x02,0x05,0x3C,0x00,
0x01,0x08,0x19,0x08,0x50,0x32,0x03,0x07,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,
0x1A,0x1E,0x14,0x8C,0x2E,0x0E,0x48,0x46,
0x46,0x08,0x00,0x00,0x00,0x00,0x03,0x1D,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x1E,0x3C,0x94,0xC5,0x02,
0x08,0x00,0x00,0x04,0x9A,0x2F,0x00,0x80,
0x3F,0x00,0x6A,0x55,0x00,0x61,0x73,0x00,
0x63,0x9C,0x00,0x6E,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,
0x12,0x14,0x16,0x18,0x1A,0x1C,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,
0x04,0x06,0x08,0x0A,0x0C,0x0F,0x10,0x12,
0x13,0x14,0x16,0x18,0x1C,0x1D,0x1E,0x1F,
0x20,0x21,0x22,0x24,0x26,0x28,0x29,0x2A,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x60,0x01 };
#endif

#if defined(CONFIG_TP_TSC2003) && defined(CONFIG_TP_GT9XX)
#error "God didn't allow"
#endif

//#include "i2c_general.h"
#include "touchpanel_tsc2003.h"
#include "touch_panel.h"
//#include "i2c.h"

#ifdef BUS_I2C_GPIO

#define Delay5Ms(i)    delay_1ms(5*i)

#define GT9xx_RSTB	61
#define GT9xx_INT	62
#define I2C_SCL     	63
#define I2C_SDA     	64

#define I2C_PULL_HIGH 
#define I2C_SCL_SET(d)  GPIO_O_SET(I2C_SCL, d)
#define I2C_SDA_SET(d)  GPIO_O_SET(I2C_SDA, d)
#define I2C_SDA_GET()   GPIO_I_GET(I2C_SDA)
#define I2C_SCL_GET()   GPIO_I_GET(I2C_SCL)
#define I2C_SCL_IN()    GPIO_E_SET(I2C_SCL, 0)
#define I2C_SDA_IN()    GPIO_E_SET(I2C_SDA, 0)
#define I2C_SCL_OUT()   GPIO_E_SET(I2C_SCL, 1)
#define I2C_SDA_OUT()   GPIO_E_SET(I2C_SDA, 1)
#define I2C_SDA_H()     GPIO_E_SET(I2C_SDA, 0)
#define I2C_SDA_L()     {I2C_SDA_SET(0); GPIO_E_SET(I2C_SDA, 1);}
#define I2C_SCL_H()     GPIO_E_SET(I2C_SCL, 0)
#define I2C_SCL_L()     {I2C_SCL_SET(0); GPIO_E_SET(I2C_SCL, 1);}

#define GT9xx_RSTB_GET()   GPIO_I_GET(GT9xx_RSTB)
#define GT9xx_RSTB_H()     GPIO_E_SET(GT9xx_RSTB, 0)
#define GT9xx_RSTB_IN()    GPIO_E_SET(GT9xx_RSTB, 0)
#define GT9xx_RSTB_L()     {GT9xx_RSTB_SET(0); GPIO_E_SET(GT9xx_RSTB, 1);}
#define GT9xx_RSTB_OUT()   GPIO_E_SET(GT9xx_RSTB, 1)
#define GT9xx_RSTB_SET(d)  GPIO_O_SET(GT9xx_RSTB, d)

#define GT9xx_INT_GET()   GPIO_I_GET(GT9xx_INT)
#define GT9xx_INT_H()     GPIO_E_SET(GT9xx_INT, 0)
#define GT9xx_INT_IN()    GPIO_E_SET(GT9xx_INT, 0)
#define GT9xx_INT_L()     {GT9xx_INT_SET(0); GPIO_E_SET(GT9xx_INT, 1);}
#define GT9xx_INT_OUT()   GPIO_E_SET(GT9xx_INT, 1)
#define GT9xx_INT_SET(d)  GPIO_O_SET(GT9xx_INT, d)

/*******************************************************************************
 * Function:       i2c_init_io_risc
 * Description:    
 * Calls:          
 * Called By:      
 * Input:          
 * Output:         
 * Return:         
 * Others:         
 * History Information Description:         
 * Date                      Author                          Modification         
 * 2011-7-18		         wangdh   	                    created
*******************************************************************************/
static void i2c_init_io_risc(void)
{
	GPIO_F_SET(I2C_SDA, 1);//guochao for 8288 ipod play
	GPIO_F_SET(I2C_SCL, 1);
	GPIO_M_SET(I2C_SDA, 1);
	GPIO_M_SET(I2C_SCL, 1);
}

/*******************************************************************************
 * Function:       i2c_init_io_iop
 * Description:    
 * Calls:          
 * Called By:      
 * Input:          
 * Output:         
 * Return:         
 * Others:         
 * History Information Description:         
 * Date                      Author                         Modification         
 * 2011-7-18		         wangdh   	                    created
*******************************************************************************/
static void i2c_init_io_iop(void)
{
	//GPIO_M_SET(I2C_SDA, 0);
	//GPIO_M_SET(I2C_SCL, 0);
}

/*******************************************************************************
 * Function:       delay_gpio
 * Description:    
 * Calls:          
 * Called By:      
 * Input:          
 * Output:         
 * Return:         
 * Others:         
 * History Information Description:         
 * Date                      Author                         Modification         
 * 2011-7-18		         wangdh   	                    created
*******************************************************************************/
static void delay_gpio(int i)
{
	do
	{
		int j = 50;
		do
		{
			asm volatile ("nop");
		}
		while (--j >= 0);
	}
	while (--i >= 0);
}

#define delay_i2c(i)    delay_gpio(i)
#define i2c_delay()     delay_i2c(1)

/*******************************************************************************
 * Function:       i2c_start_sig
 * Description:    SCL: --
 *                 SDA: -\_
 * Calls:          
 * Called By:      
 * Input:          
 * Output:         
 * Return:         
 * Others:         
 * History Information Description:         
 * Date                      Author                         Modification         
 * 2011-7-18		         wangdh   	                    created
*******************************************************************************/
static void i2c_start_sig(void)
{
    i2c_init_io_risc();
	I2C_SCL_H();
	I2C_SDA_H();
	delay_i2c(1);
	I2C_SDA_L();
	delay_i2c(1);
    i2c_init_io_iop();
}

/*******************************************************************************
 * Function:       i2c_byte_w
 * Description:    SCL: _/-\_/-\_/-\_/-\_/-\_/-\_/-\_/-\_/-\_
 *                 SDA: _<D7><D6><D5><D4><D3><D2><D1><D0>ACK__
 
 * Calls:          
 * Called By:      
 * Input:          
 * Output:         
 * Return:         
 * Others:         
 * History Information Description:         
 * Date                      Author                         Modification         
 * 2011-7-18		         wangdh   	                    created
*******************************************************************************/
static int i2c_byte_w(int data)
{
    i2c_init_io_risc();
	int i;
	I2C_SCL_L()

	//delay_i2c(1);
	for (i = 8; i; i--)
	{
		if (data & 0x80)
		{
			I2C_SDA_H();
		}
		else
		{
			I2C_SDA_L();
		}
		delay_i2c(1);     // scl -- low
		I2C_SCL_H();
		delay_i2c(1); //hawk	// scl -> high
		data <<= 1;
		I2C_SCL_L();    // scl -> low
	}

	// Get ACK
	I2C_SDA_IN();
	delay_i2c(1);           // scl -- low
	I2C_SCL_H();
	delay_i2c(1);           // scl -> high
	i = I2C_SDA_GET(); 		// 0 for no error
	//printf(" state=%d  ",i);
	I2C_SCL_L();
	delay_i2c(1);           // scl -> low
	
    i2c_init_io_iop();

	return ((i) ? 1 : 0);  //helm: 6500B i2c ACK is ok 051011
}

/*******************************************************************************
 * Function:       i2c_stop_sig
 * Description:    SCL: _/-
 *                 SDA: __/
 * Calls:          
 * Called By:      
 * Input:          
 * Output:         
 * Return:         
 * Others:         
 * History Information Description:         
 * Date                      Author                         Modification         
 * 2011-7-18		         wangdh   	                    created
*******************************************************************************/
static void i2c_stop_sig(void)
{
    i2c_init_io_risc();
	I2C_SDA_L();
	I2C_SCL_L();
	delay_i2c(1);
	I2C_SCL_H();
	delay_i2c(1);
	I2C_SDA_H();
    i2c_init_io_iop();
}

/*******************************************************************************
 * Function:       i2c_byte_r
 * Description:    SCL: _/-\_/-\_/-\_/-\_/-\_/-\_/-\_/-\_/-\_
 *                 SDA: _<D7><D6><D5><D4><D3><D2><D1><D0>ACK____
 * Calls:          
 * Called By:      
 * Input:          
 * Output:         
 * Return:         
 * Others:         
 * History Information Description:         
 * Date                      Author                         Modification         
 * 2011-7-18		         wangdh   	                    created
*******************************************************************************/
static unsigned char i2c_byte_r(BYTE ack)
{
	int i;
	unsigned char data = 0;

    i2c_init_io_risc();
	I2C_SCL_L();
	I2C_SDA_IN(); // delay_i2c(1);			// scl -> low

	for (i = 8; i; i--)
	{
		delay_i2c(1);
		I2C_SCL_IN();   // scl -> high
		delay_i2c(1);
		data = (data << 1) | (I2C_SDA_GET() ? 1 : 0);
		I2C_SCL_L();    // scl -> low
	}

	// Set ACK !!
	if (ack == 1)
	{
		I2C_SDA_H();                                    // scl -- low
	}
	else
	{
		I2C_SDA_L();                                    // scl -- low
	}
	delay_i2c(1);           // scl -- low
	I2C_SCL_H();
	delay_i2c(1);           // scl -> high
	I2C_SCL_L();
	delay_i2c(1);           // scl -> low
    i2c_init_io_iop();
    
	return data;
}

static void i2c_stop(void)
{
    i2c_init_io_risc();
	I2C_SCL_L();
	I2C_SDA_L();
	i2c_delay();
	I2C_SCL_H();
	i2c_delay();
	I2C_SDA_H();
	i2c_delay();
	I2C_SCL_L();
	i2c_delay();
	I2C_SCL_H();
	I2C_SDA_H();
    i2c_init_io_iop();
}

static void init_i2c(void)
{
    i2c_init_io_risc();
	I2C_SCL_H();
	I2C_SDA_H();
	i2c_delay();
	i2c_stop();
    i2c_init_io_iop();
}

#endif /* BUS_I2C_GPIO */

/***********************************************************************
 * Function:       reset_tsc2003
 * Description:    
 * Calls:          1.i2c_start_sig()  : start signal
                   2.i2c_byte_w()     : wirte 1 byte
                   3.i2c_stop_sig()   : stop signal
 * Called By:      is_touched()
 * Input:          None
 * Output:         None
 * Return:         None
 * History Information Description        
 * Date                      Author                         Modification 
   2012-02-15                shengjun                       creating
 * Others:         
***********************************************************************/
#ifdef CONFIG_TP_TSC2003
static void reset_tsc2003(void)  
{
	int iDeviceAddr = TSC2003_write; 
	int command = command_setup;
	
	#ifndef HW_I2C
	#ifndef SUPPORT_8288_HW_I2CM
    i2c_start_sig();
    if(0==i2c_byte_w(iDeviceAddr)) 
    {
        if(0==i2c_byte_w(command))
        {
            i2c_stop_sig();  
        }
    }
	#endif
	#else
	BYTE a[1];
	Write_I2c_Data(iDeviceAddr,command,a,I2C_WRITE_ONLY_DATA);
	#endif
}
#endif
#ifdef CONFIG_TP_GT9XX
void reset_gt9xx(void)
{
	int write_gt9xx(unsigned char reg_h, unsigned char reg_l, char buf[], int count);
	int read_gt9xx(unsigned char reg_h, unsigned char reg_l, char buf[], int count);

	static int reseted = 0;
	char buf[1];

	if(reseted) return;

#define CONFIG_TP_GT9XX_PWR_EN
#define SYSIO_TP_PWR_EN 58
#ifdef CONFIG_TP_GT9XX_PWR_EN
		GPIO_F_SET(SYSIO_TP_PWR_EN, eHW_GPIO_FIRST_GPIO);
		GPIO_M_SET(SYSIO_TP_PWR_EN, eHW_GPIO_RISC);
		GPIO_E_SET(SYSIO_TP_PWR_EN, eHW_GPIO_OUT);
		GPIO_O_SET(SYSIO_TP_PWR_EN, eHW_GPIO_STS_HIGH);
//		delay_1ms(50);
//		GPIO_O_SET(SYSIO_NIM_RST, eHW_GPIO_STS_LOW);
//		delay_1ms(100);
		GPIO_O_SET(SYSIO_TP_PWR_EN, eHW_GPIO_STS_HIGH);
//		delay_1ms(50);
#endif	
	

	read_gt9xx(0x81, 0x4E, buf, 1);
	buf[0] = 0x00;
	write_gt9xx(0x81, 0x4E, buf, 1);

	printf("TP: reset\n");
	reseted = 1;
	return;
}
#endif

/***********************************************************************
 * Function:       write_tsc2003
 * Description:    write tsc2003, must open #define DVI_I2C_SET
 * Calls:          1.i2c_start_sig()  : start signal
                   2.i2c_byte_w()     : wirte 1 byte
                   3.i2c_stop_sig()   : stop signal
 * Called By:      is_touched()
 * Input:          1.int iDeviceAddr  : device address
                   2.int command      : command
 * Output:         None
 * Return:         None
 * History Information Description        
 * Date                      Author                         Modification 
   2006-07-10                chuanyue.li                     creating
   2007-11-09                shenyj                          modify
   2007-11-28                aisubin                         notation
 * Others:         
***********************************************************************/

#ifdef CONFIG_TP_TSC2003
void write_tsc2003(int iDeviceAddr, int command )
{
    int loop;   
    for (loop=0; loop<tsc2003_wr_loop_max; loop++)
    {
		#ifndef SUPPORT_8288_HW_I2CM
        i2c_start_sig();
        
        if(0==i2c_byte_w(iDeviceAddr)) 
        {
            if(0==i2c_byte_w(command))
            {
                i2c_stop_sig();  
                
                break;
            }
        }
        
        init_i2c();
		#else
		BYTE a[1];
		if(Write_I2c_Data(iDeviceAddr,command,a,I2C_WRITE_ONLY_DATA)==0)
		break;
		#endif
    }
}
#endif

int write_gt9xx(unsigned char reg_h, unsigned char reg_l, char buf[], int count)
{
	BYTE ack = 1;
	int i = -1, rc;
	char c;
	int retry = 2;
	char addr = 0x28;

	while(retry--) {
		i2c_start_sig();
		rc = i2c_byte_w(addr & 0xFE);
		if(rc) {
			i2c_stop_sig();
			addr = 0x28;
			printf("TP: try [%02x]\n", addr);
			continue;
		}
		rc = i2c_byte_w(reg_h);
		if(rc) {
			printf("TP: reg_h\n");
			break;
		}

		rc = i2c_byte_w(reg_l);
		if(rc) {
			printf("TP: reg_l\n");
			break;
		}

		/* Register address is sent. */
		for(i=0; i<count; i++){
			c = buf[i];
			rc = i2c_byte_w(c);
			if(rc) {
				printf("TP: WR[%d]\n", i);
				break;
			}
		}	
		i2c_stop_sig();

		return i;
	}
	
	/* exit with error. */
	init_i2c();
	return i;

}

int read_gt9xx(unsigned char reg_h, unsigned char reg_l, char buf[], int count)
{
#if 0
	GPIO_F_SET(I2C_SDA, eHW_GPIO_FIRST_GPIO);
	GPIO_M_SET(I2C_SDA, eHW_GPIO_RISC);
	GPIO_E_SET(I2C_SDA, eHW_GPIO_OUT);
	GPIO_O_SET(I2C_SDA, eHW_GPIO_STS_LOW);

	GPIO_F_SET(I2C_SCL, eHW_GPIO_FIRST_GPIO);
	GPIO_M_SET(I2C_SCL, eHW_GPIO_RISC);
	GPIO_E_SET(I2C_SCL, eHW_GPIO_OUT);
	GPIO_O_SET(I2C_SCL, eHW_GPIO_STS_LOW);

	GPIO_F_SET(GT9xx_RSTB, eHW_GPIO_FIRST_GPIO);
	GPIO_M_SET(GT9xx_RSTB, eHW_GPIO_RISC);
	GPIO_E_SET(GT9xx_RSTB, eHW_GPIO_OUT);
	GPIO_O_SET(GT9xx_RSTB, eHW_GPIO_STS_LOW);

	delay_1ms(1);

	//I2C_SDA_OUT();
	//I2C_SCL_OUT();
	//GT9xx_RSTB_OUT();

	I2C_SDA_H();
	I2C_SCL_H();
	GT9xx_RSTB_H();

	for(i=0; i<3; i++) {
		I2C_SCL_H(); 
		delay_1us(32);

		I2C_SCL_L();
		delay_1us(8);		
	}
	
	for(i=0; i<4; i++) {
		I2C_SDA_H();
		delay_1us(16);

		I2C_SDA_L();
		delay_1us(8);		
	}
	for(i=0; i<5; i++) {
		GT9xx_RSTB_H();
		delay_1us(8);

		GT9xx_RSTB_L();
		delay_1us(8);		
	}

	I2C_SDA_H();
	I2C_SCL_H();
	GT9xx_RSTB_H();

	delay_1us(4);		

	I2C_SDA_L();
	I2C_SCL_L();
	GT9xx_RSTB_L();



	return 0;
#endif

	BYTE ack = 1;
	int i = -1, rc;
	char c;
	int retry = 2;
	char addr = 0x28;

	while(retry--) {
		i2c_start_sig();
		rc = i2c_byte_w(addr & 0xFE);
		if(rc) {
			i2c_stop_sig();
			addr = 0x28;
			printf("TP: try [%02x]\n", addr);
			continue;
		}
		rc = i2c_byte_w(reg_h);
		if(rc) {
			printf("TP: reg_h\n");
			break;
		}

		rc = i2c_byte_w(reg_l);
		if(rc) {
			printf("TP: reg_l\n");
			break;
		}
		i2c_stop_sig();
		/* Register address is sent. */

		i2c_start_sig();
		rc = i2c_byte_w(addr | 1);
		if(rc) {
			printf("TP: addr1\n");
		}

		for(i=0; i<count; i++){
			ack = i == count-1;
			c = i2c_byte_r(ack);
			buf[i] = c;
		}	
		i2c_stop_sig();

		return i;
	}

	init_i2c();
	return i;
}


#if 0
int read_gt9xx(unsigned reg, char buf[], int count)
{
	return __read_gt9xx(reg/256, reg%256, buf, count);
}
#endif

/***********************************************************************
 * Function:       read_tsc2003
 * Description:    read tsc2003, must open #define DVI_I2C_SET
 * Calls:          1.i2c_start_sig()  : start signal
                   2.i2c_byte_w()     : wirte 1 byte
                   3.i2c_stop_sig()   : stop signal
 * Called By:      get_x_coordinate()
 * Input:          1.int iDeviceAddr  : device address
 * Output:         None
 * Return:         None
 * History Information Description        
 * Date                       Author                         Modification 
   2006-07-10                 chuanyue.li                     creating
   2007-11-09                 shenyj                          modify
   2007-11-28                 aisubin                         notation
 * Others:         
***********************************************************************/

#ifdef CONFIG_TP_TSC2003
int read_tsc2003(int iDeviceAddr)
{
	UINT16  data=0;
	int  loop;
    
	for (loop=0; loop<tsc2003_wr_loop_max; loop++)
	{
		#ifndef SUPPORT_8288_HW_I2CM		
		i2c_start_sig();
        
		if(0 == i2c_byte_w (iDeviceAddr)) 
		{
			data = i2c_2bytes_r();/*use 12bit mode*///i2c_byte_r(1); 
			i2c_stop_sig(); 
            
			break;
		 }
        
		init_i2c();
		#else
		BYTE a[2];
		if(Read_I2c_Data(iDeviceAddr,0,a,2,I2C_UNNEED_SUBADDR)==0)
		{
			data=((UINT16)a[0]<<8)|a[1];
			break;
		}
		#endif
	}  
    
	data = data>>4;
    
	return   data;
}
#endif

/***********************************************************************
 * Function:       is_touched
 * Description:    judge  /penirq gpio is low or high?  
                   GPIO=low, touched;
                   GPIO=high, no touched
	   	           must define PENIRQ_STATUS_GPIO at touch_panel.h
 * Calls:          write_tsc2003()
 * Called By:      polling_tsc2003_100ms()
 * Input:          None
 * Output:         None
 * Return:         None
 * History Information Description        
 * Date                      Author                         Modification 
   2006-07-10                chuanyue.li                     creating
   2007-11-09                shenyj                          modify
   2007-11-28                aisubin                         notation
 * Others:         
***********************************************************************/

int is_touched()
{  	
	int penirq_flag=0;

#ifdef CONFIG_TP_GT9XX
	static int poll = 0;
	char buf[1];
	const int poll_delay = 25;

#ifdef CONFIG_TP_GT9XX_FILTER
	static int hold = 0;
#endif

	if(poll<100) poll++;

	if(poll == poll_delay) reset_gt9xx();
	else if(poll < poll_delay) return 0;

	printf(".");

#ifdef CONFIG_TP_GT9XX_POLL_INT
	penirq_flag = GT9xx_INT_GET();
#endif
#ifdef CONFIG_TP_GT9XX_POLL_I2C
	read_gt9xx(0x81, 0x4E, buf, 1);
	penirq_flag = 0x80 & buf[0];
	penirq_flag = !penirq_flag;
#endif
#ifdef CONFIG_TP_GT9XX_FILTER
	if(penirq_flag == 0) {
		hold++;
		printf("hold: %d\n", hold);
	} else {
		hold = 0;
	}
#endif
	

#endif /* CONFIG_TP_GT9XX */

#ifdef CONFIG_TP_TSC2003
    //write_tsc2003( TSC2003_write, 0xb0 );//TSC2007 reset pd0,pd1,is  nessary!!!
    reset_tsc2003();  //shengjun new update for cost time down  2012-2-15
    
	GPIO_F_SET(PENIRQ_STATUS_GPIO,1);
    GPIO_E_SET(PENIRQ_STATUS_GPIO,1);
    GPIO_M_SET(PENIRQ_STATUS_GPIO, 1);
    GPIO_E_SET(PENIRQ_STATUS_GPIO, 0);
    penirq_flag = GPIO_I_GET(PENIRQ_STATUS_GPIO);
#endif
	//printf_w("%d  penirq_flag %d\n",PENIRQ_STATUS_GPIO,penirq_flag);
	if(penirq_flag == 0)
	{
		return(1);
	}
	else
	{
		return(0);		
	}
}

/***********************************************************************
 * Function:       get_x_coordinate
 * Description:    get x coordinate
 * Calls:          1.write_tsc2003()  : write
                   2.read_tsc2003()   : read
                   
 * Called By:      polling_tsc2003_100ms()
 * Input:          None
 * Output:         None
 * Return:         None
 * History Information Description        
 * Date                      Author                         Modification 
   2006-07-10                chuanyue.li                     creating
   2007-11-09                shenyj                          modify
   2007-11-28                aisubin                         notation
 * Others:         
***********************************************************************/

#ifdef CONFIG_TP_TSC2003
int  get_x_coordinate()
{  
	UINT16 X=0;
    
    write_tsc2003( TSC2003_write,command_x );
    
    delay_1us(200);  // fly
    X= read_tsc2003(TSC2003_read);
    return X;
}

/***********************************************************************
 * Function:       get_y_coordinate
 * Description:    get y coordinate
 * Calls:          1.write_tsc2003()  : write
                   2.read_tsc2003()   : read
                   
 * Called By:      polling_tsc2003_100ms()
 * Input:          None
 * Output:         None
 * Return:         None
 * History Information Description        
 * Date                      Author                         Modification 
   2006-07-10                chuanyue.li                     creating
   2007-11-09                shenyj                          modify
   2007-11-28                aisubin                         notation
 * Others:         
***********************************************************************/

int  get_y_coordinate()
{  
	UINT16 Y=0;
    
	write_tsc2003( TSC2003_write,command_y );
	delay_1us(200);  // fly
	Y= read_tsc2003(TSC2003_read);
    return Y;
}
#endif

#ifdef CONFIG_TP_GT9XX
int  get_xy_coordinate(UINT16 *x, UINT16 *y, int point)
{
	char buf[9];

	read_gt9xx(0x81, 0x4E, buf, 9);

	if(buf[0] & 0x80) {
		*x = buf[3]<<8 | buf[2];
		*y = buf[5]<<8 | buf[4];

		*x = (*x) * 720 / 0x400;
		*y = (*y) * 480 / 0x258;

		printf("TP: (%d, %d)\n", *x, *y);
		if(buf[0] & 0x0E) {
			printf("TP: MT!(ignored)\n");
		}
		buf[0] = 0;
		write_gt9xx(0x81, 0x4E, buf, 1);
		return 0;
	}

	return -1;
}
#endif
/***********************************************************************
 * Function:       polling_tsc2003_100ms
 * Description:    get x&y coordinate
 * Calls:          1.is_touched()     : judge whether or not touch action
                   2.get_x_coordinate : get x coordinare
                   3.get_y_coordinate : get y coordinare
                   
 * Called By:      car_TouchPanel_FirmwareInit()
 * Input:          TouchDriver_Handle *pTouchDriver_Handle
 * Output:         None
 * Return:         None
 * History Information Description        
 * Date                      Author                         Modification 
   2007-11-09                shenyj                          modify
   2007-11-28                aisubin                         notation
   2011-03-15	             yinhuakui			modify(support LONG_PRESS,SLIP_PRESS)
 * Others:         
***********************************************************************/
static void reset_touch_keep_counter(TouchDriver_Handle *pTchHandle)
{
	if(pTchHandle->event_id == EVENT_DOWN)
	{
		pTchHandle->counter_flag = 0;
	}
}

static int check_is_touch_keep(TouchDriver_Handle *pTchHandle)
{
	if(pTchHandle->event_id == EVENT_KEEP)
		return 1;

	pTchHandle->counter_flag++;
	
	if(pTchHandle->counter_flag > 5)
		return 1;
	else
		return 0;
}

static int check_is_touch_move(TouchDriver_Handle *pTchHandle,UINT16 wTemp_X,UINT16 wTemp_Y)
{
	UINT16 wChange_X , wChange_Y;

	if(pTchHandle->event_id == EVENT_MOVE)
		return 1;
	
	if(pTchHandle->Touch_x_now < wTemp_X)
		wChange_X = wTemp_X - pTchHandle->Touch_x_now;
	else
		wChange_X = pTchHandle->Touch_x_now - wTemp_X;
	
	if(pTchHandle->Touch_y_now < wTemp_Y)
		wChange_Y = wTemp_Y - pTchHandle->Touch_y_now;
	else
		wChange_Y = pTchHandle->Touch_y_now - wTemp_Y;
	
	if((wChange_X>x_keep_range)||(wChange_Y>y_keep_range))
		return 1;
	else
		return 0;
}

static inline void polling_tsc2003_input(TouchDriver_Handle *pTchHandle)
{
	UINT16 wTemp_X , wTemp_Y;
	UINT8 bEventID = pTchHandle->event_id;
	
 	if(!is_touched())
 	{
 		/*up or untouch event*/
		if(bEventID == EVENT_IDLE)
		{
			return ;
		}
		else if(bEventID == EVENT_KEEP)
		{
			pTchHandle->event_id = EVENT_UP_PLT;
			pTchHandle->bIsDirty = 0;
			printf_w("\tEVENT_UP_PLT\n");
		}
		else if((bEventID != EVENT_UP) && (pTchHandle->event_id != EVENT_UP_PLT))
		{
			pTchHandle->event_id = EVENT_UP;
			pTchHandle->bIsDirty = 0;
			printf_w("\tEVENT_UP\n");
		}
		else
		{
			pTchHandle->event_id = EVENT_IDLE;
			printf_w("\tEVENT_IDLE\n");
		}
		return ;
 	}
	
#ifdef CONFIG_TP_TSC2003
	wTemp_X = get_x_coordinate();
	wTemp_Y = get_y_coordinate();
#endif
#ifdef CONFIG_TP_GT9XX
	get_xy_coordinate(&wTemp_X, &wTemp_Y, 0);
#endif

	//cout("wTemp_X:%d wTemp_Y:%d\n", wTemp_X, wTemp_Y);

	pTchHandle->bIsDirty = 0;
	if(bEventID == EVENT_IDLE)
	{
		pTchHandle->event_id = EVENT_DOWN;
		pTchHandle->Touch_x_start = wTemp_X;
		pTchHandle->Touch_y_start = wTemp_Y;
		reset_touch_keep_counter(pTchHandle);
		printf_w("\tEVENT_DOWN\n");
	}
	else
	{
		if(pTchHandle->event_id != EVENT_KEEPMOVE)
		{
			if(check_is_touch_keep(pTchHandle))
			{
				if(check_is_touch_move(pTchHandle,wTemp_X,wTemp_Y))
				{
					if(bEventID != EVENT_KEEPMOVE)
					{
						pTchHandle->event_id = EVENT_KEEPMOVE;
						printf_w("\tEVENT_KEEPMOVE\n");
					}
				}
				else
				{
					if(bEventID != EVENT_KEEP)
					{
						pTchHandle->event_id = EVENT_KEEP;
						printf_w("\tEVENT_KEEP\n");
					}
				}
			}
			else if(check_is_touch_move(pTchHandle,wTemp_X,wTemp_Y))
			{
				if(bEventID != EVENT_MOVE)
				{
					pTchHandle->event_id = EVENT_MOVE;
					printf_w("\tEVENT_MOVE\n");
				}
			}
		}
	}
	
	//printf_w("\t[%04d  %04d] ID %d\n",wTemp_X,wTemp_Y,pTchHandle->event_id);
	
	pTchHandle->Touch_x_now= wTemp_X;
	pTchHandle->Touch_y_now= wTemp_Y;	
}
