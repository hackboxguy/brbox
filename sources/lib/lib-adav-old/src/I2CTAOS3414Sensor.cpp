//this code is based on GroveColorSensor: https://github.com/Seeed-Studio/Grove_I2C_Color_Sensor/blob/master/GroveColorSensor.h
#include "I2CTAOS3414Sensor.hpp"
#include <unistd.h>
#include <string.h>
/*****************************************************************************/
I2CTAOS3414Sensor::I2CTAOS3414Sensor(std::string DevNode,std::string DevType):LightSensor(DevNode)//,I2CBusAccess(DevNode)
{
	sensorI2CAddr=COLOR_SENSOR_ADDR;
	ledStatus = 0;//flashlight led on the grove color sensor
}
I2CTAOS3414Sensor::~I2CTAOS3414Sensor()
{
}
/*****************************************************************************/
void I2CTAOS3414Sensor::setTimingReg()
{
	uint8_t data[16];data[0]=REG_TIMING;data[1]=triggerMode_;
	write_array((uint32_t)sensorI2CAddr, data,2);
	//delay(10); 
}
/*****************************************************************************/
void I2CTAOS3414Sensor::setInterruptSourceReg()
{
	uint8_t data[16];data[0]=REG_INT_SOURCE;data[1]=interruptSource_;
	write_array((uint32_t)sensorI2CAddr, data,2);
}
/*****************************************************************************/
void I2CTAOS3414Sensor::setInterruptControlReg()
{
	uint8_t data[16];data[0]=REG_INT;data[1]=interruptMode_;
	write_array((uint32_t)sensorI2CAddr, data,2);
}
/*****************************************************************************/
void I2CTAOS3414Sensor::setGain()
{
	uint8_t data[16];data[0]=REG_GAIN;data[1]=gainAndPrescaler_;
	write_array((uint32_t)sensorI2CAddr, data,2);
}
/*****************************************************************************/
void I2CTAOS3414Sensor::setEnableADC()
{
	uint8_t data[16];data[0]=REG_CTL;data[1]=CTL_DAT_INIITIATE;
	write_array((uint32_t)sensorI2CAddr, data,2);
}
/*****************************************************************************/
void I2CTAOS3414Sensor::clearInterrupt()
{
	uint8_t data[16];data[0]=CLR_INT;
	write_array((uint32_t)sensorI2CAddr, data,1);
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::readRGB()
{
	uint8_t data[16];data[0]=REG_BLOCK_READ;
	write_array((uint32_t)sensorI2CAddr, data,1);
	
	read_array((uint32_t)sensorI2CAddr, readingdata_,8);
	green_	= readingdata_[1] * 256 + readingdata_[0];
	red_ 	= readingdata_[3] * 256 + readingdata_[2];
	blue_	= readingdata_[5] * 256 + readingdata_[4];
	clear_	= readingdata_[7] * 256 + readingdata_[6];
	
	cout<<"I2CTAOS3414Sensor::readRGB:red   ="<<red_<<endl;
	cout<<"I2CTAOS3414Sensor::readRGB:green ="<<green_<<endl;
	cout<<"I2CTAOS3414Sensor::readRGB:blue  ="<<blue_<<endl;
	cout<<"I2CTAOS3414Sensor::readRGB:white ="<<clear_<<endl;
	return RPC_SRV_RESULT_SUCCESS;
	/*Serial.print("The RGB value are: RGB( ");
	Serial.print(red_,DEC);
	Serial.print(", ");
	Serial.print(green_,DEC);
	Serial.print(", ");
	Serial.print(blue_,DEC);
	Serial.println(" )");
	Serial.print("The Clear channel value are: ");
	Serial.println(clear_,DEC);*/
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::calculateCoordinate()
{
	double X;
	double Y;
	double Z;
	double x;
	double y;
	
	X = (-0.14282) * red_ + (1.54924) * green_ + (-0.95641) * blue_;
	Y = (-0.32466) * red_ + (1.57837) * green_ + (-0.73191) * blue_;
	Z = (-0.68202) * red_ + (0.77073) * green_ + (0.563320) * blue_;
	
	x = X / (X + Y + Z);
	y = Y / (X + Y + Z);
	
	if( (X > 0) && ( Y > 0) && ( Z > 0) )
	{
		//Serial.println("The x,y values are(");
		//Serial.print(x, 2);
		//Serial.print(" , ");
		//Serial.print(y, 2);
		//Serial.println(")");
		return RPC_SRV_RESULT_SUCCESS;
	}
	else
		return RPC_SRV_RESULT_FAIL;//Serial.println("Error: overflow!");
}
/*****************************************************************************/
void I2CTAOS3414Sensor::readRGB(int *red, int *green, int *blue)
{
	uint8_t data[16];data[0]=REG_BLOCK_READ;
	write_array((uint32_t)sensorI2CAddr, data,1);
	read_array((uint32_t)sensorI2CAddr, readingdata_,8);
	green_	= readingdata_[1] * 256 + readingdata_[0];
	red_ 	= readingdata_[3] * 256 + readingdata_[2];
	blue_	= readingdata_[5] * 256 + readingdata_[4];
	clear_	= readingdata_[7] * 256 + readingdata_[6];
	
	double tmp;
	int maxColor;
	
	if ( ledStatus == 1 )
	{
		red_  = red_  * 1.70;
		blue_ = blue_ * 1.35;

		maxColor = max(red_, green_);
		maxColor = max(maxColor, blue_);
	   
		if(maxColor > 255)
		{
			tmp = 250.0/maxColor;
			green_	*= tmp;
			red_ 	*= tmp;
			blue_	*= tmp;
		}
	}
	if ( ledStatus == 0 )
	{
		maxColor = max(red_, green_);
		maxColor = max(maxColor, blue_);
	   
		tmp = 250.0/maxColor;
		green_	*= tmp;
		red_ 	*= tmp;
		blue_	*= tmp;

	}
	
	int minColor = min(red_, green_);
	minColor = min(maxColor, blue_);
	maxColor = max(red_, green_);
	maxColor = max(maxColor, blue_);
	
	int greenTmp = green_;
	int redTmp 	 = red_;
	int blueTmp	 = blue_;
	
	//when turn on LED, need to adjust the RGB data,otherwise it is almost the white color
	if(red_ < 0.8*maxColor && red_ >= 0.6*maxColor)
	{
		red_ *= 0.4;
	}
	else if(red_ < 0.6*maxColor)
	{
		red_ *= 0.2;
	}
	
	if(green_ < 0.8*maxColor && green_ >= 0.6*maxColor)
	{
		green_ *= 0.4;
	}
	else if(green_ < 0.6*maxColor)
	{
		if (maxColor == redTmp && greenTmp >= 2*blueTmp && greenTmp >= 0.2*redTmp)//orange
		{
			green_ *= 5;
		}
		green_ *= 0.2;
	}
	
	if(blue_ < 0.8*maxColor && blue_ >= 0.6*maxColor)
	{
		blue_ *= 0.4;
	}
	else if(blue_ < 0.6*maxColor)
	{
		if (maxColor == redTmp && greenTmp >= 2*blueTmp && greenTmp >= 0.2*redTmp)//orange
		{
			blue_ *= 0.5;
		}
		if (maxColor == redTmp && greenTmp <= blueTmp && blueTmp >= 0.2*redTmp)	//pink
		{
			blue_  *= 5;
		}
		blue_ *= 0.2;
	}
	minColor = min(red_, green_);
	minColor = min(maxColor, blue_);
	if(maxColor == green_ && red_ >= 0.85*maxColor && minColor == blue_)//yellow
	{
		red_ = maxColor;
		blue_ *= 0.4;
	}
	*red   = red_;
	*green = green_;
	*blue  = blue_;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::init_sensor()
{
	cout<<"I2CTAOS3414Sensor::init_sensor:called"<<endl;
	triggerMode_     =INTEG_MODE_FREE | INTEG_PARAM_PULSE_COUNT1;
	interruptSource_ =INT_SOURCE_CLEAR;
	interruptMode_   =INTR_LEVEL | INTR_PERSIST_EVERY;
	gainAndPrescaler_=GAIN_1 | PRESCALER_1;
	sensorAddress_   =COLOR_SENSOR_ADDR;
	I2CTAOS3414Sensor::setTimingReg(); 
	I2CTAOS3414Sensor::setInterruptSourceReg();  
	I2CTAOS3414Sensor::setInterruptControlReg(); 
	I2CTAOS3414Sensor::setGain(); 
	I2CTAOS3414Sensor::setEnableADC(); 
}
/*****************************************************************************/

