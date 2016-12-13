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
     * Checks driver configuration
	 * @return configuration check
	 */
	AMTI_CONFIGURATION_CHECK checkDriverConfiguration();

	/**
	 * Initialize all the parameters needed for the transmission
	 * This function correctly sets the defaults
	 */
	void initDriverTransmissionParameters();

    /** calibrate the platforms
     */
	void calibratePlatforms();

    /** Starts the acquisition for all the connected platforms
     */
	void startAcquisition();

    /** Stops the acquisition for all the connected platforms
     */
	void stopAcquisition();

    /**
     * Temporally apply the configuration changes.
     * @note changes are not permanently applied to the devices.
     * At the next power cycle they get discarded.
     */
    void applyConfigurationChanges();

    /** Configure the driver run mode (i.e. poll or message)
     * @param runMode the runmode to be configured
     * @param threadID identifier of the thread if thread mode is selected
     */
	void configureDriverRunMode(AMTI_RUNMODE runMode, unsigned int threadID = -1);

    /** Configure the genlock mode
     *
     * Available modes are "off", at rising and falling edge
     * @param genlock the genlock mode
     */
	void configureGenlock(AMTI_GENLOCK genlock);

    /** Configure the data format
     *
     * Available formats are 6 (forces and moments) or
     * 8 (counter, 6 data, trigger)
     * @param format data format (6 or 8 channels)
     */
	void configureDriverDataFormat(AMTI_DATAFORMAT format);

    /** Sets the frequency of the acquisition
     *
     * @param samplePerSeconds frequency in Hz
     */
	void setAcquisitionRate(int samplePerSeconds);

    /**
     * Returns the acquisition rate (in Hz) for the specified platform
     * @param platformIndex index of the considered platform
     * @return the acquisition rate in Hz
     */
    int getAcquisitionRate(unsigned platformIndex);

    /**
     * Returns the available rates accepted by the platforms
     * @note It is responsibility of the caller to release the memory pointed by availableRates
     * @param[out] availableRates upon call the value will point to an integer array containing the available rates
     * @param[out] count size of availableRates vector
     */
    void getAvailableAcquisitionRates(int *& availableRates, unsigned& count);

    /** Get the numnber of connected platforms
     * @return the number of connected platforms
     */
	int getPlatformsCount();

    /** Get the platform model number for the specified index
     *
     * Returns the Amplifier model number
     * @param[in] platformIndex index of the platform
     * @param[out] platformModelNumber platform amplifier model number
     */
	void getPlatformModelNumber(unsigned platformIndex, char *platformModelNumber);

   /** Get the platform serial number for the specified index
    *
    * Returns the Amplifier serial number
    * @param[in] platformIndex index of the platform
    * @param[out] platformSerialNumber platform amplifier serial number
    */
	void getPlatformSerialNumber(unsigned platformIndex, char *platformSerialNumber);

   /** Get the platform FW version for the specified index
    *
    * Returns the Amplifier FW version
    * @param[in] platformIndex index of the platform
    * @param[out] platformFWVersion platform FW version
    */
	void getPlatformFirmwareVersion(unsigned platformIndex, char *platformFWVersion);

   /** Get the last calibration date of the amplifier of the specified platform
    *
    * Returns the last calibration date of the specified Amplifier
    * @param[in] platformIndex index of the platform
    * @param[out] platformDate last calibration date
    */
	void getPlatformLastCalibrationDate(unsigned platformIndex, char *platformDate);

    /**
     * Get the actual measurement from all the force plates in the system
     *
     * @note data is packed in blocks of 16 datasets. 
     * This function returns only the last measurement 
     *
     * @param[in] numOfPlatforms number of force plates in the system
     * @param[in] channelSize 6 or 8 depending on the dataFormat chosen
     * @param[out] reading already allocated vector of size channelSize * numOfPlatforms containing the last measurements
     * @return 0 if no measurements are available, > 0 if at least one measure is available.
     */
	int getCurrentData(unsigned numOfPlatforms, unsigned channelSize, double* reading);

    /**
    * Get the actual measurement from all the force plates in the system
    *
    * @note all the 16 measurements are returned
    *
    * @param[in] numOfPlatforms number of force plates in the system
    * @param[in] channelSize 6 or 8 depending on the dataFormat chosen
    * @param[out] reading already allocated vector of size 16 * channelSize * numOfPlatforms containing 16 measurements
    * @return 0 if no measurements are available, > 0 if at least one measure is available.
    */
    int getLastDataPacket(unsigned numOfPlatforms, unsigned channelSize, double* reading);


#ifdef __cplusplus
}
#endif

#endif // AMTILIB_H
