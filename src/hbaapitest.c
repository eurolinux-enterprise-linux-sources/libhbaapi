/*
 * Description
 *	hbaapitest.c - Sample application which uses the common library API for
 *                        HBA requests.
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
 * The Original Code is  SNIA HBA API Sample Application
 *
 * The Initial Developer of the Original Code is:
 *	Dixon Hutchinson, Legato Systems, Inc. (dhutchin@legato.com)
 *
 * Contributor(s):
 *
 ***************************************************************************
 */



#include <stdio.h>
#ifndef WIN32
#include <dlfcn.h> /* Dynamic library support for Solaris */
#include <time.h>
#endif
#include "hbaapi.h"

/*** Came from JNI **********************************************************
** Utility function that treats the passed value as an array of bytes, then
** formats and prints them in ASCII Hex format.  This function will
** optionally insert spaces into the output string at specified intervals.
****************************************************************************/

void PrintHexString(void *Data, int Length, int Break)
{
    int j;
    unsigned char *pc = (unsigned char *)Data;

    for (j=1; j<=Length; j++) {
        printf("%02X", (int)*pc++);
        if ( (Break != 0) && (!(j % Break))) {
            printf(" ");
        }
    }

}
/**** Came from JNI**********************************************************
** Utility function that formats and prints the World Wide Name.  The WWN
** is assumed to be represented as a char (byte) array of length 8.
****************************************************************************/

void PrintWWN(unsigned char *pWwn)
{
    PrintHexString(pWwn, 8, 0);
}

main() {
    int i, pi, j, dpi, ti, bi;
    HBA_STATUS retval;
    HBA_UINT32 hba_cnt;
    char namebuf[1028];
    HBA_HANDLE hba_handle;
    HBA_ADAPTERATTRIBUTES hba_attrs;
    HBA_PORTATTRIBUTES hba_portattrs;
    HBA_PORTSTATISTICS hba_portstats;
    HBA_PORTATTRIBUTES hba_disc_pattrs;
#define MAPENTRIES	512
    union{
	HBA_FCPTARGETMAPPING	fcp_tmap;
	struct {
	    HBA_UINT32		cnt;
	    HBA_FCPSCSIENTRY	entry[MAPENTRIES];
	}fcp_tmapi;
    }MAPU;
#define BINDENTRIES 512
    union{
	HBA_FCPBINDING		fcp_tbind;
	struct {
	    HBA_UINT32		cnt;
	    HBA_FCPBINDINGENTRY entry[BINDENTRIES];
	}fcp_tbindi;
    }BINDU;


    printf("HBA API Library version is %d\n", HBA_GetVersion());

    retval = HBA_LoadLibrary();

    if(retval != HBA_STATUS_OK) {
	perror("HBA_LoadLibrary");
	exit(1);
    }

    hba_cnt = HBA_GetNumberOfAdapters();

    printf("Number of HBA's is %d\n", hba_cnt);
    if(hba_cnt == 0) {
	fprintf(stderr, "not enough adapters\n");
	exit(1);
    }

    for(i = 0; i < hba_cnt; i++) {
	if((retval = HBA_GetAdapterName(i, namebuf)) != HBA_STATUS_OK) {
	    fprintf(stderr, "Failure of HBA_GetAdapterName: %d\n", retval);
	    continue;
	}
	printf("\n********************************************"
		"\nAdapter number %d is named: %s\n",
		i, namebuf);
	if((hba_handle = HBA_OpenAdapter(namebuf)) == 0) {
	    fprintf(stderr, "Failure of HBA_OpenAdapter\n");
	    perror("HBA_OpenAdapter");
	    continue;
	}
	if((retval = HBA_GetAdapterAttributes(hba_handle, &hba_attrs))
	   != HBA_STATUS_OK) {
	    fprintf(stderr,
		    "Failure of HBA_GetAdapterAttributes:%d\n",
		    retval);
	    goto close;
	}
	printf("Adapter Attributes\n");
	printf("   Manufacturer:%s:\n", hba_attrs.Manufacturer);
	printf("   SerialNumber:%s:\n", hba_attrs.SerialNumber);
	printf("   Model:%s:\n", hba_attrs.Model);
	printf("   ModelDesc:%s:\n", hba_attrs.ModelDescription);
	printf("   NodeWWN:0x"); PrintWWN(hba_attrs.NodeWWN.wwn); printf("\n");
	printf("   NodeSymName:%s:\n", hba_attrs.NodeSymbolicName);
	printf("   HardwareVersion:%s:\n", hba_attrs.HardwareVersion);
	printf("   DriverVersion:%s:\n", hba_attrs.DriverVersion);
	printf("   OptionROMVersion:%s:\n", hba_attrs.OptionROMVersion);
	printf("   FirmwareVersion:%s:\n", hba_attrs.FirmwareVersion);
	printf("   VendorSpecificID:%d:\n", hba_attrs.VendorSpecificID);
	printf("   NumberOfPorts:%d:\n", hba_attrs.NumberOfPorts);
	printf("   DriverName:%s:\n", hba_attrs.DriverName);

	for(pi = 0; pi < hba_attrs.NumberOfPorts; pi++) {
	    retval = HBA_GetAdapterPortAttributes(hba_handle, pi, &hba_portattrs);
	    if(retval != HBA_STATUS_OK) {
		fprintf(stderr,
			"Failure of HBA_GetAdapterPortAttributes for pi=%d,"
			"status=%d\n",
			pi, retval);
		goto close;
	    }
	    printf("  PortIndex:%d\n", pi);
	    printf("   Adapter Port Attributes\n");
	    printf("     NodeWWN:0x");
	    PrintWWN(hba_portattrs.NodeWWN.wwn); printf("\n");
	    printf("     PortWWN:0x");
	    PrintWWN(hba_portattrs.PortWWN.wwn); printf("\n");
	    printf("     PortFcId:0x%06lX\n", hba_portattrs.PortFcId);
	    printf("     PortType:%d\n", hba_portattrs.PortType);
	    printf("     PortState:%d\n", hba_portattrs.PortState);
	    printf("     PortSupportedClassofService:%d\n",
		   hba_portattrs.PortSupportedClassofService);
	    printf("     PortSupportedFc4Types:");
	    for(j = 0; j < 32; j++) {
		printf(" %02X", hba_portattrs.PortSupportedFc4Types.bits[j]);
	    }
	    printf("\n");
	    printf("     PortActiveFc4Types:");
	    for(j = 0; j < 32; j++) {
		printf(" %02X", hba_portattrs.PortActiveFc4Types.bits[j]);
	    }
	    printf("\n");
	    printf("     PortSymbolicName:%s:\n", hba_portattrs.PortSymbolicName);
	    printf("     OSDeviceName:%s:\n", hba_portattrs.OSDeviceName);
	    printf("     PortSupportedSpeed:%d\n",
		   hba_portattrs.PortSupportedSpeed);
	    printf("     PortSpeed:%d\n", hba_portattrs.PortSpeed);
	    printf("     PortMaxFrameSize:%d\n", hba_portattrs.PortMaxFrameSize);
	    printf("     FabricName:0x");
	    PrintWWN(hba_portattrs.FabricName.wwn); printf("\n");
	    printf("     NumberofDiscoveredPorts:%d\n",
		   hba_portattrs.NumberofDiscoveredPorts);

	    retval = HBA_GetPortStatistics(hba_handle, pi, &hba_portstats);
	    if(retval != HBA_STATUS_OK && retval != HBA_STATUS_ERROR_NOT_SUPPORTED) {
		fprintf(stderr,
			"Failure of HBA_GetPortStatistics for pi=%d,"
			"status=%d\n",
			pi, retval);
		goto close;
	    }
	    printf("   Adapter Port Statistics:\n");
	    if(retval == HBA_STATUS_ERROR_NOT_SUPPORTED) {
		printf("     Port Statistics not supported by this HBA\n");
	    } else {
		printf("     SecondsSinceLastReset: %lld\n",
		       hba_portstats.SecondsSinceLastReset);
		printf("     TxFrames: %lld\n", hba_portstats.TxFrames);
		printf("     TxWords: %lld\n", hba_portstats.TxWords);
		printf("     RxFrames: %lld\n", hba_portstats.RxFrames);
		printf("     RxWords: %lld\n", hba_portstats.RxWords);
		printf("     LIPCount: %lld\n", hba_portstats.LIPCount);
		printf("     NOSCount: %lld\n", hba_portstats.NOSCount);
		printf("     ErrorFrames: %lld\n", hba_portstats.ErrorFrames);
		printf("     DumpedFrames: %lld\n", hba_portstats.DumpedFrames);
		printf("     LinkFailureCount: %lld\n",
		       hba_portstats.LinkFailureCount);
		printf("     LossOfSyncCount: %lld\n", hba_portstats.LossOfSyncCount);
		printf("     LossOfSignalCount: %lld\n",
		       hba_portstats.LossOfSignalCount);
		printf("     PrimitiveSeqProtocolErrCount: %lld\n",
		       hba_portstats.PrimitiveSeqProtocolErrCount);
		printf("     InvalidTxWordCount: %lld\n",
		       hba_portstats.InvalidTxWordCount);
		printf("     InvalidCRCCount: %lld\n", hba_portstats.InvalidCRCCount);
	    }
	    for(dpi = 0; dpi < hba_portattrs.NumberofDiscoveredPorts; dpi++) {
		retval = HBA_GetDiscoveredPortAttributes(hba_handle, pi, dpi,
							 &hba_disc_pattrs);
		if(retval != HBA_STATUS_OK) {
		    fprintf(stderr,
			    "Failure of HBA_GetDiscoveredPortAttributes\n"
			    "for pi=%d, for dpi=%d, status=%d\n",
			    pi, dpi, retval);
		    goto close;
		}
		printf("   Discovered Port Attributes (index=%d)\n", dpi);
		printf("     NodeWWN:0x");
		PrintWWN( hba_disc_pattrs.NodeWWN.wwn); printf("\n");
		printf("     PortWWN:0x");
		PrintWWN(hba_disc_pattrs.PortWWN.wwn); printf("\n");
		printf("     PortFcId:0x%06lX\n", hba_disc_pattrs.PortFcId);
		printf("     PortType:%d\n", hba_disc_pattrs.PortType);
		printf("     PortState:%d\n", hba_disc_pattrs.PortState);
		printf("     PortSupportedClassofService:%d\n",
		       hba_disc_pattrs.PortSupportedClassofService);
		printf("     PortSupportedFc4Types:");
		for(j = 0; j < 32; j++) {
		    printf(" %02X", hba_disc_pattrs.PortSupportedFc4Types.bits[j]);
		}
		printf("\n");
		printf("     PortActiveFc4Types:");
		for(j = 0; j < 32; j++) {
		    printf(" %02X", hba_disc_pattrs.PortActiveFc4Types.bits[j]);
		}
		printf("\n");
		printf("     PortSymbolicName:%s:\n", hba_disc_pattrs.PortSymbolicName);
		printf("     OSDeviceName:%s:\n", hba_disc_pattrs.OSDeviceName);
		printf("     PortSupportedSpeed:%d\n",
		       hba_disc_pattrs.PortSupportedSpeed);
		printf("     PortSpeed:%d\n", hba_disc_pattrs.PortSpeed);
		printf("     PortMaxFrameSize:%d\n", hba_disc_pattrs.PortMaxFrameSize);
		printf("     FabricName:0x");
		PrintWWN(hba_disc_pattrs.FabricName.wwn); printf("\n");
		printf("     NumberofDiscoveredPorts:%d\n",
		       hba_disc_pattrs.NumberofDiscoveredPorts);
		{

		    char large_buf[1024];
		    char large_buf2[1024];
		    retval = HBA_SendReportLUNs(hba_handle, hba_disc_pattrs.PortWWN,
						large_buf, sizeof(large_buf),
						large_buf2, sizeof(large_buf2));
		}
	    }
	}
	MAPU.fcp_tmapi.cnt = MAPENTRIES;
	retval = HBA_GetFcpTargetMapping(hba_handle, &MAPU.fcp_tmap);
	if(retval != HBA_STATUS_OK) {
	    fprintf(stderr, "failure of HBA_GetFcpTargetMapping, status=%d\n",
		    retval);
	    goto close;
	}
	printf("Target Mappings: %d\n", MAPU.fcp_tmap.NumberOfEntries);
	for(ti = 0; ti < MAPU.fcp_tmap.NumberOfEntries; ti++) {
	    printf("scsiID:%s:c%ut%us%u\n",
		   MAPU.fcp_tmap.entry[ti].ScsiId.OSDeviceName,
		   MAPU.fcp_tmap.entry[ti].ScsiId.ScsiBusNumber,
		   MAPU.fcp_tmap.entry[ti].ScsiId.ScsiTargetNumber,
		   MAPU.fcp_tmap.entry[ti].ScsiId.ScsiOSLun);
	    printf("FcpId: ID:%06x, NWWN:0x",MAPU.fcp_tmap.entry[ti].FcpId.FcId);
	    PrintWWN(MAPU.fcp_tmap.entry[ti].FcpId.NodeWWN.wwn);
	    printf(" PWWN:0x");
	    PrintWWN(MAPU.fcp_tmap.entry[ti].FcpId.PortWWN.wwn);
	    printf(" FcpLun:0x%llX\n", MAPU.fcp_tmap.entry[ti].FcpId.FcpLun);
            printf("FcpId: ID:%06x, NWWN:0x",MAPU.fcp_tmap.entry[ti].FcpId.FcId);
	    PrintWWN(MAPU.fcp_tmap.entry[ti].FcpId.NodeWWN.wwn);
	    printf(" PWWN:0x");
	    PrintWWN(MAPU.fcp_tmap.entry[ti].FcpId.PortWWN.wwn);
	    printf(" FcpLun:0x%llX\n\n",
                   MAPU.fcp_tmap.entry[ti].FcpId.FcpLun);
	}
	BINDU.fcp_tbindi.cnt = BINDENTRIES;
	retval = HBA_GetFcpPersistentBinding(hba_handle, &BINDU.fcp_tbind);
	if(retval == HBA_STATUS_OK) {
	    printf("Persistant Bindings: %d\n", BINDU.fcp_tbind.NumberOfEntries);
	    for(bi = 0; bi < BINDU.fcp_tbind.NumberOfEntries; bi++) {
		printf("BindingType:%s\n",
		       (BINDU.fcp_tbind.entry[bi].type == TO_D_ID)?
		       "TO_D_ID" : "TO_WWN");
		printf("scsiID:%s:c%ut%us%u\n",
		       BINDU.fcp_tbind.entry[bi].ScsiId.OSDeviceName,
		       BINDU.fcp_tbind.entry[bi].ScsiId.ScsiBusNumber,
		       BINDU.fcp_tbind.entry[bi].ScsiId.ScsiTargetNumber,
		       BINDU.fcp_tbind.entry[bi].ScsiId.ScsiOSLun);
		printf("FcpId: ID:%06x, NWWN:0x",
		       BINDU.fcp_tbind.entry[bi].FcpId.FcId);
		PrintWWN(BINDU.fcp_tbind.entry[bi].FcpId.NodeWWN.wwn);
		printf(" PWWN:0x");
		PrintWWN(BINDU.fcp_tbind.entry[bi].FcpId.PortWWN.wwn);
		printf(" FcpLun:0x%llX\n",
		       BINDU.fcp_tbind.entry[bi].FcpId.FcpLun);

		printf("FcId:%d\n\n", BINDU.fcp_tbind.entry[bi].FcId);

	    }
	} else if (retval == HBA_STATUS_ERROR_NOT_SUPPORTED) {
	    printf("HBA_GetFcpPersistantBinding is not supported\n");
	} else {
	    fprintf(stderr, "failure of HBA_GetFcpPersistantBinding, status=%d\n",
		    retval);
	    goto close;
	}

    close:
	HBA_CloseAdapter(hba_handle);
    }
}
