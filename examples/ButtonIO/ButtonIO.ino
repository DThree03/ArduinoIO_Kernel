#include <IO-Kernel.h>

#define LED_PIN 2

inputButton START_BUT;
basicOutput LED_GR;

void task_Button_IO(void *parameter)
{
  for(;;) {
    eButtonState BTstatus = START_BUT.buttonState();
    if(BTstatus == eButtonSingleClick)
    {
      LED_GR.config(HIGH, 2, 50, false);
    }
    else if(BTstatus == eButtonDoubleClick)
    {
      LED_GR.config(HIGH, 4, 50, false);
    }
    else if(BTstatus == eButtonTripleClick)
    {
      LED_GR.config(HIGH, 6, 50, false);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void task_Kernel_IO(void *parameter)
{
  for(;;) {
    START_BUT.inputTask();
    LED_GR.outputTask();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  LED_GR.init(2);
  START_BUT.init(25, INPUT, ACTIVE_LOW);
  xTaskCreate(task_Button_IO,"Task 2",8192,NULL,1,NULL);
  xTaskCreate(task_Kernel_IO,"Task 3",8192,NULL,1,NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
}
