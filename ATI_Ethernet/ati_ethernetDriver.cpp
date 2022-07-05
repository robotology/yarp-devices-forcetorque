/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Francisco Andrade
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "ati_ethernetDriver.h"

#include <cassert>

#include <yarp/math/Math.h>

#include <string>
#include <sstream>

#include <errno.h>

using namespace yarp::math;

yarp::dev::ati_ethernetDriver::ati_ethernetDriver(): m_sensorReadings(6),
                                                                 m_status(yarp::dev::IAnalogSensor::AS_OK),
                                                                    cMatrix (6,6)
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



    yDebug("Ati_ethernetDriver: opening");
    std::lock_guard<std::mutex> guard(m_mutex);

    // config should be parsed for the options of the device
    std::string sensorname;
    sensorname=config.findGroup("calibrationFile").tail().get(0).toString();
    yInfo( "Ati_ethernetDriver: calibration file name "+ sensorname );


    #ifdef _WIN32
	wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
#endif

	/* Calculate number of samples, command code, and open socket here. */
	socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socketHandle == -1) {
        yError("Ati_ethernetDriver: Socket could not be opened.\n");
		exit(1);
	}
*(uint16_t*)&request[0] = htons(0x1234); /* standard header. */
    *(uint16_t*)&request[2] = htons(COMMAND); /* per table 9.1 in Net F/T user manual. */
    *(uint32_t*)&request[4] = htonl(1); /* see section 9.1 in Net F/T user manual. */
	
    yInfo("Ati_ethernetDriver: Sending the request.");

    he = gethostbyname(config.findGroup("ipAddress").tail().get(0).asString().c_str());// TODO: get ipAddres from config file

    if( he == 0 )
    {
        yError("Ati_ethernetDriver: Error in calling gethostbyname");
        return false;
    }

        yDebug("Ati_ethernetDriver: reading ip address");
    memcpy(&addr.sin_addr,
           he->h_addr_list[0],
           he->h_length);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
     yDebug("Ati_ethernetDriver: Attempting connection");
     err = connect( socketHandle, (struct sockaddr *)&addr, sizeof(addr) );
     if (err == -1) {
         yError("Ati_ethernetDriver: Socket could not conect.\n");
         exit(2);
     }


     TiXmlDocument calibFile( sensorname );
     bool loaded= calibFile.LoadFile();
     if (loaded)
     {
         yDebug("Ati_ethernetDriver: Opened file Correctly");

//         yInfo(  calibFile.FirstChild( "dsNetFTCalibrationFile" )->FirstChild("tblNetFTCalibrationInfo")->FirstChild("MatrixFX")->FirstChild()->Value());
//         const char *AXES[] = { "FX", "Fy", "Fz", "Tx", "Ty", "Tz" };	/* The names of the force and torque axes. */
//         std::vector<std::string> axes(AXES, AXES + 6);
//         for(int i=0;i<6;++i)
//         {
//             std::string temp=calibFile.FirstChild( "dsNetFTCalibrationFile" )->FirstChild("tblNetFTCalibrationInfo")->FirstChild("Matrix"+axes[i])->FirstChild()->Value();
//             yarp::sig::Vector tempVector(6);
//             vector6FromString(temp, tempVector);
//             bool inserted=cMatrix.setRow(i,tempVector);
//             if(inserted){
//                 std::stringstream ss;
//                 ss << "length of row "<< i << " in matrix is "<<cMatrix.getRow(i)[1];
//                 std::string str = ss.str();
//                 yInfo(str);

//                 ss << tempVector[0];
//                 str = ss.str();
//                 yInfo("position 0 in tempVector is "+ str);
//             }
//             else{
//                 yError("Ati_ethernetDriver: Couldnt insert vector to calibration matrix");
//             }

//         }

        // yInfo(  calibFile.FirstChild( "dsNetFTCalibrationFile" )->FirstChild( "tblCalibrationInformation" )->FirstChild("CountsPerForce")->FirstChild()->Value());
         std::string temp=calibFile.FirstChild( "dsNetFTCalibrationFile" )->FirstChild( "tblCalibrationInformation" )->FirstChild("CountsPerForce")->FirstChild()->Value();
         stringToDouble( temp,countsperForce);
         yInfo("CountsPerForce value read: "+ temp);
        // yInfo( calibFile.FirstChild( "dsNetFTCalibrationFile" )->FirstChild( "tblCalibrationInformation" )->FirstChild("CountsPerTorque")->FirstChild()->Value());
         temp=calibFile.FirstChild( "dsNetFTCalibrationFile" )->FirstChild( "tblCalibrationInformation" )->FirstChild("CountsPerTorque")->FirstChild()->Value();
         stringToDouble( temp,countsperTorque);
          yInfo("CountsPerTorque value read: "+ temp);
     }
     else
     {
         yError("Ati_ethernetDriver: Could not load file");
     }



     return true;
}

bool yarp::dev::ati_ethernetDriver::close()
{
    std::lock_guard<std::mutex> guard(m_mutex);
#ifdef _WIN32
    closesocket(socketHandle);
#else
    shutdown(socketHandle, SHUT_RDWR);
    if(::close(socketHandle)<0){
       yError("Ati_ethernetDriver: Could not close properly"); ;
        }
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
    std::lock_guard<std::mutex> guard(m_mutex);
    yDebug("Sending request");
   // send( socketHandle, (const char *)request, 8, 0 );
    int s=0;
    if ((s= send( socketHandle, (const char *)request, 8, 0 )) < 0) {
        int errsv = errno;

                   yError("Ati_ethernetDriver:Read:Failed to send bytes from server");

                       std::stringstream ss;
                          ss <<strerror(errsv);
                        std::string str = ss.str();
                         yError(str);

                   exit(1);
                 }
    std::stringstream ss;
       ss <<"recieved value of send function= "<<s;
     std::string str = ss.str();
      yDebug(str);
    /* Receiving the response. */
        int r=0;
        yDebug("r to 0 next comand recv");
   // recv( socketHandle, (char *)response, 36, 0 );
    if ((r = recv( socketHandle, (char *)response, 36, 0 )) < 1) {
        int errsv = errno;

                   yError("Ati_ethernetDriver:Read:Failed to receive bytes from server");
                   if(r==0){
                       yError("Ati_ethernetDriver:Read: 0 bytes received");

                   }
                   else
                   {
                       std::stringstream ss;
                          ss <<strerror(errsv);
                        std::string str = ss.str();
                         yError(str);
                   }
                   exit(1);
                 }
    else{
        std::stringstream ss;
           ss <<"recieved value of recv function= "<<r;
         std::string str = ss.str();
          yDebug(str);
    resp.rdt_sequence = ntohl(*(uint32_t*)&response[0]);
    resp.ft_sequence = ntohl(*(uint32_t*)&response[4]);
    resp.status = ntohl(*(uint32_t*)&response[8]);
    for( i = 0; i < 6; ++i ) {
        resp.FTData[i] = ntohl(*(int*)&response[12 + i * 4]);
	}
    }
    // Set force and torque measurements on x,y,z axis
    
    for (i =0;i < 6;++i) {
//        double m_temp=0; // aparently raw measurements are already given in counts which dont require the calibration matrix
//        for(int j =0;j < 6;++j){
//            std::stringstream ss;
//            ss <<" m_temp previous summation "<< m_temp;
//            m_temp=m_temp + ((double)resp.FTData[j])*cMatrix(i,j);

//            ss << " raw measurement  "<< resp.FTData[j] << " matrix row "<<i<<" column "<<j <<" value "<<cMatrix(i,j)<<" temp measuremnet "<<m_temp<< "\n";
//            std::string str = ss.str();
//            yInfo(str);
//        }
//        m_sensorReadings[i]=m_temp;
        if(i<3)
        m_sensorReadings[i]=((double)resp.FTData[i])/countsperForce;
        else
        m_sensorReadings[i]=((double)resp.FTData[i])/countsperTorque;
    }

    // When you update the sensor readings, you also need to update the timestamp
    m_timestamp.update();
    out = m_sensorReadings;
    
    return m_status;
}

int yarp::dev::ati_ethernetDriver::getState(int /*ch*/)
{
    std::lock_guard<std::mutex> guard(m_mutex);
        yDebug("checking state");
    return m_status;
}

int yarp::dev::ati_ethernetDriver::getChannels()
{
    return 6;
}

int yarp::dev::ati_ethernetDriver::calibrateSensor()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    //read sensorSheet file? or zeroing?
    return m_status;
}

int yarp::dev::ati_ethernetDriver::calibrateSensor(const yarp::sig::Vector& /*value*/)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    
    return m_status;
}

int yarp::dev::ati_ethernetDriver::calibrateChannel(int /*ch*/)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    
    return m_status;
}

int yarp::dev::ati_ethernetDriver::calibrateChannel(int /*ch*/, double /*v*/)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    
    return m_status;
}

yarp::os::Stamp yarp::dev::ati_ethernetDriver::getLastInputStamp()
{
    return m_timestamp;
}

