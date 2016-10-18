/*
* Copyright (C) 2016 iCub Facility
* Authors: Francesco Romano, Ali Akhras
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/

#ifndef AMTILIB_H
#define AMTILIB_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

	typedef enum AMTI_CONFIGURATION_CHECK
	{
		AMTI_CONFIGURATION_CHECK_NODEVICES = 0,
		AMTI_CONFIGURATION_CHECK_SETUP_IS_EQUAL = 1,
		AMTI_CONFIGURATION_CHECK_CONFIGFILE_NOT_FOUND = 211,
		AMTI_CONFIGURATION_CHECK_WRONG_SOFTWARE_VERSION = 213,
		AMTI_CONFIGURATION_CHECK_CHANGED_DIFFERENT_NUM_DEVICES = 214,
		AMTI_CONFIGURATION_CHECK_CHANGED_SN_MISMATCH = 215
	} AMTI_CONFIGURATION_CHECK;

	typedef enum AMTI_RUNMODE {
		AMTI_RUNMODE_POLL,
		AMTI_RUNMODE_CALLBACK
	};

	typedef enum AMTI_DATAFORMAT {
		AMTI_DATAFORMAT_ONLYDATA = 0,
		AMTI_DATAFORMAT_EXTENDED = 1
	};

	typedef enum AMTI_GENLOCK {
		AMTI_GENLOCK_OFF = 0,
		AMTI_GENLOCK_EDGE_RISING = 1,
		AMTI_GENLOCK_EDGE_FALLING = 2
	};

	/**
	 * Initializes the AMTI driver.
	 *
	 * @return 0 if initialization failed. 1 if no device has been found. 2 if everything is correct 
	 */
	int loadDriver();

	/**
	 * Checks the status of the AMTI driver
	 *
	 * @return 0 if the driver is not initialized. 1 if no device has been found. 2 if driver is initialized and platforms found
	*/
	int checkDriverStatus();

	/**
	 * Terminates the driver
	 */
	void releaseDriver();

	/**
	 * @return configuration check
	 */
	AMTI_CONFIGURATION_CHECK checkDriverConfiguration();
	
	/**
	 * Initialize all the parameters needed for the transmission
	 * This function correctly sets the defaults
	 */
	void initDriverTransmissionParameters();
	void calibratePlatforms();
	void startAcquisition();
	void stopAcquisition();

	void configureDriverRunMode(AMTI_RUNMODE runMode, unsigned int threadID = -1);
	void configureGenlock(AMTI_GENLOCK genlock);
	void configureDriverDataFormat(AMTI_DATAFORMAT format);
	void setAcquisitionRate(int samplePerSeconds);

	int getPlatformsCount();
	//TODO: there is an Amplifier and a Platform version for each of these function
	void getPlatformModelNumber(unsigned platformIndex, char *platformModelNumber);
	void getPlatformSerialNumber(unsigned platformIndex, char *platformSerialNumber);
	void getPlatformFirmwareVersion(unsigned platformIndex, char *platformFWVersion);
	void getPlatformLastCalibrationDate(unsigned platformIndex, char *platformDate);

	int getCurrentData(unsigned numOfPlatforms, unsigned channelSize, double* reading);


#ifdef __cplusplus
}
#endif

#endif // AMTILIB_H
