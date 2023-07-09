#include <stdio.h>
#include "mclient_callout.h"
/* ------------------ USER CUSTOIMIZE ----------------------------*/
#include "ExteriorLightController.h"
/* ------------------ USER CUSTOIMIZE ----------------------------*/

void mclient_initcallout(void)
{
    printf("my main init function call\n");
/* ------------------ USER CUSTOIMIZE ----------------------------*/
    ExterliorLightControllerInit();
/* ------------------ USER CUSTOIMIZE ----------------------------*/
}


void mclient_callout(void)
{
    printf("my main function call\n");
/* ------------------ USER CUSTOIMIZE ----------------------------*/
    ExterliorLightControllerMain();
/* ------------------ USER CUSTOIMIZE ----------------------------*/
}

