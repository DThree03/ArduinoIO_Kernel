#ifndef	_DATA_IO_TYPEDEF_H_
#define _DATA_IO_TYPEDEF_H_

#define ANTI_NOISE_PRESS		2	/* 2 cycle of Button Process 20ms */
#define ANTI_NOISE_RELEASE		2	/* 2 cycle of Button Process 20ms */
#define TIME_SINGLE_PRESS		10	/* 10 cycle of Button Process 100ms */
#define TIME_LONG_PRESS_T1		200	/* 100 cycle of Button Process 1s */
#define TIME_LONG_PRESS_T2 		300	/* 200 cycle of Button Process ~ 2000ms = 2s */
#define TIME_HOLD_ON 			600	/* 600 cycle of Button Process ~ 6000ms = 6s */
#define TIME_HOLD_OFF 			10	/* 10 cycle of Button Process ~ 100ms */
#define TIME_HOLD_OFF_LONG_T1	100	/* 100 cycle of Button Process ~ 1s */
#define TIME_HOLD_OFF_LONG_T2	200	/* 200 cycle of Button Process ~ 2s */
#define TIME_HOLD_OFF_LONG		400	/* 400 cycle of Button Process ~ 4s */

#define COUNT_TIME_SAMPLE		40

#define ACTIVE_LOW				0x01
#define ACTIVE_HIGH				0x02
/* Enum button state */
typedef enum
{
    eButtonPress			= 1,
    eButtonRelease 			= 2,
	eButtonLongPressT1		= 3,
	eButtonLongPressT2		= 4,
	eButtonHoldOn			= 5,
	eButtonDoubleClick		= 6,
	eButtonSingleClick		= 7,
	eButtonTripleClick		= 8,
	eButtonHoldOff			= 9,
    eButtonHoldOffLongT1	= 10,
    eButtonHoldOffLongT2	= 11,
    eButtonHoldOffLong  	= 12,
	eButtonStateUN			= 0xff,
}eButtonState;

/* Struct manage button */
typedef struct{
    eButtonState	bButtonState;
	unsigned long	bButtonTime;
	bool 			bFlagNewButton;
}structIO_Button;

typedef struct{
	uint32_t	uFrequency;
	uint32_t	uCountToggle;
	uint32_t	uCycleCounter;
	bool	    bStartState;
	bool	    bCurrentProcess;
	bool	    bFlagStart;
}structIO_Manage_Output;

typedef struct{
	uint32_t	uTime;		/* Frequency Toggle 	*/
	uint32_t	uCycleCounter;	/* Time Cycle Counter	*/
	bool		bRLState;		/* Start State 			*/
	bool		bCurrentProcess;/* Flag Process 		*/
	bool		bFlagStart;		/* Flag start 			*/
}structIO_Manage_LatRL;

#endif