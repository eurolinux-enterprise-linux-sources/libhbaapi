/*************************************************************************
 * Description
 *	hbasample.c - Implements a sample HBA API compliant vendor library
 *		defining the entry points for the DLL application
 *
 * License:
 *	The contents of this file are subject to the SNIA Public License
 *	Version 1.0 (the "License"); you may not use this file except in
 *	compliance with the License. You may obtain a copy of the License at
 *
 *	/http://www.snia.org/English/Resources/Code/OpenSource.html
 *
 *	Software distributed under the License is distributed on an "AS IS"
 *	basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 *	the License for the specific language governing rights and limitations
 *	under the License.
 *
 * The Original Code is  SNIA HBA API Sample Vendor Specific Library
 *
 * The Initial Developer of the Original Code is:
 *	Benjamin F. Kuo, Troika Networks, Inc. (benk@troikanetworks.com)
 *
 * Contributor(s):
 *	Tuan Lam, QLogic Corp. (t_lam@qlc.com)
 *	Dan Willie, Emulex Corp. (Dan.Willie@emulex.com)
 *	Dixon Hutchinson, Legato Systems, Inc. (dhutchin@legato.com)
 *
 *************************************************************************
 */

#ifdef WIN32
#include <windows.h>
#endif
#include <time.h>
#include "hbaapi.h"
#include "vendorhbaapi.h"
#include "hbasample.h"


/* Begin implementation */
#ifdef WIN32

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
#endif

/*
 * Two functions are exported. They both are used to register entry points.
 * Remember, this is a Rev2 Vendor specific library, but we might be
 * called by either a Rev2 or a Rev1.  The Rev1 callers will call
 * HBA_RegisterLibrary.  The Rev2 callers SHOULD call HBA_RegisterLibaryV2.
 * The size of the structure referenced in the Rev2 call is larger then in
 * the Rev1 call and the Rev1 sturcture is duplicated at the beginning of the
 * Rev2 structure
 */

/*
 * Rev1 Code will call this function and only get the Rev1 registration.
 * This prevents overrunning the data structure.
 */
HBASAMPLE_API HBA_STATUS
HBA_RegisterLibrary(HBA_ENTRYPOINTS *entrypoints ) {

    entrypoints->GetVersionHandler = Sample_GetVersion;
    entrypoints->LoadLibraryHandler = Sample_LoadLibrary;
    entrypoints->FreeLibraryHandler = Sample_FreeLibrary;
    entrypoints->GetNumberOfAdaptersHandler = Sample_GetNumberOfAdapters;
    entrypoints->GetAdapterNameHandler = Sample_GetAdapterName;
    entrypoints->OpenAdapterHandler = Sample_OpenAdapter;
    entrypoints->OpenAdapterByWWNHandler = Sample_OpenAdapterByWWN;
    entrypoints->CloseAdapterHandler = Sample_CloseAdapter;
    entrypoints->GetAdapterAttributesHandler = Sample_GetAdapterAttributes;
    entrypoints->GetAdapterPortAttributesHandler = Sample_GetAdapterPortAttributes;
    entrypoints->GetPortStatisticsHandler = Sample_GetPortStatistics;
    entrypoints->GetDiscoveredPortAttributesHandler =
	Sample_GetDiscoveredPortAttributes;
    entrypoints->GetPortAttributesByWWNHandler = Sample_GetPortAttributesByWWN;
    entrypoints->RefreshInformationHandler = Sample_RefreshInformation;
    entrypoints->RegisterForAdapterEventsHandler = Sample_RegisterForAdapterEvents;
    /* There are a lot more entry points to register, however this is just
     * busy work.  You get the idea.  */

    return 0;
}
/*
 * Here is were Rev2 callers will try first (all done by the wrapper library),
 * If the machine has not Rev2 vendor library, then obviosly, this function
 * will not be found.
 */
HBASAMPLE_API HBA_STATUS
HBA_RegisterLibraryV2(HBA_ENTRYPOINTSV2 *entrypoints) {

    /* have the old function do the Rev1 part */
    (void) HBA_RegisterLibrary((HBA_ENTRYPOINTS *)entrypoints);

    entrypoints->SendCTPassThruV2Handler = Sample_SendCTPassThruV2;
    /* so on and so forth */

    return 0
}

void
Sample_RemoveCallback() {

}

HBA_STATUS Sample_LoadLibrary() {
    /* Vendor specific intialization ...*/
    return HBA_STATUS_OK;
}

HBA_STATUS Sample_FreeLibrary() {
    /* Free up all resources any call to this library allocated */
    return HBA_STATUS_OK;
}

HBA_UINT32 Sample_GetVersion()
{
	return HBA_LIBVERSION;
}

HBA_UINT32 Sample_GetNumberOfAdapters()
{
	return 1;
}

HBA_STATUS Sample_GetAdapterName(HBA_UINT32 adapterindex, char *adaptername)
{
	if (adapterindex > 1) {
		return HBA_STATUS_ERROR_ILLEGAL_INDEX;
	} else {
		strcpy(adaptername, "org.snia.sample-1");
		return HBA_STATUS_OK;
	}
}

HBA_HANDLE
Sample_OpenAdapter(char* adaptername	){
    /*
     * WARNING WARNING, Danger Will Robinson,
     * The wrapper library will mask the return of this function to
     * the lower 16 bits.  Think of this function as returning
     * UINT16
     */
    HBA_HANDLE temp;
    temp = 0;
    if (strcmp(adaptername, "sample-adapter-1") == 0) {

    }

    return temp;
}

HBA_STATUS
Sample_OpenAdapterByWWN(HBA_HANDLE handle, HBA_WWN wwn){
    /*
     * WARNING WARNING, Danger Will Robinson,
     * The wrapper library will mask the handle of this function to
     * the lower 16 bits.  Think of this HBA_HANDLE as UINT16
     */
    HBA_HANDLE temp;
    HBA_WWN	twwn = {1,2,3,4,5,6,7,8};

    if (memcmp(wwn, twwn) == 0) {
	return HBA_STATUS_OK;
    }
    return HBA_STATUS_ERROR_ILLEGAL_WWN;

}

void Sample_CloseAdapter(
	HBA_HANDLE handle
	)
{


}

HBA_STATUS Sample_GetAdapterAttributes(
	HBA_HANDLE handle,
	PHBA_ADAPTERATTRIBUTES hbaattributes
	)
{
	strcpy(hbaattributes->Manufacturer, "Emulex");
	strcpy(hbaattributes->SerialNumber, "A12345");
	strcpy(hbaattributes->Model, "QLA2200");
	strcpy(hbaattributes->ModelDescription, "Agilent TachLite");

	hbaattributes->NodeWWN.wwn[0] = 0x01;
	hbaattributes->NodeWWN.wwn[1] = 0x02;
	hbaattributes->NodeWWN.wwn[2] = 0x03;
	hbaattributes->NodeWWN.wwn[3] = 0x04;
	hbaattributes->NodeWWN.wwn[4] = 0x05;
	hbaattributes->NodeWWN.wwn[5] = 0x06;
	hbaattributes->NodeWWN.wwn[6] = 0x07;
	hbaattributes->NodeWWN.wwn[7] = 0x08;

	strcpy(hbaattributes->NodeSymbolicName, "HBA API Sample Driver");
	strcpy(hbaattributes->HardwareVersion, "Hardware Version");
	strcpy(hbaattributes->DriverVersion, "Driver Version");
	strcpy(hbaattributes->OptionROMVersion, "Option ROM Version");
	strcpy(hbaattributes->FirmwareVersion, "Firmware Version");
	hbaattributes->VendorSpecificID = 0;
	hbaattributes->NumberOfPorts = 1;
	return HBA_STATUS_OK;
}

HBA_STATUS Sample_GetAdapterPortAttributes(
	HBA_HANDLE handle,
	HBA_UINT32 portindex,
	HBA_PORTATTRIBUTES *portattributes
	)
{
	if (portindex == 1) {
		/* WWN corresponding to 0001020304050708 */
		portattributes->NodeWWN.wwn[0] = 0x01;
		portattributes->NodeWWN.wwn[1] = 0x02;
		portattributes->NodeWWN.wwn[2] = 0x03;
		portattributes->NodeWWN.wwn[3] = 0x04;
		portattributes->NodeWWN.wwn[4] = 0x05;
		portattributes->NodeWWN.wwn[5] = 0x06;
		portattributes->NodeWWN.wwn[6] = 0x07;
		portattributes->NodeWWN.wwn[7] = 0x08;

		portattributes->PortWWN.wwn[0] = 0x01;
		portattributes->PortWWN.wwn[1] = 0x02;
		portattributes->PortWWN.wwn[2] = 0x03;
		portattributes->PortWWN.wwn[3] = 0x04;
		portattributes->PortWWN.wwn[4] = 0x05;
		portattributes->PortWWN.wwn[5] = 0x06;
		portattributes->PortWWN.wwn[6] = 0x07;
		portattributes->PortWWN.wwn[7] = 0x08;

		portattributes->PortFcId = 0x00;
		portattributes->PortType = HBA_PORTTYPE_UNKNOWN;
		portattributes->PortState = HBA_PORTSTATE_ONLINE;
		portattributes->PortSupportedClassofService = 0x00;
		portattributes->PortSupportedFc4Types.bits[0] = 0x00;
		portattributes->PortActiveFc4Types.bits[0] = 0x00;
		strcpy(portattributes->OSDeviceName, "DeviceName");
		portattributes->PortSpeed = HBA_PORTSPEED_1GBIT;
		portattributes->NumberofDiscoveredPorts = 0;
		return HBA_STATUS_OK;
	} else {
		return HBA_STATUS_ERROR_ILLEGAL_INDEX;
	}
}

HBA_STATUS Sample_GetPortStatistics(
	HBA_HANDLE				handle,
	HBA_UINT32				portindex,
	HBA_PORTSTATISTICS			*portstatistics
	)
{
	portstatistics->TxFrames = 0;
	portstatistics->RxFrames = 0;
	portstatistics->TxWords = 0;
	portstatistics->RxWords = 0;
	portstatistics->LIPCount = 0;
	portstatistics->NOSCount = 0;
	portstatistics->ErrorFrames = 0;
	portstatistics->DumpedFrames = 0;
	portstatistics->LinkFailureCount = 0;
	portstatistics->LossOfSyncCount = 0;
	portstatistics->LossOfSignalCount = 0;
	portstatistics->PrimitiveSeqProtocolErrCount = 0;
	portstatistics->InvalidTxWordCount = 0;
	portstatistics->InvalidCRCCount = 0;
	return HBA_STATUS_OK;
}

HBA_STATUS Sample_GetDiscoveredPortAttributes(
	HBA_HANDLE handle,
	HBA_UINT32 portindex,
	HBA_UINT32 discoveredportindex,
	HBA_PORTATTRIBUTES *portattributes
	)
{
	/* since this is a virtual driver with no discovered ports
	   always return an error */
	return HBA_STATUS_ERROR_ILLEGAL_INDEX;
}

HBA_STATUS Sample_GetPortAttributesByWWN(
	HBA_HANDLE handle,
	HBA_WWN PortWWN,
	HBA_PORTATTRIBUTES *portattributes
	)
{
	return HBA_STATUS_ERROR_NOT_SUPPORTED;
}

void Sample_RefreshInformation(HBA_HANDLE handle)
{
}
static void *eventhandlerdata;

HBA_STATUS Sample_RegisterForAdapterEvents(
    (void (*callback)(
	void *data,
	HBA_WWN PortWWN,
	HBA_UINT32 eventType),
     void *userData,
     HBA_HANDLE handle,
     HBA_CALLBACKHANDLE *callbackHandle)
{
    ptr = calloc();
    ptr->userdata = userData;
    ptr->callbackfunc = callback;
    /* store away a pointer to this data,
     * the low level event handler (event below)
     * will magically find this data when the
     * event is delivery is necessary
     */
    eventhandlerdata = ptr;

    /* give the caller something to remember this registion by */
    /* this can be used later to deregister the call back */
    *callbackHandle = ptr;

}

void
event()
{
    HBA_UINT64 port = 0x12345678;
    HBA_WWN PortWWN;
    ptr = eventhandlerdata;
    memcpy(&PortWWN, &port, 8);
    (*ptr->callvackfunc)(ptr->userdata, PortWWN, HBA_EVENT_ADAPTER_CHANGE);
}



/*
 * REV 2 functions
 */

HBA_STATUS Sample_HBAOpenAdapterByWWN(HBA_HANDLE *phandle, HBA_WWN wwn) {

    /* find out if it is a WWN to one of our hba's */
    /* if it is, open it */
    if(1 == 2) {
	*phandle = "123";
	return(HBA_STATUS_OK);
    } else {
	return(HBA_STATUS_ERROR_ILLEGAL_WWN);
    }
}

HBA_STATUS Sample_HBA_GetFcpTargetMapping(
    HBA_HANDLE		handle,
    HBA_FCPTARGETMAPPING
			*pmapping
    )
{
}
