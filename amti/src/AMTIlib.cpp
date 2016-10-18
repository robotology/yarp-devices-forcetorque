/*
* Copyright (C) 2016 iCub Facility
* Authors: Francesco Romano, Ali Akhras
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/

#include "AMTIlib.h"

#include <Windows.h> //This is needed by AMTIUSBDeviceDefinitions.h
#include "AMTIUSBDeviceDefinitions.h"

#include <chrono>
#include <thread>
#include <cassert>

#define AMTI_MAXINIT_WAIT 20

//Note: some functions are asynchronous, but the hardware requires time to satisfy the requests
//Sleeps are then inserted in the code when necessary.

int loadDriver()
{
	//Init the DLL
	fmDLLInit();
	//wait at least 250ms for device initialization
	int counter = 0;
	int deviceStatus = 0;
	do {
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		deviceStatus = fmDLLIsDeviceInitComplete();
		counter++;
	} while (!deviceStatus && counter <= AMTI_MAXINIT_WAIT);

	return deviceStatus;

}

int checkDriverStatus()
{
	return fmDLLIsDeviceInitComplete();
}

void releaseDriver()
{
	fmDLLShutDown();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

AMTI_CONFIGURATION_CHECK checkDriverConfiguration()
{
	return static_cast<AMTI_CONFIGURATION_CHECK>(fmDLLSetupCheck());
}

void initDriverTransmissionParameters()
{
	//Set packet size to 512
	fmDLLSetUSBPacketSize(512);
	//Use only Metric Units
	fmBroadcastRunMode(0); //NOTE: I am not considering the ANALOG output. Change to 1 if needed
}

void configureDriverRunMode(AMTI_RUNMODE runMode, unsigned int threadID)
{
	if (runMode == AMTI_RUNMODE_POLL) {
		fmDLLPostDataReadyMessages(0);
	}
	else {
		fmDLLPostDataReadyMessages(1);
		fmDLLPostUserThreadMessages(threadID);
	}
}
void configureGenlock(AMTI_GENLOCK genlock)
{
	fmBroadcastGenlock(static_cast<int>(genlock));
}

void configureDriverDataFormat(AMTI_DATAFORMAT format)
{
	fmDLLSetDataFormat(static_cast<int>(format));
}

void setAcquisitionRate(int samplePerSeconds)
{
	fmBroadcastAcquisitionRate(samplePerSeconds);
}

void calibratePlatforms()
{
	fmBroadcastZero();
}

void startAcquisition()
{
	fmBroadcastStart();
}
void stopAcquisition()
{
	fmBroadcastStop();
}

int getPlatformsCount()
{
	return fmDLLGetDeviceCount();
}

void getPlatformModelNumber(unsigned platformIndex, char *platformID)
{
	fmDLLSelectDeviceIndex(platformIndex);
	fmGetAmplifierModelNumber(platformID);
}

void getPlatformSerialNumber(unsigned platformIndex, char *platformID)
{
	fmDLLSelectDeviceIndex(platformIndex);
	fmGetAmplifierSerialNumber(platformID);
}


void getPlatformFirmwareVersion(unsigned platformIndex, char *platformFWVersion)
{
	fmDLLSelectDeviceIndex(platformIndex);
	fmGetAmplifierFirmwareVersion(platformFWVersion);
}

void getPlatformLastCalibrationDate(unsigned platformIndex, char *platformDate)
{
	fmDLLSelectDeviceIndex(platformIndex);
	fmGetAmplifierDate(platformDate);
}

int getCurrentData(unsigned numOfPlatforms, unsigned channelSize, double* reading)
{
    assert(numOfPlatforms == getPlatformsCount());
	float *buffer = 0;
	int numberOfDataSets = fmDLLTransferFloatData(buffer);
	if (numberOfDataSets == 0) return 0;
	// more than one dataset. Check the buffer pointer

	for (unsigned i = 0; i < numOfPlatforms; ++i) {
		for (unsigned ch = 0; ch < channelSize; ++ch) {
			reading[(i * channelSize) + ch] = buffer[(i * channelSize) + ch];
		}
	}

	return 1;

}
