#include <IO-Kernel.h>

#define LED_PIN 2

basicOutput LED1;

void task_Kernel_IO(void *parameter)
{
  for(;;) {
    LED1.outputTask();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  LED1.init(LED_PIN);
  xTaskCreate(task_Kernel_IO,"Task 3",8192,NULL,1,NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    char inChar = Serial.read();
    if(inChar == 'a'){
      LED1.config(1-LED1.status(), 1, 1, false);
    }
    else if(inChar == 'b')
    {
      LED1.init(4);
      LED1.config(true, 4, 50, false);
    }
  }
}
