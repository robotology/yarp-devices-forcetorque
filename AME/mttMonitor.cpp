/**
 * @file
 * @author  Giuseppe Iellamo <giuseppe.iellamo@iit.it>
 * @author  Igor Ingegnosi <igor.ingegnosi@iit.it>
 * @version 1.0
 *
 * @copyright (c) IIT Fondazione Istituto Italiano
 *            di Tecnologia. All rights reserved
 *
 */
#include <iostream>
#include <cassert>
#include <getopt.h>
#include <string.h>
#include "rehab.h"
#include "osutil.h"
#include "ethService.h"
#include "ftSensorNew.h"
#include <ncurses.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <ifaddrs.h>

using namespace std;
using namespace Ethservice;
using namespace Multitorque;

/**
 * @brief Print of all actiona can be selected via line command
 */
void printUsage()
{

  std::cout <<std::endl;
  std::cout << "Help";
  std::cout << "\n\t--addr\t   [-a]  Connection address"<<std::endl;
}

static const struct option longOpts[] = {
{ "addr", required_argument, NULL, 'a' },
{ "help", no_argument, NULL, 'h' },
{ NULL, no_argument, NULL, 0 }
};

struct OptionArgument
{
  //addr
  bool addrOption = false;
  std::string ipAddress = "192.168.111.254";
  std::string deviceAddress = "192.168.111.1";

  //help
  bool helpOption = false;

} optionArgument;

/**
 * @brief This function checks the arguments of selected choice on menu
 * @param argc Non-negative value representing the number of arguments passed
 *        to the program from the environment in which the program is run.
 * @param argv Pointer to the first element of an array of pointers to null-terminated multibyte
 *        strings that represent the arguments passed to the program from the execution environment
 *        representing the arguments to the program.
 * @return 0 if no errors, otherwise -1 in case of errors.
 */
int checkArguments (int argc, char *argv[])
{
  int longIndex;
  if ( argc <= 1 )
  {
    printUsage();
    return -1;
  }
  while (1)
  {
    //Check all input parameter from command line
    auto opt = getopt_long( argc, argv, "a:h", longOpts, &longIndex );
    if (opt == -1)
    {
      break;
    }
    switch( opt ) {
    case 'a':
    {
      vector<string> list;
      list = Ethservice::getIpAddress();
      optionArgument.ipAddress = Ethservice::searchAddressOnList(optarg,list);
      if (optionArgument.ipAddress== "")
      {
        std::cerr << "ipAddress ["  << optarg << "]" << " no matching" << std::endl;
        return -1;
      }
      optionArgument.deviceAddress=optarg;
      optionArgument.addrOption = true;
    }
      break;

    case 'h':   /* fall-through is intentional */
    case '?':
      optionArgument.helpOption = true;
      break;

    default:
      /* You won't actually get here. */
      break;
    }
  }
  return 0;
}

/**
 * @brief This function allow the connction from pc to board
 * @return shared pointer of FtSensorNew class
 */
std::shared_ptr<rehab::FtSensorNew> connect()
{

  //Device Connection
  //TODO FIX IT this static is shit
  static auto bdm2= std::make_shared<Ethservice::EthInterfaceManager>(optionArgument.ipAddress,64321);
  auto ftBoard = bdm2->addBoard(optionArgument.deviceAddress, true, 64321, true, 64321);
  auto rc = bdm2->connectAll();
  if ( !rehab::isRCOk(rc) )
  {
    exit(-1);
  }
  auto ftSensor =  std::make_shared<rehab::FtSensorNew>(bdm2.get(), ftBoard);
  bdm2->startRecvThread();
  return ftSensor;
}

/**
 * @brief This function is the utility to allow the control
 * of all functionality of Board
 * @param ftSensor Pointer of object of FtSensorNew class
 */
int readSensorUtility(rehab::FtSensorNew* ftSensor)
{
  //Device Connection
  ftSensor->setSampleStreamPol(Multitorque::SampleStreamPolicy_NORMAL, 10);
  initscr();
  noecho();
  cbreak();
  timeout(1);
  rehab::FtSensorNew::FTSensorData ftc2;
  rehab::FtSensorNew::FTSensorData ftcRaw ;
  int32_t calibResult=0;
  while (1)
  {
    mvprintw(0, 10, " MULTITORQUE MONITOR");
    mvprintw(1, 10, " IP Sensor %s",optionArgument.deviceAddress.c_str());
    mvprintw(2, 10, " IP Address %s",optionArgument.ipAddress.c_str());
    ftSensor->getFTData(ftc2);
    ftSensor->getRawFTData(ftcRaw);
    mvprintw(4,2,"fx    % 04.2f\t  tx   % 04.2f               ",  ftc2[0], ftc2[3] );
    mvprintw(5,2,"fy    % 04.2f\t  ty   % 04.2f               ",  ftc2[1], ftc2[4] );
    mvprintw(6,2,"fz    % 04.2f\t  tz   % 04.2f               ",  ftc2[2], ftc2[5] );
    mvprintw(8,2,"raw % 06.0f % 06.0f % 06.0f % 06.0f % 06.0f % 06.0f                                           ",
             ftcRaw[0], ftcRaw[1],ftcRaw[2], ftcRaw[3],ftcRaw[4],ftcRaw[5]);
    mvprintw(10,2,"Calibration Result : %d ", calibResult);
    mvprintw(12, 2,  " a - READ MtxRow");
    mvprintw(12, 20, " g - READ CalibOffset");
    mvprintw(13, 2,  " c - Unbias Offset");
    
    refresh();
    auto ch = getch();
    if (ch == 'q')
    {
      break;
    }
    if (ch == 'c')
    {
      clear();
      calibResult = ftSensor->calibrateOffsets();
    }
    if (ch == 'g')
    {
      clear();
      rehab::FtSensorNew::FTCalibrationOffsets calOff;
      ftSensor->getCalibrationOffsets(calOff);
      mvprintw(15,2, " %d %d %d %d %d %d",calOff[0],calOff[1], calOff[2],
          calOff[3], calOff[4], calOff[5] );
    }
    if (ch == 'a')
    {
      clear();
      for (auto i : {0,1,2,3,4,5})
      {
        rehab::FtSensorNew::FTSensorData row;
        ftSensor->getMtxRow(i, row);
        mvprintw(16+i, 4, " %f %f %f %f %f %f", row[0], row[1],row[2],
            row[3],row[4],row[5]);
      }
    }
  }
  MILLISLEEP(500);
  ftSensor->setSampleStreamPol(Multitorque::SampleStreamPolicy_OFF, 0);
  refresh();
  endwin();

  return 0;
}

/**
 * @brief The main function is called at program startup after initialization of the non-local objects with static storage duration.
 *        It is the designated entry point to a program that is executed in hosted environment (that is, with an operating system).
 * @param argc Non-negative value representing the number of arguments passed
 *        to the program from the environment in which the program is run.
 * @param argv Pointer to the first element of an array of pointers to null-terminated multibyte
 *        strings that represent the arguments passed to the program from the execution environment
 *        representing the arguments to the program.
 * @return 0 if no errors otherwise -1
 */
int main(int argc, char *argv[])
{
  if ( argc < 2 )
  {
    printUsage();
    return -1;
  }

  //Check Arguments of Command Line
  auto res =  checkArguments (argc, argv);
  if (res != 0)
  {
    std::cerr << "Error on checking command line parameters" << std::endl;
    return -1;
  }

  //Check connection
  if(!optionArgument.helpOption)
  {
    auto ftSensorObj = connect();
    res = readSensorUtility(ftSensorObj.get());
    if (res != 0)
    {
      std::cerr << "Error on execution command " << std::endl;
      return -1;
    }
  }
  else
  {
    printUsage();
  }

  return 0;
}


