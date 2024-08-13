#include "3dsDebug/types.h"
#include "3dsDebug/result.h"
#include "3dsDebug/svc.h"
#include "3dsDebug/srv.h"
#include "3dsDebug/synchronization.h"
#include "3dsDebug/services/gspgpu.h"
#include "3dsDebug/ipc.h"
#include <string.h>

#define GSP_EVENT_STACK_SIZE 0x1000

Handle gspGpuHandle;
static int gspRefCount;

Result gspInit(void)
{
	Result res=0;
	if (AtomicPostIncrement(&gspRefCount)) return 0;
	// res = srvGetServiceHandle(&gspGpuHandle, "gsp::Gpu");
    res = srvGetServiceHandleDirect(&gspGpuHandle, "gsp::Gpu");
	if (R_FAILED(res)) AtomicDecrement(&gspRefCount);
	return res;
}

Result GSPGPU_SetBufferSwap(u32 screenid, GSPGPU_FramebufferInfo* framebufinfo)
{
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x5,8,0); // 0x50200
	cmdbuf[1] = screenid;
	memcpy(&cmdbuf[2], framebufinfo, sizeof(GSPGPU_FramebufferInfo));

	Result ret=0;
	if(R_FAILED(ret=svcSendSyncRequest(gspGpuHandle)))return ret;

	return cmdbuf[1];
}