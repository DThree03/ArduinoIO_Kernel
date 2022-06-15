#ifndef	_ARDUINO_IO_KERNEL_H_
#define _ARDUINO_IO_KERNEL_H_

#include <Arduino.h>

#include "config/dataTypedef.h"

class basicOutput
{
	public:
		basicOutput(void);
		void init(int setPin);
		void init(int setPin, bool status);
		bool config(bool bStartState, uint32_t uCountToggle, uint32_t uFrequency, bool bFlagInterrupt);
		void outputTask(void);
		bool status(void);
	private:
		structIO_Manage_Output strOutput;
		int outputPin;
		bool outputState;
		
		void outputWrite(bool BitVal);
};

class latchRelay
{
	public:
		latchRelay(uint32_t latRLdelayTime);
		void init(int setOpenPin, int setClosePin);
		void init(int setOpenPin, int setClosePin, bool status);
		bool config(bool bRLState);
		void outputTask(void);
		bool status(void);
	private:
		structIO_Manage_LatRL strLatRL;
		int openPin;
		int closePin;
		bool RLState;
		
		void latRLWrite(uint8_t BitVal);
};

class inputSwitch
{
	public:
		inputSwitch(void);
		void init(int swPin, uint8_t mode);
		void inputTask(void);
		bool isNewStateSW(void);
	private:
		bool bSwitchState;
		uint8_t	uSWTimeCounter;
		int iSwitchPin;
		int FlagNewSWState;
		bool status(void);
};

class inputButton
{
	public:
		inputButton(void);
		void init(int btPin, uint8_t mode, uint8_t btMode);
		void inputTask(void);
		eButtonState buttonState(void);
	
	private:
		structIO_Button OldValue;
		structIO_Button NewValue;
		uint32_t	uTimeCheck;
		uint32_t	uTimePress;
		uint32_t	bClickCount;
		uint32_t	bHoldOff;
		int 		iButtonPin;
		uint8_t		u8ButtonMode;
		bool 		flag_start_sample;
		
		bool status(void);
		void vGetIOButtonValue(bool InputState);
};
#endif