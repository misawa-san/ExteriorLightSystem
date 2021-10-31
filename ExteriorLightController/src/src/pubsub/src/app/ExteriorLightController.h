#ifdef __cplusplus
extern "C" {
#endif

#ifndef __EXTERIORLIGHTCONTROLLER_H__
#define __EXTERIORLIGHTCONTROLLER_H__

#include <stdint.h>

/* メイン周期 */
#define MAIN_CYCLE  (uint32_t)(20UL)

extern volatile uint8_t payload[8];

extern void ExterliorLightControllerInit(void);    /* 初期化処理 */
extern void ExterliorLightControllerMain(void);    /* メイン処理 */

#endif

#ifdef __cplusplus
}
#endif
