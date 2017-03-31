/**
 * @file
 * @author  Giuseppe Iellamo <giuseppe.iellamo@iit.it>
 * @version 1.0
 *
 * @copyright (c) IIT Fondazione Istituto Italiano
 *            di Tecnologia. All rights reserved
 *
 * @brief Tiny Tcp/IP Wrapper
 */

#include "ethService.h"
#include "osutil.h"

#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>


namespace Ethservice {

using namespace rehab;

/*****************************************************************
 *                       TCP  SECTION                            *
*****************************************************************/

///
const static string udpBroadcastAddress = "255.255.255.255";

/**
   * @brief The destructor is not virtual because the class is Final
   *        and is meant not to be overriden
   */
TCPConnector::~TCPConnector()
{
  if (this->sockId != 0)
  {
    close(this->sockId);
    this->sockId = INVALID_SOCKET;
  }
}


/*!
 * \brief This function send message over TCP protocol
 * \param packetToBeSent Buffer which contains the message to be sent
 * \return Number of byte sent otherwise -1, this means error on trasmission data
 */
int TCPConnector::send(const CharBuff *packetToBeSent) const
{
  int BytesSent;

  BytesSent = ::send(this->sockId, (char*) packetToBeSent->content,
                     packetToBeSent->size, 0);
  if (BytesSent == SOCKET_ERROR)
  {
    std::cerr << "send() failed! Error code:  "<< errno <<  std::endl;
    return -1;
  }
  else
  {
    return BytesSent;
  }
}

/**
 * @brief This function receive message over TCP protocol
 * @param packetToReceive Buffer which contains the received message
 * @return  Number of byte received
 */
int TCPConnector::recv(CharBuff *packetToReceive) const
{
  int BytesReceived = ::recv(this->sockId, (char*) packetToReceive->content,
                             sizeof(packetToReceive->content), 0);
  packetToReceive->size = BytesReceived;
  //RH_TRACE("Received: ", BytesReceived, " Bytes"<<std::endl;
  return BytesReceived;
}


/*****************************************************************
 *                       UDP  SECTION                            *
*****************************************************************/
/**
   * @brief The destructor is not virtual because the class is Final
   *        and is meant not to be overriden
   */
UDPConnector::~UDPConnector()
{
}

/*!
 * \brief Transmission Control Protocol (TCP) connection
 * \return 0 if no errors otherwise UNKNOWN_ERR
 */
RC TCPConnector::connect()
{
  this->sockId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (this->sockId == INVALID_SOCKET)
  {
    std::cerr << "Client: socket() failed! Error code:  "<< errno <<  std::endl;

    return RC::UNKNOWN_ERR;
  }

  struct sockaddr_in ServerAddr;
  ServerAddr.sin_family = AF_INET;
  ServerAddr.sin_port = htons(this->port);
  ServerAddr.sin_addr.s_addr = inet_addr(this->address.c_str());

  int RetCode = ::connect(this->sockId, (struct sockaddr *) &ServerAddr,
                          sizeof(ServerAddr));

  if (RetCode != 0)
  {
    std::cerr << " connect() failed! Error code:  "<< errno <<  std::endl;
    close(this->sockId);
    this->sockId = INVALID_SOCKET;

    return RC::UNKNOWN_ERR;
  }

  this->connected = true;
  return RC::OK;
}


/*!
 * \brief Initialize of all data of User Datagram Protocol (UDP)
 * \return 0 (OK) means no errors.
 */
RC UDPConnector::initData(int sockId)
{

  this->ReceiverAddr.sin_family = AF_INET;
  this->ReceiverAddr.sin_port = htons(this->port);
  this->ReceiverAddr.sin_addr.s_addr = inet_addr(this->address.c_str());
  this->sockId = sockId;

  return RC::OK;
}

/*!
 * \brief This function send message over UDP protocol
 * \param packetToBeSent Buffer which contains the message to be sent
 * \return Number of byte sent otherwise -1, this means error on trasmission data
 */
int UDPConnector::send(const CharBuff *packetToBeSent) const
{
  int BytesSent = sendto(this->sockId, (char*) packetToBeSent->content,
                         packetToBeSent->size, 0, (struct sockaddr *) &ReceiverAddr,
                         sizeof(ReceiverAddr));
  return BytesSent;
}


/*****************************************************************
 *                       BOARD  SECTION                          *
*****************************************************************/
/**
 * @brief User Datagram Protocol (UDP) connection
 * @param udpSockId Intege which represent Socket identtifier
 * @return If no errors 0 (OK) otherwise errors
 */
rehab::RC Board::connect(int udpSockId)
{
  auto rc = RC::OK;

  std::cout << " connecting addr:   " <<address<<std::endl;

  if (this->hasTcpService())
  {
    rc = this->tcpConn.connect();
  }

  if (rc == RC::OK)
  {
    if (this->hasUdpService())
    {
      rc = this->udpConn.initData(udpSockId);
    }
  }

  return rc;
}

/*!
 * \brief Handler of UDP message, add message on back
 *        to udpMessageHandler_t function
 * \param msgHandle Pointer to function that takes care of the message
 */
void Board::installUdpDatagramHandler(udpMessageHandler_t&& msgHandle)
{
  this->msgh.emplace_back(msgHandle);
}

/*!
 * \brief Gives the datagram to each of the Datagram Handler
 * \param datagram Buffer which contains the received message
 */
void Board::onUdpDatagram(const CharBuff* datagram)
{
  // Gives the datagram to each of the Datagram Handler
  for (auto&& udpHandlers : msgh)
  {
    udpHandlers(datagram);
  }
}

/*****************************************************************
 *             EthInterfaceManager  SECTION                      *
*****************************************************************/

/**
 * @brief Constructor of EthInterfaceManager that initialize the class with
 *        address and port of specific board
 * @param localAddress Address of board
 * @param port Connection port
 */
EthInterfaceManager::EthInterfaceManager(const std::string& localAddress, int port, std::chrono::milliseconds timeout_) :
  localAddress { localAddress }, numConfiguredBoards(0), sockId(0), port(port),
  workerThread{}, workerStarted(false),continuing{true},timeoutHandler{}, timeout{timeout_}
{
}

/**
   * @brief The destructor is not virtual because the class is Final
   *        and is meant not to be overriden
   */
EthInterfaceManager::~EthInterfaceManager()
{
  this->stopRecvThread();

  // close local "broadcast socket"
  close(this->sockId);
}

/**
 * @brief Starts the thread that monitors udp connections
 */
void EthInterfaceManager::startRecvThread()
{
  std::cerr << "Start Recv Thread"<<std::endl;
  this->workerThread = std::thread {&EthInterfaceManager::worker, this};
  this->workerStarted = true;
}

/**
 * @brief Stops the thread that monitors udp connections
 */
void EthInterfaceManager::stopRecvThread()
{
  if (workerStarted)
  {
    this->continuing = false;
  }
  if (this->workerThread.joinable())
    this->workerThread.join();
}

/**
  * \brief This function add one board to array of Board class.
  *  Only if the number of boards are not over 4 boards
  * @param address_ Ip address of board
  * @param tcpService_ Boolean which contains status of TCP connection(true connected)
  * @param tcpPort_ TCP port connection
  * @param udpService_ Boolean which contains status of UDP connection(true connected)
  * @param udpPort_ UDP port connection
  * \return number of boardsAdded or -1 if full, it can be used as an ID
 */
board_id_t EthInterfaceManager::addBoard(string address_, bool tcpService_, int tcpPort_,
                           bool udpService_, int udpPort_)
{
  if (this->numConfiguredBoards >= MAX_NUM_BOARDS)
  {
    return -1;
  }

  this->boards[this->numConfiguredBoards] = std::unique_ptr<Board> { new Board(
        address_, tcpService_, tcpPort_, udpService_, udpPort_) };

  this->boardNumIp[this->numConfiguredBoards] = inet_addr(address_.c_str());

  return (this->numConfiguredBoards++);

}

/*!
 * \brief This function try to bind specific board socket for receiving
 *        broadcast messages.
 * \return 0 means no errors, otherwise errors (UNKNOWN_ERR)
 */
RC EthInterfaceManager::bind()
{
  this->sockId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (this->sockId == INVALID_SOCKET)
  {
    std::cerr << "socket() failed! Error code: "<< errno <<  std::endl;
    ; // Do the clean up
    return RC::UNKNOWN_ERR;
  }

  struct sockaddr_in localAddress;
  localAddress.sin_family = AF_INET;
  localAddress.sin_port = htons(port);
  localAddress.sin_addr.s_addr = inet_addr(this->localAddress.c_str());

  // Allow broadcast!
  auto iOptVal = 1;
  auto iOptLen = sizeof(iOptVal);
  if (setsockopt(this->sockId, SOL_SOCKET, SO_BROADCAST, (char*) &iOptVal,
                 iOptLen) == SOCKET_ERROR)
  {
    std::cerr << "setsockopt() failed! Error code:   "<< errno <<  std::endl;
    return RC::UNKNOWN_ERR;
  }

  if (::bind(this->sockId, (struct sockaddr *) &(localAddress),
             sizeof(localAddress)) == SOCKET_ERROR) //Error at bind()
  {
    std::cerr << "bind() failed! Error code:"<< errno <<  std::endl;
    close(this->sockId);
    this->sockId = INVALID_SOCKET;
    return RC::UNKNOWN_ERR;
  }

  std::cerr << "Binded socket for receiving BroadCasts sckId:  " << this->sockId << std:: endl;

  return RC::OK;
}

/*!
 * \brief Connect to all the configured boards
 */
RC EthInterfaceManager::connectAll()
{
  rehab::RC rc = RC::OK;

  if (this->bind() != RC::OK)
  {
    return RC::UNKNOWN_ERR;
  }

  for (int i = 0; i < this->numConfiguredBoards; i++)
  {
    rc = this->boards[i]->connect(this->sockId);
    if (rc != RC::OK)
    {
      std::cerr << "connect() failed! Error code: "<< errno <<  std::endl;
      return rc;
    }
  }

  return rc;
}

/*!
 * \brief Install an handler for udp messages going to boardId board
 * \param boardId       Code of the board interested by the handler
 * \param msgHandle     Pointer to function that takes care of the message
 */
void EthInterfaceManager::installUdpDatagramHandler(const uint16_t boardId,
                                                    udpMessageHandler_t &&msgHandle)
{
  if (boardId < this->numConfiguredBoards)
  {
    this->boards[boardId]->installUdpDatagramHandler(std::move(msgHandle));
  }
}

void EthInterfaceManager::installTimeoutHandler(timeoutHandler_t &&msgHandle)
{
  auto now = std::chrono::high_resolution_clock::now();
  for ( auto& last: this->lastTimePoint)
    last = now;
  this->timeoutHandler = std::move(msgHandle);
}

/*!
 * \brief This function receive message over UDP protocol
 * \param packetToReceive  Buffer which contains the received message
 * \param timeout Timeout for receiving message
 * \return Number of Bytes received; -1 on timeout
 */
int EthInterfaceManager::recvUdp(CharBuff* packetToReceive)
{
  struct sockaddr_in SenderAddr;
  socklen_t SenderAddrSize = sizeof(SenderAddr);

  fd_set readset;

  // Initialize the set
  FD_ZERO(&readset);
  FD_SET(this->sockId, &readset);


  struct timeval tv;
  // Initialize time out struct
  tv.tv_sec = 0;
  tv.tv_usec = timeout.count() * 1000;
  // select()
  auto result = select(this->sockId + 1, &readset, NULL, NULL, &tv);
  if (result <= 0)
  {
    if (timeoutHandler)
      timeoutHandler(99);
    return -1;
  }


  // TODO Better way to handle errors?
  int BytesReceived = recvfrom(this->sockId, (char*) packetToReceive->content,
                               sizeof(packetToReceive->content), 0, (struct sockaddr *) &SenderAddr,
                               &SenderAddrSize);
  packetToReceive->size = BytesReceived;

  for (int i = 0; i < this->numConfiguredBoards; i++)
  {
    if (SenderAddr.sin_addr.s_addr == boardNumIp[i])
    {
      this->lastTimePoint[i] =  std::chrono::high_resolution_clock::now();

      this->boards[i]->onUdpDatagram(packetToReceive);
    }
  }

  auto now = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < this->numConfiguredBoards; i++)
  {
    auto duration = std::chrono::duration_cast< std::chrono::milliseconds >(now-this->lastTimePoint[i]);
    if (duration > timeout  )
    {
      {
        if (timeoutHandler)
          timeoutHandler(i);
      }

    }
  }

  return BytesReceived;
}

/**
 * @brief Receiver Thread Management
 */
void EthInterfaceManager::worker()
{

  while (continuing)
  {
    auto rc = this->recvUdp(&cbUdpRec);
    if (rc == -1) {
      // TIMEOUT ERROR
    }

  }
}

/**
 * @brief getIpAddress
 * @return List of pc address
 */
vector<string> getIpAddress()
{
  struct ifaddrs * ifAddrStruct=NULL;
  struct ifaddrs * ifa=NULL;
  void * tmpAddrPtr=NULL;
  char addressBuffer[INET_ADDRSTRLEN];
  vector<string> list;
  list.clear();
  getifaddrs(&ifAddrStruct);
  for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
  {
    if (ifa ->ifa_addr->sa_family==AF_INET)
    {
      tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
      inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
      list.push_back(addressBuffer);
    }
  }
  freeifaddrs(ifAddrStruct);
  return list;
}



/**
 * @brief ipAddressMatching
 * @warning it works only for class C address
 * @return true if two string matching otherwise false
 */
bool ipAddressMatching(const std::string & sensorAddress,const std::string &  ipAddress)
{
  int occurrence  = 0;
  int index = 0;
  bool findAddress = false;
  for(auto count=0u;count< ipAddress.length();count++)
  {
    if(ipAddress[count]=='.')
    {
      // TODO testing only last octet of ip Adrress
      if (++occurrence == 3)
      {
        index = count+1;
        break;
      }
    }
  }
  if (ipAddress.find(sensorAddress.c_str()  ,0,index) == 0)
  {
    findAddress = true;
  }
  return findAddress;
}
/**
 * @brief searchAddressOnList
 * @return Correct IP Address otherwise string Empty
 */
std::string searchAddressOnList(const std::string & sensorAddress,const vector<string> &listOfIpAddress)
{
  for (decltype(listOfIpAddress.size()) i = 0; i < listOfIpAddress.size(); ++i)
  {
    if(ipAddressMatching(sensorAddress, listOfIpAddress[i]))
    {
      return listOfIpAddress[i];
    }
  }
  return "";
}

bool checkMessageConsistency(uint32_t actualId, uint32_t expectedId )
{
  //TODO ERROR
  if (actualId != expectedId)
  {
    std::cerr << " ERROR RECEIVING MESSAGE ActualId  ExpectedId   " << actualId << " "<< expectedId << std::endl;
    return false;
    exit(-1);
  }
  return true;

}

}
