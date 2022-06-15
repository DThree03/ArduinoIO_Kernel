/*
	IO-Kernel Arduino Library for Arduino or ESP module.
	Author: DThree03
*/

#include "IO-Kernel.h"

/***************************************************************************************************************/
void basicOutput::init(int setPin)
{
	outputPin = setPin;
	pinMode(outputPin, OUTPUT);
	digitalWrite(outputPin, outputState);
}

void basicOutput::init(int setPin, bool status)
{
	outputPin = setPin;
	outputState = status;
	pinMode(outputPin, OUTPUT);
	digitalWrite(outputPin, outputState);
}

void basicOutput::outputWrite(bool BitVal)
{
	outputState = BitVal;
	digitalWrite(outputPin, outputState);
}

basicOutput::basicOutput(void)
{
	outputState = false;
}

bool basicOutput::status(void)
{
	return outputState;
}

bool basicOutput::config(bool bStartState, uint32_t uCountToggle, uint32_t uFrequency, bool bFlagInterrupt)
{
	bool bFlagSetParaSucess = false;
	if(bFlagInterrupt)
	{
		strOutput.bCurrentProcess = true;

		strOutput.uFrequency 	= uFrequency;
		strOutput.uCountToggle 	= uCountToggle;
		strOutput.bStartState 	= bStartState;
		strOutput.bFlagStart	= true;

		strOutput.uCycleCounter	= uFrequency - 1;

		bFlagSetParaSucess		= true;
	}
	else
	{
		if(!strOutput.bCurrentProcess)
		{
			strOutput.bCurrentProcess = true;

			strOutput.uFrequency 	= uFrequency;
			strOutput.uCountToggle 	= uCountToggle;
			strOutput.bStartState 	= bStartState;
			strOutput.bFlagStart		= true;

			strOutput.uCycleCounter	= uFrequency - 1;

			bFlagSetParaSucess		= true;
		}
	}
	return bFlagSetParaSucess;
}

void basicOutput::outputTask(void)
{
	if(strOutput.bCurrentProcess)
	{
		// Increase counter
		strOutput.uCycleCounter++;
		if(((strOutput.uCycleCounter)%(strOutput.uFrequency))==0)
		{

			if(strOutput.bFlagStart)
			{
				strOutput.bFlagStart = false;
				outputWrite(strOutput.bStartState);
				if(strOutput.uCountToggle != 0)
				{
					strOutput.uCountToggle--;
				}
			}
			else
			{
				if(strOutput.uCountToggle!=0)
				{
					strOutput.uCountToggle--;
					outputWrite(1 - status());
				}
				else
				{
					//pControl->write(strOutput.bEndState);
					strOutput.bCurrentProcess = false;
				}
			}
		}
	}
}

/***************************************************************************************************************/
void latchRelay::init(int setOpenPin, int setClosePin)
{
	openPin = setOpenPin;
	closePin = setClosePin;
	pinMode(openPin, OUTPUT);
	digitalWrite(openPin, false);
	pinMode(closePin, OUTPUT);
	digitalWrite(openPin, false);
}

void latchRelay::init(int setOpenPin, int setClosePin, bool status)
{
	init(setOpenPin, setClosePin);
	
	RLState = status;
	latRLWrite(status);
	delay(100);
	latRLWrite(0x02);
}

void latchRelay::latRLWrite(uint8_t BitVal)
{
	if(BitVal == 0x01){
		digitalWrite(openPin, HIGH);
		RLState = true;
	}
	else if(BitVal == 0){
		digitalWrite(closePin, HIGH);
		RLState = false;
	}
	else{
		digitalWrite(openPin, LOW);
		digitalWrite(closePin, LOW);
	}
}

latchRelay::latchRelay(uint32_t latRLdelayTime)
{
	RLState = false;
	strLatRL.uTime = latRLdelayTime;
	strLatRL.bCurrentProcess = false;
}

bool latchRelay::status(void)
{
	return RLState;
}

bool latchRelay::config(bool bRLState)
{
	bool bFlagSetParaSucess = false;
	/* Check flag process */
	if(strLatRL.bCurrentProcess == false)
	{
		/* Enable Flag Process */
		strLatRL.bCurrentProcess = true;
		/* Config parameter */
		strLatRL.bRLState = bRLState;
		strLatRL.bFlagStart = true;
		/* Reset Counter */
		strLatRL.uCycleCounter = strLatRL.uTime - 1;
		/* Set Flag Success */
		bFlagSetParaSucess = true;
	}
	return bFlagSetParaSucess;
}

void latchRelay::outputTask(void)
{
	if(strLatRL.bCurrentProcess)
	{
		/* Increase counter */
		strLatRL.uCycleCounter++;
		/* Check Frequency */
		if(((strLatRL.uCycleCounter)%(strLatRL.uTime))==0)
		{
			/* Check bFlagStart */
			if(strLatRL.bFlagStart)
			{
				strLatRL.bFlagStart = false;
				latRLWrite(strLatRL.bRLState);
			}
			else
			{
				latRLWrite(0x02);
				strLatRL.bCurrentProcess = false;
			}
		}
	}
}

/***************************************************************************************************************/
void inputSwitch::init(int swPin, uint8_t mode)
{
	iSwitchPin = swPin;
	pinMode(iSwitchPin, mode);
	bSwitchState = digitalRead(iSwitchPin);
}

bool inputSwitch::status(void)
{
	return digitalRead(iSwitchPin);
}

inputSwitch::inputSwitch(void)
{
	FlagNewSWState = 0;
	uSWTimeCounter = 0;
}

void inputSwitch::inputTask(void)
{
	bool StatusSW = status();
	if(StatusSW != bSwitchState)
	{
		uSWTimeCounter++;
		if(uSWTimeCounter > 2){
			bSwitchState = StatusSW;
			FlagNewSWState++;
		}
	}
	else{
		if(uSWTimeCounter > 0)
			uSWTimeCounter = 0;
	}
}

bool inputSwitch::isNewStateSW(void)
{
	if(FlagNewSWState > 0){
		FlagNewSWState--;
		return true;
	}
	return false;
}

/***************************************************************************************************************/
void inputButton::init(int btPin, uint8_t mode, uint8_t btMode)
{
	iButtonPin = btPin;
	u8ButtonMode = btMode;
	pinMode(iButtonPin, mode);
}

bool inputButton::status(void)
{
	/* FALSE button press, TRUE button release*/
	if(u8ButtonMode == (uint8_t)ACTIVE_LOW)
		return digitalRead(iButtonPin);
	else
		return !digitalRead(iButtonPin);
}

inputButton::inputButton(void)
{
	NewValue.bFlagNewButton = false;
	NewValue.bButtonState = eButtonRelease;
	OldValue.bButtonState = eButtonRelease;
}

eButtonState inputButton::buttonState(void)
{
	if(NewValue.bFlagNewButton){
		//eButtonState returnValue = NewValue.bButtonState;
		//NewValue.bButtonState = eButtonHoldOff;
		NewValue.bFlagNewButton = false;
		return NewValue.bButtonState;
	}
	else{
		return eButtonStateUN;
	}
}

void inputButton::inputTask(void)
{
	vGetIOButtonValue(status());
	if(OldValue.bButtonState != NewValue.bButtonState)
	{
		OldValue.bButtonState = NewValue.bButtonState;
		NewValue.bFlagNewButton = true;
	}
}

void inputButton::vGetIOButtonValue(bool InputState)
{
	if(InputState == true)
	{
		/* Base on the last state, caculate to change state */
		switch(OldValue.bButtonState)
		{
			case eButtonPress:
				if(bHoldOff>=(int)ANTI_NOISE_RELEASE)
					NewValue.bButtonState = eButtonRelease;
				break;
			case eButtonSingleClick:
				if(bHoldOff>=(int)ANTI_NOISE_RELEASE)
					NewValue.bButtonState = eButtonRelease;
				break;
			case eButtonDoubleClick:
				if(bHoldOff>=(int)ANTI_NOISE_RELEASE)
					NewValue.bButtonState = eButtonRelease;
				break;
			case eButtonTripleClick:
				if(bHoldOff>=(int)ANTI_NOISE_RELEASE)
					NewValue.bButtonState = eButtonRelease;
				break;
			case eButtonLongPressT1:
				if(bHoldOff>=(int)ANTI_NOISE_RELEASE)
					NewValue.bButtonState = eButtonRelease;
				break;
			case eButtonLongPressT2:
				if(bHoldOff>=(int)ANTI_NOISE_RELEASE)
					NewValue.bButtonState = eButtonRelease;
				break;
			case eButtonHoldOn:
				if(bHoldOff>=(int)ANTI_NOISE_RELEASE)
					NewValue.bButtonState = eButtonRelease;
				break;
			case eButtonRelease:
				if(bHoldOff>=(int)TIME_HOLD_OFF)
					NewValue.bButtonState = eButtonHoldOff;
				break;
			case eButtonHoldOff:
				if(bHoldOff>=(int)TIME_HOLD_OFF_LONG_T1)
					NewValue.bButtonState = eButtonHoldOffLongT1;
				break;
			case eButtonHoldOffLongT1:
				if(bHoldOff>=(int)TIME_HOLD_OFF_LONG_T2)
					NewValue.bButtonState = eButtonHoldOffLongT2;
				break;
			case eButtonHoldOffLongT2:
				if(bHoldOff>=(int)TIME_HOLD_OFF_LONG)
					NewValue.bButtonState = eButtonHoldOffLong;
				break;
			case eButtonHoldOffLong:
				break;
		}
		/* Increase time Hold Off */
		if(bHoldOff<(int)TIME_HOLD_OFF_LONG){bHoldOff++;}
		/* Reset Time counter */
		uTimePress = 0;
		NewValue.bButtonTime = 0;
	}
	/* Button press */
	else if(InputState == false)
	{
		/* Base on the last state, caculate to change state */
		switch(OldValue.bButtonState)
		{
			case eButtonRelease:
				bHoldOff=0;
				if(uTimeCheck>=(int)ANTI_NOISE_PRESS)
				{
					NewValue.bButtonState = eButtonPress;
					bClickCount++;
				}
				flag_start_sample = true;
				break;
			case eButtonHoldOff:
				NewValue.bButtonState = eButtonRelease;
				break;
			case eButtonHoldOffLongT1:
				NewValue.bButtonState = eButtonRelease;
				break;
			case eButtonHoldOffLongT2:
				NewValue.bButtonState = eButtonRelease;
				break;
			case eButtonHoldOffLong:
				NewValue.bButtonState = eButtonRelease;
				break;
			}
		/* Increase counter */
		uTimePress++;
	}

	/* Time End period */
	if(flag_start_sample==true)
		uTimeCheck++;
	if(uTimeCheck>=(int)COUNT_TIME_SAMPLE)/* 10*20=200ms */
	{
		if((bClickCount==1)&&(uTimePress<=(int)TIME_SINGLE_PRESS))
			NewValue.bButtonState = eButtonSingleClick;
		if(bClickCount==2)
			NewValue.bButtonState = eButtonDoubleClick;
		if(bClickCount==3)
			NewValue.bButtonState = eButtonTripleClick;
		/* Reset counter */
		bClickCount = 0;
		/* Reset */
		uTimeCheck = 0;
		/* Set flag sample */
		flag_start_sample = false;
	}
	/* Change mode */
	if(uTimePress>=(int)TIME_HOLD_ON)
		NewValue.bButtonState = eButtonHoldOn;
	else if(uTimePress>=TIME_LONG_PRESS_T2)
		NewValue.bButtonState = eButtonLongPressT2;
	else if(uTimePress>=TIME_LONG_PRESS_T1)
		NewValue.bButtonState = eButtonLongPressT1;
	/* Update button press time */
	NewValue.bButtonTime = uTimePress;
}