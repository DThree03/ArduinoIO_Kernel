#include <IO-Kernel.h>

#define LED_PIN 2

latchRelay LatRL1(10);

void task_Kernel_IO(void *parameter)
{
  for(;;) {
    LatRL1.outputTask();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  LatRL1.init(2, 4);
  xTaskCreate(task_Kernel_IO,"Task 3",8192,NULL,1,NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    char inChar = Serial.read();
    if(inChar == 'a'){
      LatRL1.config(1-LatRL1.status());
    }
    else if(inChar == 'b')
    {
      
    }
  }
}
