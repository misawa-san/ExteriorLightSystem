#ifdef __cplusplus
extern "C" {
#endif

#ifndef __FRONTSPOTLIGHT_H__
#define __FRONTSPOTLIGHT_H__

#include <stdint.h>

/* メイン周期 */
#define MAIN_CYCLE  (uint32_t)(10UL)

extern volatile uint8_t payload[8];

extern void FrontSpotLightsInit(void);    /* 初期化処理 */
extern void FrontSpotLightsMain(void);    /* メイン処理 */

#endif

#ifdef __cplusplus
}
#endif
