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
#include <algorithm>

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

int getAcquisitionRate(unsigned platformIndex)
{
    fmDLLSelectDeviceIndex(platformIndex);
    return fmGetAcquisitionRate();
}

void getAvailableAcquisitionRates(int *& availableRates, unsigned& count)
{
    count = 34;
    availableRates = new int[count];
    availableRates[0] = 10; availableRates[1] = 15; availableRates[2] = 20; availableRates[3] = 25;
    availableRates[4] = 30; availableRates[5] = 40; availableRates[6] = 45; availableRates[7] = 50;
    availableRates[8] = 60; availableRates[9] = 75; availableRates[10] = 80; availableRates[11] = 90;
    availableRates[12] = 100; availableRates[13] = 120; availableRates[14] = 125; availableRates[15] = 150;
    availableRates[16] = 180; availableRates[17] = 200; availableRates[18] = 225; availableRates[19] = 240;
    availableRates[20] = 250; availableRates[21] = 300; availableRates[22] = 360; availableRates[23] = 400;
    availableRates[24] = 450; availableRates[25] = 500; availableRates[26] = 600; availableRates[27] = 800;
    availableRates[28] = 900; availableRates[29] = 1000; availableRates[30] = 1200; availableRates[31] = 1500;
    availableRates[32] = 1800; availableRates[33] = 2000;
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

void applyConfigurationChanges()
{
    fmBroadcastResetSoftware();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
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
    // more than one dataset. Return only the 16th dataset
    float *lastDataset = buffer + 15 * (channelSize * numOfPlatforms);

    std::copy(lastDataset, lastDataset + (channelSize * numOfPlatforms) - 1, reading);

    return numberOfDataSets;
}

int getLastDataPacket(unsigned numOfPlatforms, unsigned channelSize, double* reading)
{
    assert(numOfPlatforms == getPlatformsCount());
    float *buffer = 0;
    int numberOfDataSets = fmDLLTransferFloatData(buffer);
    if (numberOfDataSets == 0) return 0;
    // more than one dataset. Check the buffer pointer

    std::copy(buffer, buffer +  16 * (channelSize * numOfPlatforms) - 1, reading);

    return numberOfDataSets;
}
