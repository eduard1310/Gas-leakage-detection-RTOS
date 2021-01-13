#include <Arduino_FreeRTOS.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "semphr.h"
#include "task.h"

#define LCD_TASK_PRIORITY  ( tskIDLE_PRIORITY )
#define RECEIVE_TASK_PRIORITY ( tskIDLE_PRIORITY +2)
#define BUZZER_TASK_PRIORITY ( tskIDLE_PRIORITY )


const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal LCD(rs , en , d4 ,d5 , d6 ,d7);

SemaphoreHandle_t recv_Semaphore = NULL;
TaskHandle_t recv_TaskHandle;
TaskHandle_t lcd_TaskHandle;
TaskHandle_t buzz_TaskHandle;

int recv_data = 0;


/*void buzzingTask(void *pvParameters)

{
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1)
  {
    if(recv_data > 30)
    {
      digitalWrite(10 , HIGH);
      vTaskDelay( 500 / portTICK_PERIOD_MS);
      digitalWrite(10 , LOW);
    }
    xTaskDelayUntil(xLastWakeTime , (1000 / portTICK_PERIOD_MS));
  }
}*/

void lcd_UpdateTask(void *pvParameters)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();  
  LCD.setCursor(0,0);
  LCD.print("Hello!");
  LCD.setCursor(0 , 1);
  LCD.print("Smoke per:");
  while(1)
  {
    //Serial.println("task lcd");
     
    if(recv_data > 30)
    {
      digitalWrite(10 , HIGH);
      vTaskDelay( 500 / portTICK_PERIOD_MS);
      digitalWrite(10 , LOW);
      vTaskDelay( 500 / portTICK_PERIOD_MS);
      LCD.setCursor(0,0);
      LCD.print("Warning!");
      
    }    
   else
   {
    LCD.setCursor(0,0);
    LCD.print("Hello!     ");
    LCD.setCursor(10,1);
    LCD.print(recv_data);
    LCD.setCursor(10,1);
   } 
   }    
    
    xTaskDelayUntil(&xLastWakeTime , (1000 / portTICK_PERIOD_MS));
}
 


void receiveTask(void *pvParameters)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  xSemaphoreTake(recv_Semaphore , 0 );
  
     Serial.println("sunt aici");  
  while(1)
  {
    //recv_data = Wire.read();
     Wire.onReceive(receiveChar);
    Serial.println(recv_data);
    if(receiveEvent == pdFALSE)
    {
      xSemaphoreGive(recv_Semaphore);
    }
    xTaskDelayUntil(&xLastWakeTime , (1000 / portTICK_PERIOD_MS));
  } 
  
}

void receiveChar( int bytes)
{
  recv_data = Wire.read();
}

uint8_t receiveEvent()
{  
  //recv_data = Wire.read();
  if(recv_data < 10)
  {
    return pdTRUE;
  }
  else
  {
    return pdFALSE;
  }
}

/*void buzzingEvent()
{  
  //recv_data = Wire.read();
  if(recv_data > 30)
  {
    xSemaphoreGive(buzz_Semaphore);
  }
  else
  {
    xSemaphoreTake(buzz_Semaphore , 0);
  }
}*/

void setup() {
  // put your setup code here, to run once:  
  Serial.begin(9600);
  Wire.begin(9);
  pinMode(10 ,OUTPUT);
 LCD.begin(16 , 2);

recv_Semaphore = xSemaphoreCreateBinary();
 
xTaskCreate(lcd_UpdateTask , "Task afisare" , 255 , NULL , 1 , &lcd_TaskHandle);

xTaskCreate(receiveTask , "Task receptie" , 255 , NULL , 2 , &recv_TaskHandle);

//xTaskCreate(buzzingTask , "Task receptie" , 255 , NULL , 1 , &buzz_TaskHandle);

if(recv_Semaphore == NULL)
{
  Serial.println ("Could not create inital semaphore \n");
}

}

void loop() {}
