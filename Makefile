DEBUG = -g
CFLAGS = -fPIC
LIB_DIR = $(RPM_BUILD_ROOT)/opt/zhcp/lib
BIN_DIR = $(RPM_BUILD_ROOT)/opt/zhcp/bin
LD_DIR = $(RPM_BUILD_ROOT)/etc/ld.so.conf.d
SRC_DIR = zhcp/src/
INCLUDE_DIR = zhcp/include/
ZHCPLIB = "libzhcp.so"

#-----------------------------------------------------------------------
# Objects for utils
#-----------------------------------------------------------------------
UTILS_OBJECTS += rcdescriptions.o
UTILS_OBJECTS += wrapperutils.o

#-----------------------------------------------------------------------
# Objects for sm library
#-----------------------------------------------------------------------
SM_OBJECTS += smSocket.o 
SM_OBJECTS += smMemoryGroup.o 
SM_OBJECTS += smapiUtilities.o 
SM_OBJECTS += smapiTableParser.o 

#-----------------------------------------------------------------------
# Objects for vmapi library
#-----------------------------------------------------------------------
VMAPI_OBJECTS += vmapiAsynchronousNotificationDisableDm.o 
VMAPI_OBJECTS += vmapiAsynchronousNotificationEnableDm.o 
VMAPI_OBJECTS += vmapiAsynchronousNotificationQueryDm.o 
VMAPI_OBJECTS += vmapiAuthorizationListAdd.o 
VMAPI_OBJECTS += vmapiAuthorizationListQuery.o 
VMAPI_OBJECTS += vmapiAuthorizationListRemove.o
VMAPI_OBJECTS += vmapiCheckAuthentication.o
VMAPI_OBJECTS += vmapiDeleteABENDDump.o 
VMAPI_OBJECTS += vmapiEventStreamAdd.o  
VMAPI_OBJECTS += vmapiEventUnsubscribe.o 
VMAPI_OBJECTS += vmapiDirectoryManagerLocalTagDefineDm.o 
VMAPI_OBJECTS += vmapiDirectoryManagerLocalTagDeleteDm.o 
VMAPI_OBJECTS += vmapiDirectoryManagerLocalTagQueryDm.o 
VMAPI_OBJECTS += vmapiDirectoryManagerLocalTagSetDm.o 
VMAPI_OBJECTS += vmapiDirectoryManagerSearchDm.o 
VMAPI_OBJECTS += vmapiDirectoryManagerTaskCancelDm.o 
VMAPI_OBJECTS += vmapiImageActivate.o 
VMAPI_OBJECTS += vmapiImageActiveConfigurationQuery.o 
VMAPI_OBJECTS += vmapiImageConsoleGet.o
VMAPI_OBJECTS += vmapiImageCpuDefine.o 
VMAPI_OBJECTS += vmapiImageCpuDefineDm.o 
VMAPI_OBJECTS += vmapiImageCpuDelete.o 
VMAPI_OBJECTS += vmapiImageCpuDeleteDm.o 
VMAPI_OBJECTS += vmapiImageCpuQuery.o 
VMAPI_OBJECTS += vmapiImageCpuQueryDm.o 
VMAPI_OBJECTS += vmapiImageCpuSetMaximumDm.o 
VMAPI_OBJECTS += vmapiImageCreateDm.o 
VMAPI_OBJECTS += vmapiImageDeactivate.o
VMAPI_OBJECTS += vmapiImageDefinitionAsyncUpdates.o
VMAPI_OBJECTS += vmapiImageDefinitionCreateDm.o
VMAPI_OBJECTS += vmapiImageDefinitionDeleteDm.o
VMAPI_OBJECTS += vmapiImageDefinitionQueryDm.o
VMAPI_OBJECTS += vmapiImageDefinitionUpdateDm.o
VMAPI_OBJECTS += vmapiImageDeleteDm.o 
VMAPI_OBJECTS += vmapiImageDeviceDedicate.o 
VMAPI_OBJECTS += vmapiImageDeviceDedicateDm.o 
VMAPI_OBJECTS += vmapiImageDeviceReset.o 
VMAPI_OBJECTS += vmapiImageDeviceUndedicate.o 
VMAPI_OBJECTS += vmapiImageDeviceUndedicateDm.o 
VMAPI_OBJECTS += vmapiImageDiskCopy.o 
VMAPI_OBJECTS += vmapiImageDiskCopyDm.o
VMAPI_OBJECTS += vmapiImageDiskCreate.o 
VMAPI_OBJECTS += vmapiImageDiskCreateDm.o 
VMAPI_OBJECTS += vmapiImageDiskDelete.o 
VMAPI_OBJECTS += vmapiImageDiskDeleteDm.o 
VMAPI_OBJECTS += vmapiImageDiskQuery.o 
VMAPI_OBJECTS += vmapiImageDiskShare.o 
VMAPI_OBJECTS += vmapiImageDiskShareDm.o 
VMAPI_OBJECTS += vmapiImageDiskUnshare.o 
VMAPI_OBJECTS += vmapiImageDiskUnshareDm.o 
VMAPI_OBJECTS += vmapiImageIplDeleteDm.o 
VMAPI_OBJECTS += vmapiImageIplDeviceQuery.o 
VMAPI_OBJECTS += vmapiImageIplQueryDm.o 
VMAPI_OBJECTS += vmapiImageIplSetDm.o 
VMAPI_OBJECTS += vmapiImageLockDm.o 
VMAPI_OBJECTS += vmapiImageLockQueryDm.o
VMAPI_OBJECTS += vmapiImageMdiskLinkQuery.o
VMAPI_OBJECTS += vmapiImageNameQueryDm.o 
VMAPI_OBJECTS += vmapiImagePasswordSetDm.o 
VMAPI_OBJECTS += vmapiImagePause.o 
VMAPI_OBJECTS += vmapiImagePerformanceQuery.o 
VMAPI_OBJECTS += vmapiImageQueryActivateTime.o 
VMAPI_OBJECTS += vmapiImageQueryDm.o 
VMAPI_OBJECTS += vmapiImageRecycle.o 
VMAPI_OBJECTS += vmapiImageReplaceDm.o 
VMAPI_OBJECTS += vmapiImageScsiCharacteristicsDefineDm.o 
VMAPI_OBJECTS += vmapiImageScsiCharacteristicsQueryDm.o 
VMAPI_OBJECTS += vmapiImageStatusQuery.o 
VMAPI_OBJECTS += vmapiImageUnlockDm.o 
VMAPI_OBJECTS += vmapiImageVolumeAdd.o 
VMAPI_OBJECTS += vmapiImageVolumeDelete.o 
VMAPI_OBJECTS += vmapiImageVolumeShare.o 
VMAPI_OBJECTS += vmapiImageVolumeSpaceDefineDm.o 
VMAPI_OBJECTS += vmapiImageVolumeSpaceDefineExtendedDm.o 
VMAPI_OBJECTS += vmapiImageVolumeSpaceQueryDm.o 
VMAPI_OBJECTS += vmapiImageVolumeSpaceQueryExtendedDm.o 
VMAPI_OBJECTS += vmapiImageVolumeSpaceRemoveDm.o 
VMAPI_OBJECTS += vmapiIPaddrGet.o 
VMAPI_OBJECTS += vmapiMetadataDelete.o 
VMAPI_OBJECTS += vmapiMetadataGet.o 
VMAPI_OBJECTS += vmapiMetadataSet.o 
VMAPI_OBJECTS += vmapiNameListAdd.o 
VMAPI_OBJECTS += vmapiNameListDestroy.o 
VMAPI_OBJECTS += vmapiNameListQuery.o 
VMAPI_OBJECTS += vmapiNameListRemove.o 
VMAPI_OBJECTS += vmapiNetworkIPInterfaceCreate.o
VMAPI_OBJECTS += vmapiNetworkIPInterfaceModify.o
VMAPI_OBJECTS += vmapiNetworkIPInterfaceQuery.o
VMAPI_OBJECTS += vmapiNetworkIPInterfaceRemove.o
VMAPI_OBJECTS += vmapiPageOrSpoolVolumeAdd.o 
VMAPI_OBJECTS += vmapiProcessAbendDump.o
VMAPI_OBJECTS += vmapiProfileCreateDm.o 
VMAPI_OBJECTS += vmapiProfileDeleteDm.o 
VMAPI_OBJECTS += vmapiProfileLockDm.o 
VMAPI_OBJECTS += vmapiProfileLockQueryDm.o
VMAPI_OBJECTS += vmapiProfileQueryDm.o 
VMAPI_OBJECTS += vmapiProfileReplaceDm.o 
VMAPI_OBJECTS += vmapiProfileUnlockDm.o 
VMAPI_OBJECTS += vmapiPrototypeCreateDm.o
VMAPI_OBJECTS += vmapiPrototypeDeleteDm.o 
VMAPI_OBJECTS += vmapiPrototypeNameQueryDm.o 
VMAPI_OBJECTS += vmapiPrototypeQueryDm.o 
VMAPI_OBJECTS += vmapiPrototypeReplaceDm.o 
VMAPI_OBJECTS += vmapiQueryAbendDump.o 
VMAPI_OBJECTS += vmapiQueryAllDmNo.o
VMAPI_OBJECTS += vmapiQueryAllDmYes.o
VMAPI_OBJECTS += vmapiQueryApiFunctionalLevel.o 
VMAPI_OBJECTS += vmapiQueryAsynchronousOperationDm.o 
VMAPI_OBJECTS += vmapiQueryDirectoryManagerLevelDm.o 
VMAPI_OBJECTS += vmapiResponseRecovery.o
VMAPI_OBJECTS += vmapiSharedMemoryAccessAddDm.o 
VMAPI_OBJECTS += vmapiSharedMemoryAccessQueryDm.o 
VMAPI_OBJECTS += vmapiSharedMemoryAccessRemoveDm.o 
VMAPI_OBJECTS += vmapiSharedMemoryCreate.o 
VMAPI_OBJECTS += vmapiSharedMemoryDelete.o 
VMAPI_OBJECTS += vmapiSharedMemoryQuery.o 
VMAPI_OBJECTS += vmapiSharedMemoryReplace.o
VMAPI_OBJECTS += vmapiSMAPIStatusCapture.o 
VMAPI_OBJECTS += vmapiSSIQuery.o
VMAPI_OBJECTS += vmapiStaticImageChangesActivateDm.o 
VMAPI_OBJECTS += vmapiStaticImageChangesDeactivateDm.o 
VMAPI_OBJECTS += vmapiStaticImageChangesImmediateDm.o 
VMAPI_OBJECTS += vmapiSystemConfigSyntaxCheck.o 
VMAPI_OBJECTS += vmapiSystemDiskAccessibility.o 
VMAPI_OBJECTS += vmapiSystemDiskAdd.o
VMAPI_OBJECTS += vmapiSystemDiskIoQuery.o
VMAPI_OBJECTS += vmapiSystemDiskQuery.o
VMAPI_OBJECTS += vmapiSystemEQIDQuery.o
VMAPI_OBJECTS += vmapiSystemFCPFreeQuery.o
VMAPI_OBJECTS += vmapiSystemInfoQuery.o 
VMAPI_OBJECTS += vmapiSystemInformationQuery.o
VMAPI_OBJECTS += vmapiSystemIoQuery.o 
VMAPI_OBJECTS += vmapiSystemPageUtilizationQuery.o
VMAPI_OBJECTS += vmapiSystemPerformanceInfoQuery.o
VMAPI_OBJECTS += vmapiSystemPerformanceInformationQuery.o
VMAPI_OBJECTS += vmapiSystemPerformanceThresholdDisable.o
VMAPI_OBJECTS += vmapiSystemPerformanceThresholdEnable.o 
VMAPI_OBJECTS += vmapiSystemSCSIDiskAdd.o
VMAPI_OBJECTS += vmapiSystemSCSIDiskDelete.o
VMAPI_OBJECTS += vmapiSystemSCSIDiskQuery.o
VMAPI_OBJECTS += vmapiSystemServiceQuery.o
VMAPI_OBJECTS += vmapiSystemShutdown.o
VMAPI_OBJECTS += vmapiSystemSpoolUtilizationQuery.o
VMAPI_OBJECTS += vmapiSystemWWPNQuery.o
VMAPI_OBJECTS += vmapiVirtualChannelConnectionCreate.o 
VMAPI_OBJECTS += vmapiVirtualChannelConnectionCreateDm.o 
VMAPI_OBJECTS += vmapiVirtualChannelConnectionDelete.o 
VMAPI_OBJECTS += vmapiVirtualChannelConnectionDeleteDm.o 
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterConnectLan.o 
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterConnectLanDm.o 
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterConnectVswitch.o 
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterConnectVswitchDm.o 
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterConnectVswitchExtended.o
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterCreate.o 
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterCreateDm.o 
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterCreateExtended.o
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterCreateExtendedDm.o
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterDelete.o 
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterDeleteDm.o 
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterDisconnect.o 
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterDisconnectDm.o 
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterQuery.o 
VMAPI_OBJECTS += vmapiVirtualNetworkAdapterQueryExtended.o 
VMAPI_OBJECTS += vmapiVirtualNetworkLanAccess.o 
VMAPI_OBJECTS += vmapiVirtualNetworkLanAccessQuery.o 
VMAPI_OBJECTS += vmapiVirtualNetworkLanCreate.o 
VMAPI_OBJECTS += vmapiVirtualNetworkLanDelete.o 
VMAPI_OBJECTS += vmapiVirtualNetworkLanQuery.o 
VMAPI_OBJECTS += vmapiVirtualNetworkOsaQuery.o 
VMAPI_OBJECTS += vmapiVirtualNetworkVLANQueryStats.o
VMAPI_OBJECTS += vmapiVirtualNetworkQueryLan.o 
VMAPI_OBJECTS += vmapiVirtualNetworkQueryOsa.o 
VMAPI_OBJECTS += vmapiVirtualNetworkVswitchCreate.o 
VMAPI_OBJECTS += vmapiVirtualNetworkVswitchCreateExtended.o 
VMAPI_OBJECTS += vmapiVirtualNetworkVswitchDelete.o 
VMAPI_OBJECTS += vmapiVirtualNetworkVswitchDeleteExtended.o 
VMAPI_OBJECTS += vmapiVirtualNetworkVswitchQuery.o 
VMAPI_OBJECTS += vmapiVirtualNetworkVswitchQueryExtended.o
VMAPI_OBJECTS += vmapiVirtualNetworkVswitchQueryIUOStats.o
VMAPI_OBJECTS += vmapiVirtualNetworkVswitchQueryStats.o 
VMAPI_OBJECTS += vmapiVirtualNetworkVswitchSet.o 
VMAPI_OBJECTS += vmapiVirtualNetworkVswitchSetExtended.o
VMAPI_OBJECTS += vmapiVMRelocate.o
VMAPI_OBJECTS += vmapiVMRelocateImageAttributes.o
VMAPI_OBJECTS += vmapiVMRelocateModify.o
VMAPI_OBJECTS += vmapiVMRelocateStatus.o
VMAPI_OBJECTS += vmapiVmrmConfigurationQuery.o 
VMAPI_OBJECTS += vmapiVmrmConfigurationUpdate.o 
VMAPI_OBJECTS += vmapiVmrmMeasurementQuery.o
VMAPI_OBJECTS += vmapiXcatCommandsIUO.o

#-----------------------------------------------------------------------
# Objects for smcli
#-----------------------------------------------------------------------
SMCLI_OBJECTS += smcli.o
SMCLI_OBJECTS += smcliAsynchronous.o
SMCLI_OBJECTS += smcliAuthorization.o
SMCLI_OBJECTS += smcliCheck.o
SMCLI_OBJECTS += smcliDelete.o
SMCLI_OBJECTS += smcliDirectory.o
SMCLI_OBJECTS += smcliEvent.o
SMCLI_OBJECTS += smcliImage.o
SMCLI_OBJECTS += smcliImageDefinition.o
SMCLI_OBJECTS += smcliMetadata.o
SMCLI_OBJECTS += smcliName.o
SMCLI_OBJECTS += smcliNetwork.o
SMCLI_OBJECTS += smcliPage.o
SMCLI_OBJECTS += smcliProcess.o
SMCLI_OBJECTS += smcliProfile.o
SMCLI_OBJECTS += smcliPrototype.o
SMCLI_OBJECTS += smcliQuery.o
SMCLI_OBJECTS += smcliResponse.o
SMCLI_OBJECTS += smcliShared.o
SMCLI_OBJECTS += smcliSMAPI.o
SMCLI_OBJECTS += smcliSSI.o
SMCLI_OBJECTS += smcliStatic.o
SMCLI_OBJECTS += smcliSystem.o
SMCLI_OBJECTS += smcliVirtual.o
SMCLI_OBJECTS += smcliVMRelocate.o
SMCLI_OBJECTS += smcliVMRM.o
SMCLI_OBJECTS += smcliUnDocumented.o

#-----------------------------------------------------------------------
# Objects for cikada
#-----------------------------------------------------------------------
CIKADA_OBJECTS += ckddecode.o
CIKADA_OBJECTS += ckdencode.o
CIKADA_OBJECTS += ckdhex.o


.PHONY: all install post clean
#-----------------------------------------------------------------------
# All
#-----------------------------------------------------------------------
all: $(ZHCPLIB) \
    $(CIKADA_OBJECTS) \
    $(SMCLI_OBJECTS) \
    $(UTILS_OBJECTS)  \
    add3390 \
    add3390active \
    add9336 \
    adddisk2pool \
    addvdisk \
    addprocessor \
    addnic \
    ckddecode \
    ckdencode \
    ckdhex \
    connectnic2guestlan \
    connectnic2vswitch \
    createuserprofile \
    createvs \
    deleteipl \
    deleteuserprofile \
    deletevs \
    dedicatedevice \
    disconnectnic \
    getuserentrynames \
    getuserentry \
    getipl \
    getuserprofile \
    getdiskpool \
    getdiskpoolnames \
    queryvolumes \
    removediskfrompool \
    removemdisk \
    removenic \
    removeprocessor \
    replaceuserprofile \
    replacevs \
    setipl \
    setpassword \
    startvs \
    stopvs \
    syncfileutil \
    undedicatedevice \
    smcli\
    iucvserver\
    iucvclient

#------------------------------------------------------------------------------------------------------------------------------------------------------
# Utils object files
#------------------------------------------------------------------------------------------------------------------------------------------------------
rcdescriptions.o : $(SRC_DIR)rcdescriptions.c
    gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)rcdescriptions.c

wrapperutils.o : $(SRC_DIR)wrapperutils.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)wrapperutils.c

#------------------------------------------------------------------------------------------------------------------------------------------------------
# sm object files
#------------------------------------------------------------------------------------------------------------------------------------------------------
smSocket.o : $(SRC_DIR)smSocket.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smSocket.c

smMemoryGroup.o : $(SRC_DIR)smMemoryGroup.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smMemoryGroup.c

smapiUtilities.o : $(SRC_DIR)smapiUtilities.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smapiUtilities.c

smapiTableParser.o : $(SRC_DIR)smapiTableParser.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smapiTableParser.c

#------------------------------------------------------------------------------------------------------------------------------------------------------
# vmapi object files
#------------------------------------------------------------------------------------------------------------------------------------------------------
vmapiAsynchronousNotificationDisableDm.o : $(SRC_DIR)vmapiAsynchronousNotificationDisableDm.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiAsynchronousNotificationDisableDm.c

vmapiAsynchronousNotificationEnableDm.o : $(SRC_DIR)vmapiAsynchronousNotificationEnableDm.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiAsynchronousNotificationEnableDm.c

vmapiAsynchronousNotificationQueryDm.o : $(SRC_DIR)vmapiAsynchronousNotificationQueryDm.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiAsynchronousNotificationQueryDm.c

vmapiAuthorizationListAdd.o : $(SRC_DIR)vmapiAuthorizationListAdd.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiAuthorizationListAdd.c

vmapiAuthorizationListQuery.o : $(SRC_DIR)vmapiAuthorizationListQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiAuthorizationListQuery.c

vmapiAuthorizationListRemove.o : $(SRC_DIR)vmapiAuthorizationListRemove.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiAuthorizationListRemove.c

vmapiCheckAuthentication.o : $(SRC_DIR)vmapiCheckAuthentication.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiCheckAuthentication.c

vmapiDeleteABENDDump.o : $(SRC_DIR)vmapiDeleteABENDDump.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiDeleteABENDDump.c

vmapiDirectoryManagerLocalTagDefineDm.o : $(SRC_DIR)vmapiDirectoryManagerLocalTagDefineDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiDirectoryManagerLocalTagDefineDm.c

vmapiDirectoryManagerLocalTagDeleteDm.o : $(SRC_DIR)vmapiDirectoryManagerLocalTagDeleteDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiDirectoryManagerLocalTagDeleteDm.c

vmapiDirectoryManagerLocalTagQueryDm.o : $(SRC_DIR)vmapiDirectoryManagerLocalTagQueryDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiDirectoryManagerLocalTagQueryDm.c

vmapiDirectoryManagerLocalTagSetDm.o : $(SRC_DIR)vmapiDirectoryManagerLocalTagSetDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiDirectoryManagerLocalTagSetDm.c

vmapiDirectoryManagerSearchDm.o : $(SRC_DIR)vmapiDirectoryManagerSearchDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiDirectoryManagerSearchDm.c

vmapiDirectoryManagerTaskCancelDm.o : $(SRC_DIR)vmapiDirectoryManagerTaskCancelDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiDirectoryManagerTaskCancelDm.c

vmapiEventStreamAdd.o : $(SRC_DIR)vmapiEventStreamAdd.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiEventStreamAdd.c

vmapiEventUnsubscribe.o : $(SRC_DIR)vmapiEventUnsubscribe.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiEventUnsubscribe.c

vmapiImageActivate.o : $(SRC_DIR)vmapiImageActivate.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageActivate.c

vmapiImageActiveConfigurationQuery.o : $(SRC_DIR)vmapiImageActiveConfigurationQuery.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageActiveConfigurationQuery.c

vmapiImageConsoleGet.o : $(SRC_DIR)vmapiImageConsoleGet.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageConsoleGet.c

vmapiImageCpuDefine.o : $(SRC_DIR)vmapiImageCpuDefine.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageCpuDefine.c

vmapiImageCpuDefineDm.o : $(SRC_DIR)vmapiImageCpuDefineDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageCpuDefineDm.c

vmapiImageCpuDelete.o : $(SRC_DIR)vmapiImageCpuDelete.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageCpuDelete.c

vmapiImageCpuDeleteDm.o : $(SRC_DIR)vmapiImageCpuDeleteDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageCpuDeleteDm.c

vmapiImageCpuQuery.o : $(SRC_DIR)vmapiImageCpuQuery.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageCpuQuery.c

vmapiImageCpuQueryDm.o : $(SRC_DIR)vmapiImageCpuQueryDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageCpuQueryDm.c

vmapiImageCpuSetMaximumDm.o : $(SRC_DIR)vmapiImageCpuSetMaximumDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageCpuSetMaximumDm.c

vmapiImageCreateDm.o : $(SRC_DIR)vmapiImageCreateDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageCreateDm.c

vmapiImageDeactivate.o : $(SRC_DIR)vmapiImageDeactivate.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDeactivate.c
    
vmapiImageDefinitionAsyncUpdates.o : $(SRC_DIR)vmapiImageDefinitionAsyncUpdates.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDefinitionAsyncUpdates.c    
    
vmapiImageDefinitionCreateDm.o : $(SRC_DIR)vmapiImageDefinitionCreateDm.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDefinitionCreateDm.c
    
vmapiImageDefinitionDeleteDm.o : $(SRC_DIR)vmapiImageDefinitionDeleteDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDefinitionDeleteDm.c
    
vmapiImageDefinitionQueryDm.o : $(SRC_DIR)vmapiImageDefinitionQueryDm.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDefinitionQueryDm.c
    
vmapiImageDefinitionUpdateDm.o : $(SRC_DIR)vmapiImageDefinitionUpdateDm.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDefinitionUpdateDm.c

vmapiImageDeleteDm.o : $(SRC_DIR)vmapiImageDeleteDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDeleteDm.c

vmapiImageDeviceDedicate.o : $(SRC_DIR)vmapiImageDeviceDedicate.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDeviceDedicate.c

vmapiImageDeviceDedicateDm.o : $(SRC_DIR)vmapiImageDeviceDedicateDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDeviceDedicateDm.c

vmapiImageDeviceReset.o : $(SRC_DIR)vmapiImageDeviceReset.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDeviceReset.c

vmapiImageDeviceUndedicate.o : $(SRC_DIR)vmapiImageDeviceUndedicate.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDeviceUndedicate.c

vmapiImageDeviceUndedicateDm.o : $(SRC_DIR)vmapiImageDeviceUndedicateDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDeviceUndedicateDm.c

vmapiImageDiskCopy.o : $(SRC_DIR)vmapiImageDiskCopy.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDiskCopy.c

vmapiImageDiskCopyDm.o : $(SRC_DIR)vmapiImageDiskCopyDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDiskCopyDm.c

vmapiImageDiskCreate.o : $(SRC_DIR)vmapiImageDiskCreate.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDiskCreate.c

vmapiImageDiskCreateDm.o : $(SRC_DIR)vmapiImageDiskCreateDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDiskCreateDm.c

vmapiImageDiskDelete.o : $(SRC_DIR)vmapiImageDiskDelete.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDiskDelete.c

vmapiImageDiskDeleteDm.o : $(SRC_DIR)vmapiImageDiskDeleteDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDiskDeleteDm.c

vmapiImageDiskQuery.o : $(SRC_DIR)vmapiImageDiskQuery.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDiskQuery.c

vmapiImageDiskShare.o : $(SRC_DIR)vmapiImageDiskShare.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDiskShare.c

vmapiImageDiskShareDm.o : $(SRC_DIR)vmapiImageDiskShareDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDiskShareDm.c

vmapiImageDiskUnshare.o : $(SRC_DIR)vmapiImageDiskUnshare.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDiskUnshare.c

vmapiImageDiskUnshareDm.o : $(SRC_DIR)vmapiImageDiskUnshareDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageDiskUnshareDm.c

vmapiImageIplDeleteDm.o : $(SRC_DIR)vmapiImageIplDeleteDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageIplDeleteDm.c

vmapiImageIplDeviceQuery.o  : $(SRC_DIR)vmapiImageIplDeviceQuery.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageIplDeviceQuery.c

vmapiImageIplQueryDm.o : $(SRC_DIR)vmapiImageIplQueryDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageIplQueryDm.c

vmapiImageIplSetDm.o : $(SRC_DIR)vmapiImageIplSetDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageIplSetDm.c

vmapiImageLockDm.o : $(SRC_DIR)vmapiImageLockDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageLockDm.c
	
vmapiImageLockQueryDm.o : $(SRC_DIR)vmapiImageLockQueryDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageLockQueryDm.c	

vmapiImageMdiskLinkQuery.o : $(SRC_DIR)vmapiImageMdiskLinkQuery.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageMdiskLinkQuery.c

vmapiImageNameQueryDm.o : $(SRC_DIR)vmapiImageNameQueryDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageNameQueryDm.c

vmapiImagePasswordSetDm.o : $(SRC_DIR)vmapiImagePasswordSetDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImagePasswordSetDm.c
	
vmapiImagePause.o : $(SRC_DIR)vmapiImagePause.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImagePause.c

vmapiImagePerformanceQuery.o : $(SRC_DIR)vmapiImagePerformanceQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImagePerformanceQuery.c

vmapiImageQueryActivateTime.o : $(SRC_DIR)vmapiImageQueryActivateTime.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageQueryActivateTime.c

vmapiImageQueryDm.o : $(SRC_DIR)vmapiImageQueryDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageQueryDm.c

vmapiImageRecycle.o : $(SRC_DIR)vmapiImageRecycle.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageRecycle.c

vmapiImageReplaceDm.o : $(SRC_DIR)vmapiImageReplaceDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageReplaceDm.c

vmapiImageScsiCharacteristicsDefineDm.o : $(SRC_DIR)vmapiImageScsiCharacteristicsDefineDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageScsiCharacteristicsDefineDm.c

vmapiImageScsiCharacteristicsQueryDm.o : $(SRC_DIR)vmapiImageScsiCharacteristicsQueryDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageScsiCharacteristicsQueryDm.c

vmapiImageStatusQuery.o : $(SRC_DIR)vmapiImageStatusQuery.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageStatusQuery.c

vmapiImageUnlockDm.o : $(SRC_DIR)vmapiImageUnlockDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageUnlockDm.c

vmapiImageVolumeAdd.o : $(SRC_DIR)vmapiImageVolumeAdd.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageVolumeAdd.c

vmapiImageVolumeDelete.o : $(SRC_DIR)vmapiImageVolumeDelete.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageVolumeDelete.c

vmapiImageVolumeShare.o : $(SRC_DIR)vmapiImageVolumeShare.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageVolumeShare.c

vmapiImageVolumeSpaceDefineDm.o : $(SRC_DIR)vmapiImageVolumeSpaceDefineDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageVolumeSpaceDefineDm.c
    
vmapiImageVolumeSpaceDefineExtendedDm.o : $(SRC_DIR)vmapiImageVolumeSpaceDefineExtendedDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageVolumeSpaceDefineExtendedDm.c    

vmapiImageVolumeSpaceQueryDm.o : $(SRC_DIR)vmapiImageVolumeSpaceQueryDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageVolumeSpaceQueryDm.c

vmapiImageVolumeSpaceQueryExtendedDm.o : $(SRC_DIR)vmapiImageVolumeSpaceQueryExtendedDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageVolumeSpaceQueryExtendedDm.c    

vmapiImageVolumeSpaceRemoveDm.o : $(SRC_DIR)vmapiImageVolumeSpaceRemoveDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiImageVolumeSpaceRemoveDm.c

vmapiIPaddrGet.o : $(SRC_DIR)vmapiIPaddrGet.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiIPaddrGet.c

vmapiMetadataDelete.o : $(SRC_DIR)vmapiMetadataDelete.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiMetadataDelete.c
	
vmapiMetadataGet.o : $(SRC_DIR)vmapiMetadataGet.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiMetadataGet.c
	
vmapiMetadataSet.o : $(SRC_DIR)vmapiMetadataSet.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiMetadataSet.c		

vmapiNameListAdd.o : $(SRC_DIR)vmapiNameListAdd.c $(INCLUDE_DIR)vmapiName.h  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiNameListAdd.c

vmapiNameListDestroy.o : $(SRC_DIR)vmapiNameListDestroy.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiNameListDestroy.c

vmapiNameListQuery.o : $(SRC_DIR)vmapiNameListQuery.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiNameListQuery.c

vmapiNameListRemove.o : $(SRC_DIR)vmapiNameListRemove.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiNameListRemove.c

vmapiNetworkIPInterfaceCreate.o : $(SRC_DIR)vmapiNetworkIPInterfaceCreate.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiNetworkIPInterfaceCreate.c
	
vmapiNetworkIPInterfaceModify.o : $(SRC_DIR)vmapiNetworkIPInterfaceModify.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiNetworkIPInterfaceModify.c

vmapiNetworkIPInterfaceQuery.o : $(SRC_DIR)vmapiNetworkIPInterfaceQuery.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiNetworkIPInterfaceQuery.c

vmapiNetworkIPInterfaceRemove.o : $(SRC_DIR)vmapiNetworkIPInterfaceRemove.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiNetworkIPInterfaceRemove.c

vmapiPageOrSpoolVolumeAdd.o : $(SRC_DIR)vmapiPageOrSpoolVolumeAdd.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiPageOrSpoolVolumeAdd.c

vmapiProcessAbendDump.o : $(SRC_DIR)vmapiProcessAbendDump.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiProcessAbendDump.c

vmapiProfileCreateDm.o : $(SRC_DIR)vmapiProfileCreateDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiProfileCreateDm.c

vmapiProfileDeleteDm.o : $(SRC_DIR)vmapiProfileDeleteDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiProfileDeleteDm.c
	
vmapiProfileLockDm.o : $(SRC_DIR)vmapiProfileLockDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiProfileLockDm.c

vmapiProfileLockQueryDm.o : $(SRC_DIR)vmapiProfileLockQueryDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiProfileLockQueryDm.c
	
vmapiProfileQueryDm.o : $(SRC_DIR)vmapiProfileQueryDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiProfileQueryDm.c

vmapiProfileReplaceDm.o : $(SRC_DIR)vmapiProfileReplaceDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiProfileReplaceDm.c

vmapiPrototypeCreateDm.o : $(SRC_DIR)vmapiPrototypeCreateDm.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiPrototypeCreateDm.c

vmapiProfileUnlockDm.o : $(SRC_DIR)vmapiProfileUnlockDm.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiProfileUnlockDm.c

vmapiPrototypeDeleteDm.o : $(SRC_DIR)vmapiPrototypeDeleteDm.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiPrototypeDeleteDm.c

vmapiPrototypeNameQueryDm.o : $(SRC_DIR)vmapiPrototypeNameQueryDm.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiPrototypeNameQueryDm.c

vmapiPrototypeQueryDm.o : $(SRC_DIR)vmapiPrototypeQueryDm.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiPrototypeQueryDm.c

vmapiPrototypeReplaceDm.o : $(SRC_DIR)vmapiPrototypeReplaceDm.c  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiPrototypeReplaceDm.c

vmapiQueryAbendDump.o : $(SRC_DIR)vmapiQueryAbendDump.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiQueryAbendDump.c

vmapiQueryAllDmNo.o : $(SRC_DIR)vmapiQueryAllDmNo.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiQueryAllDmNo.c

vmapiQueryAllDmYes.o : $(SRC_DIR)vmapiQueryAllDmYes.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiQueryAllDmYes.c
	
vmapiQueryApiFunctionalLevel.o : $(SRC_DIR)vmapiQueryApiFunctionalLevel.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiQueryApiFunctionalLevel.c

vmapiQueryAsynchronousOperationDm.o : $(SRC_DIR)vmapiQueryAsynchronousOperationDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiQueryAsynchronousOperationDm.c

vmapiQueryDirectoryManagerLevelDm.o : $(SRC_DIR)vmapiQueryDirectoryManagerLevelDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiQueryDirectoryManagerLevelDm.c
	
vmapiResponseRecovery.o : $(SRC_DIR)vmapiResponseRecovery.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiResponseRecovery.c

vmapiSharedMemoryAccessAddDm.o : $(SRC_DIR)vmapiSharedMemoryAccessAddDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSharedMemoryAccessAddDm.c

vmapiSharedMemoryAccessQueryDm.o : $(SRC_DIR)vmapiSharedMemoryAccessQueryDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSharedMemoryAccessQueryDm.c

vmapiSharedMemoryAccessRemoveDm.o : $(SRC_DIR)vmapiSharedMemoryAccessRemoveDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSharedMemoryAccessRemoveDm.c

vmapiSharedMemoryCreate.o : $(SRC_DIR)vmapiSharedMemoryCreate.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSharedMemoryCreate.c

vmapiSharedMemoryDelete.o : $(SRC_DIR)vmapiSharedMemoryDelete.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSharedMemoryDelete.c

vmapiSharedMemoryQuery.o : $(SRC_DIR)vmapiSharedMemoryQuery.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSharedMemoryQuery.c
	
vmapiSMAPIStatusCapture.o : $(SRC_DIR)vmapiSMAPIStatusCapture.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSMAPIStatusCapture.c	

vmapiSharedMemoryReplace.o : $(SRC_DIR)vmapiSharedMemoryReplace.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSharedMemoryReplace.c

vmapiSSIQuery.o : $(SRC_DIR)vmapiSSIQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSSIQuery.c

vmapiStaticImageChangesActivateDm.o : $(SRC_DIR)vmapiStaticImageChangesActivateDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiStaticImageChangesActivateDm.c

vmapiStaticImageChangesDeactivateDm.o : $(SRC_DIR)vmapiStaticImageChangesDeactivateDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiStaticImageChangesDeactivateDm.c

vmapiStaticImageChangesImmediateDm.o : $(SRC_DIR)vmapiStaticImageChangesImmediateDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiStaticImageChangesImmediateDm.c

vmapiSystemConfigSyntaxCheck.o : $(SRC_DIR)vmapiSystemConfigSyntaxCheck.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemConfigSyntaxCheck.c

vmapiSystemDiskAccessibility.o : $(SRC_DIR)vmapiSystemDiskAccessibility.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemDiskAccessibility.c
    
vmapiSystemDiskAdd.o : $(SRC_DIR)vmapiSystemDiskAdd.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemDiskAdd.c

vmapiSystemDiskIoQuery.o : $(SRC_DIR)vmapiSystemDiskIoQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemDiskIoQuery.c

vmapiSystemDiskQuery.o : $(SRC_DIR)vmapiSystemDiskQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemDiskQuery.c

vmapiSystemEQIDQuery.o : $(SRC_DIR)vmapiSystemEQIDQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemEQIDQuery.c

vmapiSystemFCPFreeQuery.o : $(SRC_DIR)vmapiSystemFCPFreeQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemFCPFreeQuery.c

vmapiSystemInfoQuery.o : $(SRC_DIR)vmapiSystemInfoQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemInfoQuery.c

vmapiSystemInformationQuery.o : $(SRC_DIR)vmapiSystemInformationQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemInformationQuery.c

vmapiSystemIoQuery.o : $(SRC_DIR)vmapiSystemIoQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemIoQuery.c

vmapiSystemPageUtilizationQuery.o : $(SRC_DIR)vmapiSystemPageUtilizationQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemPageUtilizationQuery.c

vmapiSystemPerformanceInfoQuery.o : $(SRC_DIR)vmapiSystemPerformanceInfoQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemPerformanceInfoQuery.c

vmapiSystemPerformanceInformationQuery.o : $(SRC_DIR)vmapiSystemPerformanceInformationQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemPerformanceInformationQuery.c
	
vmapiSystemPerformanceThresholdDisable.o : $(SRC_DIR)vmapiSystemPerformanceThresholdDisable.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemPerformanceThresholdDisable.c        

vmapiSystemPerformanceThresholdEnable.o : $(SRC_DIR)vmapiSystemPerformanceThresholdEnable.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemPerformanceThresholdEnable.c

vmapiSystemSCSIDiskAdd.o : $(SRC_DIR)vmapiSystemSCSIDiskAdd.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemSCSIDiskAdd.c
    
vmapiSystemSCSIDiskDelete.o : $(SRC_DIR)vmapiSystemSCSIDiskDelete.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemSCSIDiskDelete.c

vmapiSystemSCSIDiskQuery.o : $(SRC_DIR)vmapiSystemSCSIDiskQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemSCSIDiskQuery.c
        
vmapiSystemServiceQuery.o : $(SRC_DIR)vmapiSystemServiceQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemServiceQuery.c 
	       
vmapiSystemShutdown.o : $(SRC_DIR)vmapiSystemShutdown.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemShutdown.c 
       
vmapiSystemSpoolUtilizationQuery.o : $(SRC_DIR)vmapiSystemSpoolUtilizationQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemSpoolUtilizationQuery.c     
        
vmapiSystemWWPNQuery.o : $(SRC_DIR)vmapiSystemWWPNQuery.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiSystemWWPNQuery.c

vmapiVirtualChannelConnectionCreate.o : $(SRC_DIR)vmapiVirtualChannelConnectionCreate.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualChannelConnectionCreate.c

vmapiVirtualChannelConnectionCreateDm.o : $(SRC_DIR)vmapiVirtualChannelConnectionCreateDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualChannelConnectionCreateDm.c

vmapiVirtualChannelConnectionDelete.o : $(SRC_DIR)vmapiVirtualChannelConnectionDelete.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualChannelConnectionDelete.c

vmapiVirtualChannelConnectionDeleteDm.o : $(SRC_DIR)vmapiVirtualChannelConnectionDeleteDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualChannelConnectionDeleteDm.c

vmapiVirtualNetworkAdapterConnectLan.o : $(SRC_DIR)vmapiVirtualNetworkAdapterConnectLan.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterConnectLan.c

vmapiVirtualNetworkAdapterConnectLanDm.o : $(SRC_DIR)vmapiVirtualNetworkAdapterConnectLanDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterConnectLanDm.c

vmapiVirtualNetworkAdapterConnectVswitch.o : $(SRC_DIR)vmapiVirtualNetworkAdapterConnectVswitch.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterConnectVswitch.c

vmapiVirtualNetworkAdapterConnectVswitchDm.o : $(SRC_DIR)vmapiVirtualNetworkAdapterConnectVswitchDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterConnectVswitchDm.c

vmapiVirtualNetworkAdapterConnectVswitchExtended.o : $(SRC_DIR)vmapiVirtualNetworkAdapterConnectVswitchExtended.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterConnectVswitchExtended.c
    
vmapiVirtualNetworkAdapterCreate.o : $(SRC_DIR)vmapiVirtualNetworkAdapterCreate.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterCreate.c

vmapiVirtualNetworkAdapterCreateDm.o : $(SRC_DIR)vmapiVirtualNetworkAdapterCreateDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterCreateDm.c

vmapiVirtualNetworkAdapterCreateExtended.o : $(SRC_DIR)vmapiVirtualNetworkAdapterCreateExtended.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterCreateExtended.c

vmapiVirtualNetworkAdapterCreateExtendedDm.o : $(SRC_DIR)vmapiVirtualNetworkAdapterCreateExtendedDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterCreateExtendedDm.c
    
vmapiVirtualNetworkAdapterDelete.o : $(SRC_DIR)vmapiVirtualNetworkAdapterDelete.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterDelete.c

vmapiVirtualNetworkAdapterDeleteDm.o : $(SRC_DIR)vmapiVirtualNetworkAdapterDeleteDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterDeleteDm.c

vmapiVirtualNetworkAdapterDisconnect.o : $(SRC_DIR)vmapiVirtualNetworkAdapterDisconnect.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterDisconnect.c

vmapiVirtualNetworkAdapterDisconnectDm.o : $(SRC_DIR)vmapiVirtualNetworkAdapterDisconnectDm.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterDisconnectDm.c

vmapiVirtualNetworkAdapterQuery.o : $(SRC_DIR)vmapiVirtualNetworkAdapterQuery.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterQuery.c

vmapiVirtualNetworkAdapterQueryExtended.o : $(SRC_DIR)vmapiVirtualNetworkAdapterQueryExtended.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkAdapterQueryExtended.c

vmapiVirtualNetworkLanAccess.o : $(SRC_DIR)vmapiVirtualNetworkLanAccess.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkLanAccess.c

vmapiVirtualNetworkLanAccessQuery.o : $(SRC_DIR)vmapiVirtualNetworkLanAccessQuery.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkLanAccessQuery.c

vmapiVirtualNetworkLanCreate.o : $(SRC_DIR)vmapiVirtualNetworkLanCreate.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkLanCreate.c

vmapiVirtualNetworkLanDelete.o : $(SRC_DIR)vmapiVirtualNetworkLanDelete.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkLanDelete.c

vmapiVirtualNetworkLanQuery.o : $(SRC_DIR)vmapiVirtualNetworkLanQuery.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkLanQuery.c

vmapiVirtualNetworkOsaQuery.o : $(SRC_DIR)vmapiVirtualNetworkOsaQuery.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkOsaQuery.c
    
vmapiVirtualNetworkQueryLan.o : $(SRC_DIR)vmapiVirtualNetworkQueryLan.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkQueryLan.c

vmapiVirtualNetworkQueryOsa.o : $(SRC_DIR)vmapiVirtualNetworkQueryOsa.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkQueryOsa.c
    
vmapiVirtualNetworkVLANQueryStats.o : $(SRC_DIR)vmapiVirtualNetworkVLANQueryStats.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkVLANQueryStats.c    

vmapiVirtualNetworkVswitchCreate.o : $(SRC_DIR)vmapiVirtualNetworkVswitchCreate.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkVswitchCreate.c

vmapiVirtualNetworkVswitchCreateExtended.o : $(SRC_DIR)vmapiVirtualNetworkVswitchCreateExtended.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkVswitchCreateExtended.c

vmapiVirtualNetworkVswitchDelete.o : $(SRC_DIR)vmapiVirtualNetworkVswitchDelete.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkVswitchDelete.c

vmapiVirtualNetworkVswitchDeleteExtended.o : $(SRC_DIR)vmapiVirtualNetworkVswitchDeleteExtended.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkVswitchDeleteExtended.c

vmapiVirtualNetworkVswitchQuery.o : $(SRC_DIR)vmapiVirtualNetworkVswitchQuery.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkVswitchQuery.c

vmapiVirtualNetworkVswitchQueryExtended.o : $(SRC_DIR)vmapiVirtualNetworkVswitchQueryExtended.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkVswitchQueryExtended.c

vmapiVirtualNetworkVswitchQueryIUOStats.o : $(SRC_DIR)vmapiVirtualNetworkVswitchQueryIUOStats.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkVswitchQueryIUOStats.c

vmapiVirtualNetworkVswitchQueryStats.o : $(SRC_DIR)vmapiVirtualNetworkVswitchQueryStats.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkVswitchQueryStats.c

vmapiVirtualNetworkVswitchSet.o : $(SRC_DIR)vmapiVirtualNetworkVswitchSet.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkVswitchSet.c

vmapiVirtualNetworkVswitchSetExtended.o : $(SRC_DIR)vmapiVirtualNetworkVswitchSetExtended.c   
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVirtualNetworkVswitchSetExtended.c
    
vmapiVMRelocate.o : $(SRC_DIR)vmapiVMRelocate.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVMRelocate.c
    
vmapiVMRelocateImageAttributes.o : $(SRC_DIR)vmapiVMRelocateImageAttributes.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVMRelocateImageAttributes.c
    
vmapiVMRelocateModify.o : $(SRC_DIR)vmapiVMRelocateModify.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVMRelocateModify.c
    
vmapiVMRelocateStatus.o : $(SRC_DIR)vmapiVMRelocateStatus.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVMRelocateStatus.c    

vmapiVmrmConfigurationQuery.o : $(SRC_DIR)vmapiVmrmConfigurationQuery.c $(INCLUDE_DIR)vmapiVMRM.h  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVmrmConfigurationQuery.c

vmapiVmrmConfigurationUpdate.o : $(SRC_DIR)vmapiVmrmConfigurationUpdate.c $(INCLUDE_DIR)vmapiVMRM.h  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVmrmConfigurationUpdate.c

vmapiVmrmMeasurementQuery.o : $(SRC_DIR)vmapiVmrmMeasurementQuery.c $(INCLUDE_DIR)vmapiVMRM.h  
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiVmrmMeasurementQuery.c
    
vmapiXcatCommandsIUO.o : $(SRC_DIR)vmapiXcatCommandsIUO.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)vmapiXcatCommandsIUO.c

#------------------------------------------------------------------------------------------------------------------------------------------------------
# smcli object files
#------------------------------------------------------------------------------------------------------------------------------------------------------
smcli.o : $(SRC_DIR)smcli.c 
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcli.c
smcliAsynchronous.o : $(SRC_DIR)smcliAsynchronous.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliAsynchronous.c
smcliAuthorization.o : $(SRC_DIR)smcliAuthorization.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliAuthorization.c
smcliCheck.o : $(SRC_DIR)smcliCheck.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliCheck.c
smcliDelete.o : $(SRC_DIR)smcliDelete.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliDelete.c    
smcliDirectory.o : $(SRC_DIR)smcliDirectory.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliDirectory.c    
smcliEvent.o : $(SRC_DIR)smcliEvent.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliEvent.c    
smcliImage.o : $(SRC_DIR)smcliImage.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliImage.c    
smcliImageDefinition.o : $(SRC_DIR)smcliImageDefinition.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliImageDefinition.c
smcliMetadata.o : $(SRC_DIR)smcliMetadata.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliMetadata.c    
smcliName.o : $(SRC_DIR)smcliName.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliName.c  
smcliNetwork.o : $(SRC_DIR)smcliNetwork.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliNetwork.c	  
smcliPage.o : $(SRC_DIR)smcliPage.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliPage.c    
smcliProcess.o : $(SRC_DIR)smcliProcess.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliProcess.c    
smcliProfile.o : $(SRC_DIR)smcliProfile.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliProfile.c    
smcliPrototype.o : $(SRC_DIR)smcliPrototype.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliPrototype.c        
smcliQuery.o : $(SRC_DIR)smcliQuery.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliQuery.c    
smcliResponse.o : $(SRC_DIR)smcliResponse.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliResponse.c    
smcliShared.o : $(SRC_DIR)smcliShared.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliShared.c    
smcliSMAPI.o : $(SRC_DIR)smcliSMAPI.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliSMAPI.c	
smcliSSI.o : $(SRC_DIR)smcliSSI.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliSSI.c        
smcliStatic.o : $(SRC_DIR)smcliStatic.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliStatic.c    
smcliSystem.o : $(SRC_DIR)smcliSystem.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliSystem.c    
smcliVirtual.o : $(SRC_DIR)smcliVirtual.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliVirtual.c    
smcliVMRelocate.o : $(SRC_DIR)smcliVMRelocate.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliVMRelocate.c    
smcliVMRM.o : $(SRC_DIR)smcliVMRM.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliVMRM.c    
smcliUnDocumented.o : $(SRC_DIR)smcliUnDocumented.c
	gcc -c $(CFLAGS) $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)smcliUnDocumented.c
 
#------------------------------------------------------------------------------------------------------------------------------------------------------
# CIKADA object files
#------------------------------------------------------------------------------------------------------------------------------------------------------
ckddecode.o : $(SRC_DIR)ckddecode.c
	gcc -c -Wall -std=c99 -O2 $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)ckddecode.c 
ckdencode.o : $(SRC_DIR)ckdencode.c
	gcc -c -Wall -std=c99 -O2 $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)ckdencode.c 
ckdhex.o : $(SRC_DIR)ckdhex.c
	gcc -c -Wall -std=c99 -O2 $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)ckdhex.c

#------------------------------------------------------------------------------------------------------------------------------------------------------
# IUCV files
#------------------------------------------------------------------------------------------------------------------------------------------------------
iucvserver : iucvserver.o
	gcc -o iucvserver   iucvserver.o
iucvserver.o : $(SRC_DIR)/IUCV/iucvserver.c
	gcc -c  $(DEBUG) -I$(INCLUDE_DIR)IUCV/ $(SRC_DIR)IUCV/iucvserver.c

iucvclient : iucvclient.o
	gcc -o iucvclient   iucvclient.o
iucvclient.o : $(SRC_DIR)/IUCV/iucvclient.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR)IUCV/ $(SRC_DIR)IUCV/iucvclient.c 

#-----------------------------------------------------------------------
# Libraries
#-----------------------------------------------------------------------
$(ZHCPLIB) : $(SM_OBJECTS) $(UTILS_OBJECTS) $(VMAPI_OBJECTS) 
	gcc -Wl,-d -shared -pthread -ldl -o $(ZHCPLIB) $(SM_OBJECTS) $(UTILS_OBJECTS) $(VMAPI_OBJECTS)

#-----------------------------------------------------------------------
# Executables
#-----------------------------------------------------------------------
add3390 : add3390.o $(ZHCPLIB)
	gcc -o add3390  -L./ -lzhcp $(UTILS_OBJECTS) add3390.o
add3390.o : $(SRC_DIR)add3390.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)add3390.c

add3390active : add3390active.o $(ZHCPLIB)
	gcc -o add3390active  -L./ -lzhcp $(UTILS_OBJECTS) add3390active.o
add3390active.o : $(SRC_DIR)add3390active.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)add3390active.c

add9336 : add9336.o $(ZHCPLIB)
	gcc -o add9336  -L./ -lzhcp $(UTILS_OBJECTS) add9336.o
add9336.o : $(SRC_DIR)add9336.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)add9336.c

adddisk2pool : adddisk2pool.o $(ZHCPLIB)
	gcc -o adddisk2pool  -L./ -lzhcp $(UTILS_OBJECTS) adddisk2pool.o
adddisk2pool.o : $(SRC_DIR)adddisk2pool.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)adddisk2pool.c
    
addvdisk : addvdisk.o $(ZHCPLIB)
	gcc -o addvdisk  -L./ -lzhcp $(UTILS_OBJECTS) addvdisk.o
addvdisk.o : $(SRC_DIR)addvdisk.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)addvdisk.c

addprocessor : addprocessor.o $(ZHCPLIB)
	gcc -o addprocessor  -L./ -lzhcp $(UTILS_OBJECTS) addprocessor.o
addprocessor.o : $(SRC_DIR)addprocessor.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)addprocessor.c

addnic : addnic.o $(ZHCPLIB)
	gcc -o addnic  -L./ -lzhcp $(UTILS_OBJECTS) addnic.o
addnic.o : $(SRC_DIR)addnic.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)addnic.c

ckddecode : ckddecode.o 
	gcc -o ckddecode  -Wall  ckddecode.o

ckdencode : ckdencode.o 
	gcc -o ckdencode  -Wall  ckdencode.o

ckdhex : ckdhex.o 
	gcc -o ckdhex  -Wall  ckdhex.o
connectnic2guestlan : connectnic2guestlan.o $(ZHCPLIB)
	gcc -o connectnic2guestlan  -L./ -lzhcp $(UTILS_OBJECTS) connectnic2guestlan.o
connectnic2guestlan.o : $(SRC_DIR)connectnic2guestlan.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)connectnic2guestlan.c

connectnic2vswitch : connectnic2vswitch.o $(ZHCPLIB)
	gcc -o connectnic2vswitch  -L./ -lzhcp $(UTILS_OBJECTS) connectnic2vswitch.o
connectnic2vswitch.o : $(SRC_DIR)connectnic2vswitch.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)connectnic2vswitch.c

createuserprofile : createuserprofile.o $(ZHCPLIB)
	gcc -o createuserprofile  -L./ -lzhcp $(UTILS_OBJECTS) createuserprofile.o
createuserprofile.o : $(SRC_DIR)createuserprofile.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)createuserprofile.c
    
createvs : createvs.o $(ZHCPLIB)
	gcc -o createvs  -L./ -lzhcp $(UTILS_OBJECTS) createvs.o
createvs.o : $(SRC_DIR)createvs.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)createvs.c

deleteuserprofile : deleteuserprofile.o $(ZHCPLIB)
	gcc -o deleteuserprofile  -L./ -lzhcp $(UTILS_OBJECTS) deleteuserprofile.o
deleteuserprofile.o : $(SRC_DIR)deleteuserprofile.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)deleteuserprofile.c

deletevs : deletevs.o $(ZHCPLIB)
	gcc -o deletevs  -L./ -lzhcp $(UTILS_OBJECTS) deletevs.o
deletevs.o : $(SRC_DIR)deletevs.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)deletevs.c
    
dedicatedevice : dedicatedevice.o $(ZHCPLIB)
	gcc -o dedicatedevice -L./ -lzhcp $(UTILS_OBJECTS) dedicatedevice.o
dedicatedevice.o : $(SRC_DIR)dedicatedevice.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)dedicatedevice.c

deleteipl : deleteipl.o $(ZHCPLIB)
	gcc -o deleteipl  -L./ -lzhcp $(UTILS_OBJECTS) deleteipl.o
deleteipl.o : $(SRC_DIR)deleteipl.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)deleteipl.c

disconnectnic : disconnectnic.o $(ZHCPLIB)
	gcc -o disconnectnic  -L./ -lzhcp $(UTILS_OBJECTS) disconnectnic.o
disconnectnic.o : $(SRC_DIR)disconnectnic.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)disconnectnic.c

getuserentrynames : getuserentrynames.o $(ZHCPLIB)
	gcc -o getuserentrynames  -L./ -lzhcp $(UTILS_OBJECTS) getuserentrynames.o
getuserentrynames.o : $(SRC_DIR)getuserentrynames.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)getuserentrynames.c

getuserentry : getuserentry.o $(ZHCPLIB)
	gcc -o getuserentry -L./ -lzhcp $(UTILS_OBJECTS) getuserentry.o
getuserentry.o : $(SRC_DIR)getuserentry.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)getuserentry.c

getipl : getipl.o $(ZHCPLIB)
	gcc -o getipl  -L./ -lzhcp $(UTILS_OBJECTS) getipl.o
getipl.o : $(SRC_DIR)getipl.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)getipl.c

getuserprofile : getuserprofile.o $(ZHCPLIB)
	gcc -o getuserprofile -L./ -lzhcp $(UTILS_OBJECTS) getuserprofile.o
getuserprofile.o : $(SRC_DIR)getuserprofile.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)getuserprofile.c

replaceuserprofile : replaceuserprofile.o $(ZHCPLIB)
	gcc -o replaceuserprofile -L./ -lzhcp $(UTILS_OBJECTS) replaceuserprofile.o
replaceuserprofile.o : $(SRC_DIR)replaceuserprofile.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)replaceuserprofile.c

getdiskpool : getdiskpool.o $(ZHCPLIB)
	gcc -o getdiskpool  -L./ -lzhcp $(UTILS_OBJECTS) getdiskpool.o
getdiskpool.o : $(SRC_DIR)getdiskpool.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)getdiskpool.c

getdiskpoolnames : getdiskpoolnames.o $(ZHCPLIB)
	gcc -o getdiskpoolnames  -L./ -lzhcp $(UTILS_OBJECTS) getdiskpoolnames.o
getdiskpoolnames.o : $(SRC_DIR)getdiskpoolnames.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)getdiskpoolnames.c

queryvolumes : queryvolumes.o $(ZHCPLIB)
	gcc -o queryvolumes -L./ -lzhcp $(UTILS_OBJECTS) queryvolumes.o
queryvolumes.o : $(SRC_DIR)queryvolumes.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)queryvolumes.c

removediskfrompool : removediskfrompool.o $(ZHCPLIB)
	gcc -o removediskfrompool  -L./ -lzhcp $(UTILS_OBJECTS) removediskfrompool.o
removediskfrompool.o : $(SRC_DIR)removediskfrompool.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)removediskfrompool.c
    
removemdisk : removemdisk.o $(ZHCPLIB)
	gcc -o removemdisk  -L./ -lzhcp $(UTILS_OBJECTS) removemdisk.o
removemdisk.o : $(SRC_DIR)removemdisk.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)removemdisk.c

removenic : removenic.o $(ZHCPLIB)
	gcc -o removenic  -L./ -lzhcp $(UTILS_OBJECTS) removenic.o
removenic.o : $(SRC_DIR)removenic.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)removenic.c

removeprocessor : removeprocessor.o $(ZHCPLIB)
	gcc -o removeprocessor  -L./ -lzhcp $(UTILS_OBJECTS) removeprocessor.o
removeprocessor.o : $(SRC_DIR)removeprocessor.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)removeprocessor.c

replacevs : replacevs.o $(ZHCPLIB)
	gcc -o replacevs  -L./ -lzhcp $(UTILS_OBJECTS) replacevs.o
replacevs.o : $(SRC_DIR)replacevs.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)replacevs.c
    
setipl : setipl.o $(ZHCPLIB)
	gcc -o setipl  -L./ -lzhcp $(UTILS_OBJECTS) setipl.o
setipl.o : $(SRC_DIR)setipl.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)setipl.c
    
setpassword : setpassword.o $(ZHCPLIB)
	gcc -o setpassword  -L./ -lzhcp $(UTILS_OBJECTS) setpassword.o
setpassword.o : $(SRC_DIR)setpassword.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)setpassword.c

startvs : startvs.o $(ZHCPLIB)
	gcc -o startvs  -L./ -lzhcp $(UTILS_OBJECTS) startvs.o
startvs.o : $(SRC_DIR)startvs.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)startvs.c

stopvs : stopvs.o $(ZHCPLIB)
	gcc -o stopvs  -L./ -lzhcp $(UTILS_OBJECTS) stopvs.o
stopvs.o : $(SRC_DIR)stopvs.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)stopvs.c
	
syncfileutil : syncfileutil.o
	gcc -o syncfileutil syncfileutil.o
syncfileutil.o : $(SRC_DIR)syncfileutil.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)syncfileutil.c

undedicatedevice : undedicatedevice.o $(ZHCPLIB)
	gcc -o undedicatedevice  -L./ -lzhcp $(UTILS_OBJECTS) undedicatedevice.o
undedicatedevice.o : $(SRC_DIR)undedicatedevice.c
	gcc -c $(DEBUG) -I$(INCLUDE_DIR) $(SRC_DIR)undedicatedevice.c
    
smcli :smcli.o $(SMCLI_OBJECTS) $(UTILS_OBJECTS) $(ZHCPLIB) 
	gcc -o smcli  -L./ -lzhcp  $(UTILS_OBJECTS) $(SMCLI_OBJECTS) 

#-----------------------------------------------------------------------
# Install
#-----------------------------------------------------------------------
install:
	mkdir -p $(LIB_DIR)
	install $(ZHCPLIB) $(LIB_DIR)
	mkdir -p $(BIN_DIR)
	install stopvs $(BIN_DIR)
	install add3390 $(BIN_DIR)
	install add3390active $(BIN_DIR)
	install add9336 $(BIN_DIR)
	install adddisk2pool $(BIN_DIR)
	install addvdisk $(BIN_DIR)
	install addprocessor $(BIN_DIR)
	install addnic $(BIN_DIR)
	install ckddecode $(BIN_DIR)
	install ckdencode $(BIN_DIR)
	install ckdhex $(BIN_DIR)
	install connectnic2guestlan $(BIN_DIR)
	install connectnic2vswitch $(BIN_DIR)
	install createuserprofile $(BIN_DIR)
	install createvs $(BIN_DIR)
	install deleteuserprofile $(BIN_DIR)
	install deletevs $(BIN_DIR)
	install dedicatedevice $(BIN_DIR)
	install deleteipl $(BIN_DIR)
	install disconnectnic $(BIN_DIR)
	install getuserentrynames $(BIN_DIR)
	install getuserentry $(BIN_DIR)
	install getipl $(BIN_DIR)
	install getuserprofile $(BIN_DIR)
	install getdiskpool $(BIN_DIR)
	install getdiskpoolnames $(BIN_DIR)
	install queryvolumes $(BIN_DIR)
	install removediskfrompool $(BIN_DIR)
	install removemdisk $(BIN_DIR)
	install removenic $(BIN_DIR)
	install removeprocessor $(BIN_DIR)
	install replaceuserprofile $(BIN_DIR)
	install replacevs $(BIN_DIR)
	install setipl $(BIN_DIR)
	install setpassword $(BIN_DIR)
	install startvs $(BIN_DIR)
	install stopvs $(BIN_DIR)
	install undedicatedevice $(BIN_DIR)
	install smcli $(BIN_DIR)
	install syncfileutil $(BIN_DIR)
	mkdir -p $(LD_DIR)
	mkdir -p $(BIN_DIR)/IUCV
	install iucvserver  $(BIN_DIR)/IUCV/
	install iucvclient $(BIN_DIR)/IUCV/
	cp $(SRC_DIR)IUCV/iucvserverd $(BIN_DIR)/IUCV/

#-----------------------------------------------------------------------
# Post
#-----------------------------------------------------------------------
post:
	echo "/opt/zhcp/lib" > /etc/ld.so.conf.d/zhcp.conf
	ldconfig

#-----------------------------------------------------------------------
# Clean
#-----------------------------------------------------------------------
clean:
	-rm -f *.o *.so $(ZHCPLIB) 
	-rm add3390
	-rm add3390active
	-rm add9336
	-rm adddisk2pool
	-rm addvdisk
	-rm addprocessor
	-rm addnic
	-rm ckddecode
	-rm ckdencode
	-rm ckdhex
	-rm connectnic2guestlan
	-rm connectnic2vswitch
	-rm createuserprofile
	-rm createvs
	-rm deleteipl
	-rm deleteuserprofile
	-rm deletevs
	-rm dedicatedevice
	-rm disconnectnic
	-rm getuserentrynames
	-rm getuserentry
	-rm getipl
	-rm getuserprofile
	-rm getdiskpool
	-rm getdiskpoolnames
	-rm queryvolumes
	-rm removediskfrompool
	-rm removemdisk
	-rm removenic
	-rm removeprocessor
	-rm replacevs
	-rm replaceuserprofile
	-rm setipl
	-rm setpassword
	-rm startvs
	-rm stopvs
	-rm syncfileutil
	-rm undedicatedevice
	-rm smcli
	-rm iucvserver
	-rm iucvclient
