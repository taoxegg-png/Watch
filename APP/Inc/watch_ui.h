#ifndef __WATCH_UI_H
#define __WATCH_UI_H

#include "watch_data.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void WatchUI_Init(void);

    void WatchUI_Update(const WatchData_t *data);

#ifdef __cplusplus
}
#endif

#endif
