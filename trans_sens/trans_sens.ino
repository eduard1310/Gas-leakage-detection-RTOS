#include<Arduino_FreeRTOS.h>
#include<Wire.h>
#include "task.h"
#define SensePin A0

#define INCLUDE_vTaskPrioritySet 1


#define ACQUISITION_TASK_PRIORITY  ( tskIDLE_PRIORITY +1 )
#define TRANSMISSION_TASK_PRIORITY ( tskIDLE_PRIORITY )

unsigned int x =0;
unsigned int transmissionVal = 0;

TaskHandle_t TransmissionTask_Handle;
TaskHandle_t AcquisitionTask_Handle;

void AcquisitionTask(void *pvParameters)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  /*x = analogRead(SensePin);
  transmissionVal = map(x , 0 , 1023 , 0 , 100);*/
  while(1)
  {
  x = analogRead(SensePin);
  transmissionVal = map(x , 0 , 1023 , 0 , 100);

  //debugging purposes
  Serial.println(transmissionVal);
  
  xTaskDelayUntil(&xLastWakeTime , ( 1000 / portTICK_PERIOD_MS));
  } 
  
}  

void TransmissionTask(void *pvParameters)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  while(1)
  {
    Wire.beginTransmission(9); // transmit to device #9
  Wire.write(transmissionVal);              // sends x 
  Wire.endTransmission();    // stop transmitting
    xTaskDelayUntil(&xLastWakeTime , (1000 / portTICK_PERIOD_MS));
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  ////////////////////////////////////////////////////////////////////////
  xTaskCreate(AcquisitionTask , "Task citire senzor" , 255 , NULL , ACQUISITION_TASK_PRIORITY , &AcquisitionTask_Handle);
  xTaskCreate(TransmissionTask , "Task transmisie " ,255 , NULL , TRANSMISSION_TASK_PRIORITY , &TransmissionTask_Handle);
}

void loop() {}
