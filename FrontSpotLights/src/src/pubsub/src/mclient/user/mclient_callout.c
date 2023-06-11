#include <stdio.h>
#include "mclient_callout.h"
/* ------------------ USER CUSTOIMIZE ----------------------------*/
extern void FrontSpotLightsInit(void);    /* 初期化処理 */
extern void FrontSpotLightsMain(void);    /* メイン処理 */
/* ------------------ USER CUSTOIMIZE ----------------------------*/

void mclient_initcallout(void)
{
    //printf("my main init function call\n");
/* ------------------ USER CUSTOIMIZE ----------------------------*/
    FrontSpotLightsInit();
/* ------------------ USER CUSTOIMIZE ----------------------------*/
}


void mclient_callout(void)
{
    //printf("my main function call\n");
/* ------------------ USER CUSTOIMIZE ----------------------------*/
    FrontSpotLightsMain();
/* ------------------ USER CUSTOIMIZE ----------------------------*/
}
