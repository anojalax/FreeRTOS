/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Demo includes. */
#include "ParTest.h"

/* Hardware includes. */
#include "stm320518_eval.h"

/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void user1( void *pvParameters );
static void user2( void *pvParameters );
static void access_precious_resource(void);

void main_mutex( void );


/*-----------------------------------------------------------*/

xSemaphoreHandle gatekeeper = 0;
int howLongToWait = 1000;
/*-----------------------------------------------------------*/

void main_mutex( void )
{

  	gatekeeper = xSemaphoreCreateMutex();
	vParTestToggleLED( LED1);
	xTaskCreate( user1,					/* The function that implements the task. */
				( signed char * ) "T1", 				/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				configMINIMAL_STACK_SIZE, 				/* The size of the stack to allocate to the task. */
				NULL, /* The parameter passed to the task - just to check the functionality. */
				1, 	  /* The priority assigned to the task. */
				NULL );									/* The task handle is not required, so NULL is passed. */

	xTaskCreate( user2,					/* The function that implements the task. */
			( signed char * ) "T2", 				/* The text name assigned to the task - for debug only as it is not used by the kernel. */
			configMINIMAL_STACK_SIZE, 				/* The size of the stack to allocate to the task. */
			NULL, /* The parameter passed to the task - just to check the functionality. */
			1, 	  /* The priority assigned to the task. */
			NULL );		
		
	/* Start the tasks and timer running. */
	vTaskStartScheduler();


	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details. */
	for( ;; );
}
/*-----------------------------------------------------------*/


static void user1( void *pvParameters )
{
	for( ;; )
	{
	  if(xSemaphoreTake(gatekeeper, howLongToWait)){
		vParTestToggleLED( LED1);
		access_precious_resource();
		xSemaphoreGive(gatekeeper);
	  }
	  else{
		vParTestSetLED( LED1, 0 );
	  }
	  vTaskDelay(1000);
	}
}
/*-----------------------------------------------------------*/

static void user2( void *pvParameters )
{
	for( ;; )
	{
	  if(xSemaphoreTake(gatekeeper, howLongToWait)){
		vParTestToggleLED( LED2);
		access_precious_resource();
		xSemaphoreGive(gatekeeper);
	  }
	  else{
		vParTestSetLED( LED2, 0 );
	  }
	  vTaskDelay(1000);
	}
}

static void access_precious_resource(void){
  // This could be a SPI, I2C or any other peripheral access
  return;
}
/*-----------------------------------------------------------*/

