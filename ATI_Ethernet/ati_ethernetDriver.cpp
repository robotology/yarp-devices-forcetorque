/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Francisco Andrade
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "ati_ethernetDriver.h"

#include <cassert>

#include <yarp/os/LockGuard.h>

yarp::dev::ati_ethernetDriver::ati_ethernetDriver(): m_sensorReadings(6),
                                                                 m_status(yarp::dev::IAnalogSensor::AS_OK)
{
    yInfo("Constructor beggining.");
    // We fill the sensor readings only once in the constructor in this example
    // In reality, the buffer will be updated once a new measurement is avaible
   m_sensorReadings[0] = 0;
    m_sensorReadings[1] = 0;
    m_sensorReadings[2] = 0;
    
    // Set torque on x,y,z axis 
    m_sensorReadings[3] = 0;
    m_sensorReadings[4] = 0;
    m_sensorReadings[5] = 0;
    
    
    // When you update the sensor readings, you also need to update the timestamp
    m_timestamp.update();
    yInfo("Constructor end");
}

yarp::dev::ati_ethernetDriver::~ati_ethernetDriver()
{
}

bool yarp::dev::ati_ethernetDriver::open(yarp::os::Searchable &config)
{
    yInfo("opening");
    yarp::os::LockGuard guard(m_mutex);
    #ifdef _WIN32
	wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
#endif

	/* Calculate number of samples, command code, and open socket here. */
	socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socketHandle == -1) {
		yError("Socket could not be opened.\n");
		exit(1);
	}
*(uint16_t*)&request[0] = htons(0x1234); /* standard header. */
    *(uint16_t*)&request[2] = htons(COMMAND); /* per table 9.1 in Net F/T user manual. */
    *(uint32_t*)&request[4] = htonl(1); /* see section 9.1 in Net F/T user manual. */
	
    yInfo("Sending the request.");
    //he = gethostbyname(ipAddres);// TODO: get ipAddres from config file
    he = gethostbyname("10.255.36.44");// TODO: get ipAddres from config file

    if( he == 0 )
    {
        yError("Error in calling gethostbyname");
        return false;
    }

        yInfo("reading ip address");
    memcpy(&addr.sin_addr,
           he->h_addr_list[0],
           he->h_length);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
     yInfo("Attempting connection");
	err = connect( socketHandle, (struct sockaddr *)&addr, sizeof(addr) );
	if (err == -1) {
		yError("Socket could not conect.\n");
		exit(2);
	}


    // config should be parsed for the options of the device 
    return true;
}

bool yarp::dev::ati_ethernetDriver::close()
{
    yarp::os::LockGuard guard(m_mutex);
   #ifdef _WIN32
	closesocket(socketHandle);
#else
   ::close(socketHandle);
#endif
    return true;
}

yarp::dev::ati_ethernetDriver::ati_ethernetDriver(const yarp::dev::ati_ethernetDriver& /*other*/)
{
    // Copy is disabled 
    assert(false);
}

int yarp::dev::ati_ethernetDriver::read(yarp::sig::Vector &out)
{
    yarp::os::LockGuard guard(m_mutex);
send( socketHandle, (const char *)request, 8, 0 );

	/* Receiving the response. */
	recv( socketHandle, (char *)response, 36, 0 );
    resp.rdt_sequence = ntohl(*(uint32_t*)&response[0]);
    resp.ft_sequence = ntohl(*(uint32_t*)&response[4]);
    resp.status = ntohl(*(uint32_t*)&response[8]);
	for( i = 0; i < 6; i++ ) {
        resp.FTData[i] = ntohl(*(int*)&response[12 + i * 4]);
	}
    // Set force and torque measurements on x,y,z axis
    
    for (i =0;i < 6;i++) {
        m_sensorReadings[i]= resp.FTData[i];
	}

    // When you update the sensor readings, you also need to update the timestamp
    m_timestamp.update();
    out = m_sensorReadings;
    
    return m_status;
}

int yarp::dev::ati_ethernetDriver::getState(int /*ch*/)
{
    yarp::os::LockGuard guard(m_mutex);
        
    return m_status;
}

int yarp::dev::ati_ethernetDriver::getChannels()
{
    return 6;
}

int yarp::dev::ati_ethernetDriver::calibrateSensor()
{
    yarp::os::LockGuard guard(m_mutex);
    //read sensorSheet file? or zeroing?
    return m_status;
}

int yarp::dev::ati_ethernetDriver::calibrateSensor(const yarp::sig::Vector& /*value*/)
{
    yarp::os::LockGuard guard(m_mutex);
    
    return m_status;
}

int yarp::dev::ati_ethernetDriver::calibrateChannel(int /*ch*/)
{
    yarp::os::LockGuard guard(m_mutex);
    
    return m_status;
}

int yarp::dev::ati_ethernetDriver::calibrateChannel(int /*ch*/, double /*v*/)
{
    yarp::os::LockGuard guard(m_mutex);
    
    return m_status;
}

yarp::os::Stamp yarp::dev::ati_ethernetDriver::getLastInputStamp()
{
    return m_timestamp;
}

