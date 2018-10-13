/*
* Wrapper for ADL, inspired by wrapnvml from John E. Stone
* 
* By Philipp Andreas - github@smurfy.de
* ADL power by Davesmacer
*/
#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum wrap_adlReturn_enum {
	WRAPADL_OK= 0
} wrap_adlReturn_t;

// Some ADL defines and structs from adl sdk
#if defined (__MSC_VER)
#define ADL_API_CALL __cdecl
#elif defined (_WIN32) || defined (__WIN32__)
#define ADL_API_CALL __stdcall
#else
#define ADL_API_CALL
#endif

typedef void* (ADL_API_CALL *ADL_MAIN_MALLOC_CALLBACK)(int);
/// \brief Handle to ADL client context.
///
///  ADL clients obtain context handle from initial call to \ref ADL2_Main_Control_Create.
///  Clients have to pass the handle to each subsequent ADL call and finally destroy
///  the context with call to \ref ADL2_Main_Control_Destroy
/// \nosubgrouping
typedef void* ADL_CONTEXT_HANDLE;

#define ADL_MAX_PATH                                    256
typedef struct AdapterInfo
{
	/// \ALL_STRUCT_MEM

	/// Size of the structure.
	int iSize;
	/// The ADL index handle. One GPU may be associated with one or two index handles
	int iAdapterIndex;
	/// The unique device ID associated with this adapter.
	char strUDID[ADL_MAX_PATH];
	/// The BUS number associated with this adapter.
	int iBusNumber;
	/// The driver number associated with this adapter.
	int iDeviceNumber;
	/// The function number.
	int iFunctionNumber;
	/// The vendor ID associated with this adapter.
	int iVendorID;
	/// Adapter name.
	char strAdapterName[ADL_MAX_PATH];
	/// Display name. For example, "\\Display0" for Windows or ":0:0" for Linux.
	char strDisplayName[ADL_MAX_PATH];
	/// Present or not; 1 if present and 0 if not present.It the logical adapter is present, the display name such as \\.\Display1 can be found from OS
	int iPresent;
	// @}

#if defined (_WIN32) || defined (_WIN64)
	/// \WIN_STRUCT_MEM

	/// Exist or not; 1 is exist and 0 is not present.
	int iExist;
	/// Driver registry path.
	char strDriverPath[ADL_MAX_PATH];
	/// Driver registry path Ext for.
	char strDriverPathExt[ADL_MAX_PATH];
	/// PNP string from Windows.
	char strPNPString[ADL_MAX_PATH];
	/// It is generated from EnumDisplayDevices.
	int iOSDisplayIndex;
	// @}
#endif /* (_WIN32) || (_WIN64) */

#if defined (LINUX)
	/// \LNX_STRUCT_MEM

	/// Internal X screen number from GPUMapInfo (DEPRICATED use XScreenInfo)
	int iXScreenNum;
	/// Internal driver index from GPUMapInfo
	int iDrvIndex;
	/// \deprecated Internal x config file screen identifier name. Use XScreenInfo instead.
	char strXScreenConfigName[ADL_MAX_PATH];

	// @}
#endif /* (LINUX) */
} AdapterInfo, *LPAdapterInfo;

typedef struct ADLTemperature
{
	/// Must be set to the size of the structure
	int iSize;
	/// Temperature in millidegrees Celsius.
	int iTemperature;
} ADLTemperature;

typedef struct ADLFanSpeedValue
{
	/// Must be set to the size of the structure
	int iSize;
	/// Possible valies: \ref ADL_DL_FANCTRL_SPEED_TYPE_PERCENT or \ref ADL_DL_FANCTRL_SPEED_TYPE_RPM
	int iSpeedType;
	/// Fan speed value
	int iFanSpeed;
	/// The only flag for now is: \ref ADL_DL_FANCTRL_FLAG_USER_DEFINED_SPEED
	int iFlags;
} ADLFanSpeedValue;

/*
* Handle to hold the function pointers for the entry points we need,
* and the shared library itself.
*/
typedef struct {
	void *adl_dll;
	int adl_gpucount;
	int log_gpucount;
	int opencl_gpucount;
	int *phys_logi_device_id;
	LPAdapterInfo devs;
	ADL_CONTEXT_HANDLE context;
	int *adl_opencl_device_id;          /* map ADL dev to OPENCL dev */
	int *opencl_adl_device_id;          /* map OPENCL dev to ADL dev */
	wrap_adlReturn_t(*adlMainControlCreate)(ADL_MAIN_MALLOC_CALLBACK, int);
	wrap_adlReturn_t(*adlAdapterNumberOfAdapters)(int *);
	wrap_adlReturn_t(*adlAdapterAdapterInfoGet)(LPAdapterInfo, int);
	wrap_adlReturn_t(*adlAdapterAdapterIdGet)(int, int*);
	wrap_adlReturn_t(*adlOverdrive5TemperatureGet)(int, int, ADLTemperature*);
	wrap_adlReturn_t(*adlOverdrive5FanSpeedGet)(int, int, ADLFanSpeedValue*);
	wrap_adlReturn_t(*adlMainControlRefresh)(void);
	wrap_adlReturn_t(*adlMainControlDestroy)(void);
	wrap_adlReturn_t(*adl2MainControlCreate)(ADL_MAIN_MALLOC_CALLBACK, int, ADL_CONTEXT_HANDLE* );
	wrap_adlReturn_t(*adl2MainControlDestroy)(ADL_CONTEXT_HANDLE);
	wrap_adlReturn_t(*adl2Overdrive6CurrentPowerGet)(ADL_CONTEXT_HANDLE, int, int, int*);
	wrap_adlReturn_t(*adl2MainControlRefresh)(ADL_CONTEXT_HANDLE);
} wrap_adl_handle;

wrap_adl_handle * wrap_adl_create();
int wrap_adl_destroy(wrap_adl_handle *adlh);

int wrap_adl_get_gpucount(wrap_adl_handle *adlh, int *gpucount);

int wrap_adl_get_gpu_name(wrap_adl_handle *adlh, int gpuindex, char *namebuf, int bufsize);

int wrap_adl_get_gpu_pci_id(wrap_adl_handle *adlh, int gpuindex, char *idbuf, int bufsize);

int wrap_adl_get_tempC(wrap_adl_handle *adlh, int gpuindex, unsigned int *tempC);

int wrap_adl_get_fanpcnt(wrap_adl_handle *adlh, int gpuindex, unsigned int *fanpcnt);

int wrap_adl_get_power_usage(wrap_adl_handle *adlh, int gpuindex, unsigned int *milliwatts);


#if defined(__cplusplus)
}
#endif

