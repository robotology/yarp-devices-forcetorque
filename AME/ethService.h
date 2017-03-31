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



#ifndef ETHSERVICE_H
#define ETHSERVICE_H

#include <string>
#include <vector>
#include <array>
#include <atomic>
#include <thread>
#include <functional>
#include <memory>
#include <cstdint>
#include <type_traits>

#include <unistd.h>
#include <netinet/in.h>

#include "rehab.h"

using board_id_t = int32_t;
using namespace std;

/** \brief Namespace which contains all functions of
 *         ethernet trasmission (e.g. connection, board parameters,
 *         send and receive messages etc)
 */
namespace Ethservice {

// Max Size of ethernet Packet
constexpr size_t PACKET_MAX_SIZE{256};
constexpr board_id_t MAX_NUM_BOARDS {4};
///Error for invalid socket selection
constexpr int INVALID_SOCKET {-1};
///Generic socket error
constexpr int SOCKET_ERROR{-1};

using std::string;
using CharBuff=rehab::CharBuff_t<PACKET_MAX_SIZE>;
using udpMessageHandler_t = std::function<void(const CharBuff*)>;
using timeoutHandler_t = std::function<void(board_id_t)>;

static constexpr const size_t MAC_ADDRESS_SIZE   = 8;
using MacAddress = std::array< uint8_t, MAC_ADDRESS_SIZE > ;

vector<string> getIpAddress();
bool ipAddressMatching(const std::string & ,const std::string & );
std::string searchAddressOnList(const std::string & ,const vector<string> & );

bool checkMessageConsistency(uint32_t actualId, uint32_t expectedId );

/**
* @brief Wraps the TCP api to communicate with board
*/
class TCPConnector
{
public:

  /**
   * @brief Constructor that initialize all data fot TCP connection
   *        (port and address like input parameters and put
   *         the boolean which represent the status of connection to false
   *         and the id of socket to 0)
   * @param address Address fo connection
   * @param port Port of connection
   */
  TCPConnector(string address, uint16_t port) :
      address(address), port(port), connected(false), sockId(INVALID_SOCKET)
  {  }

  /**
     * @brief The destructor is not virtual because the class is Final
     *        and is meant not to be overriden
     */
  ~TCPConnector();

  rehab::RC connect();
  /**
   * @brief Function that returns status of connection
   * @return The status of connection true if connected ,
   *         otherwise false.
   */
  bool isConnected() const  {    return this->connected;  }

  int send(const CharBuff* packetToBeSent) const;
  int recv(CharBuff* packetToReceive) const;

  /**
   * @brief getSockId
   * @return
   */
  int getSockId() const  {    return sockId;  }

private:
  const string address;
  const uint16_t port;
  bool connected;
  int sockId;
};


/**
* @brief Wraps the UDP int api to communicate with board
*/
class UDPConnector
{
public:


  /**
   * @brief Constructor that initialize all data fot UDP connection
   *        (port and address like input parameters and put
   *         the boolean which represent the status of connection to false
   *         and the id of socket to 0)
   * @param address_ Address fo connection
   * @param port_ Port of connection
   */
  UDPConnector(string address_, uint16_t port_) :
      address(address_), port(port_), sockId(INVALID_SOCKET)
  {
  }

  /**
     * @brief The destructor is not virtual because the class is Final
     *        and is meant not to be overriden
     */
  ~UDPConnector();

  rehab::RC initData(int sockId);
  int send(const CharBuff* packetToBeSent) const;

  /**
   * @brief Function getters for socket identifier parameter
   * @return Integere which represent socket identifier
   */
  int getSockId() const  {    return sockId;  }

private:
  const string address;
  const uint16_t port;
  int sockId;
  struct sockaddr_in ReceiverAddr;
};

/*!
 * \brief The Board class contains all information and action that
 *        can be made by a specific board
 */
class Board
{
public:

  /**
   * @brief Initialization of all parameters which represent the board cclas
   * @param address_ Ip address of board
   * @param tcpService_ Boolean which contains status of TCP connection(true connected)
   * @param tcpPort_ TCP port connection
   * @param udpService_ Boolean which contains status of UDP connection(true connected)
   * @param udpPort_ UDP port connection
   */
  Board(string address_, bool tcpService_, int tcpPort_, bool udpService_,
      int udpPort_) :
      address(address_), tcpService(tcpService_), tcpPort(tcpPort_), udpService(
          udpService_), udpPort(udpPort_), tcpConn(address_, tcpPort_), udpConn(
          address_, udpPort_)
  {
  }

  rehab::RC connect(int udpSockId);

  /**
 * \brief This function send message over TCP protocol of specific board
 *        only if TCP service is active
 * \param packetToBeSent Buffer which contains the message to be sent
 * \return Number of byte sent otherwise -1, this means error on trasmission data
   */
  int sendTcp(const CharBuff* packetToBeSent) const
  {
    return ((this->tcpService) ? tcpConn.send(packetToBeSent) : -1);
  }

  /**
   * @brief This function receive message over TCP protocol of specific board
   *        only if TCP service is active
   * @param packetToBeReceived Buffer which contains the received message
   * @return  Number of byte received
   */
  int recvTcp(CharBuff* packetToBeReceived) const
  {
    return ((this->tcpService) ? tcpConn.recv(packetToBeReceived) : -1);
  }

  /** \brief This function send message over UDP protocol of specific board
   *          only if UDP service is active
   *  \param packetToBeSent Buffer which contains the message to be sent
   *  \return Number of byte sent otherwise -1, this means error on trasmission data
   */
  int sendUdp(const CharBuff* packetToBeSent) const
  {
    return ((this->udpService) ? udpConn.send(packetToBeSent) : -1);
  }

  /**
   * @brief Check if TCP service is active and connect for specific board
   * @return Status of TCP connection (true if connected
   *         otherwise false)
   */
  bool hasTcpService() const  {    return this->tcpService;  }

  /**
   * @brief Check if UDP service is active and connect for specific board
   * @return Status of UDP connection (true if connected
   *         otherwise false)
   */
  bool hasUdpService() const  {    return this->udpService;  }

  /**
   * @brief Function getters for TCP socket identifier parameter
   * @return Integere which represent socket identifier
   */
  int getTcpSockId() const  {    return tcpConn.getSockId();  }

  void installUdpDatagramHandler(udpMessageHandler_t&& msgHandle);

  void onUdpDatagram(const CharBuff* datagram);

private:
  const string address;
  const bool tcpService;
  const int tcpPort;
  const bool udpService;
  const int udpPort;
  TCPConnector tcpConn;
  UDPConnector udpConn;
  std::vector<udpMessageHandler_t> msgh;
};


constexpr static std::chrono::milliseconds aHundredMillis{100};
constexpr static std::chrono::milliseconds fiftyMillis{50};
/*!
 * \brief Class which contains all method to manage ethernet
 *        connection from PC to specific board
 */
class EthInterfaceManager
{
public:


  EthInterfaceManager(const std::string& localAddress, int port, std::chrono::milliseconds timeout_=fiftyMillis);

  /**
     * @brief The destructor is not virtual because the class is Final
     *        and is meant not to be overriden
     */
  ~EthInterfaceManager();

  board_id_t addBoard(string address_, bool tcpService_, int tcpPort_,
      bool udpService_, int udpPort_);

  rehab::RC connectAll();


  void startRecvThread();
  void stopRecvThread();

  /**
   * @brief This function send message over TCP protocol by specific Board
   * @param boardId Identifier of specific board
   * @param packetToBeSent Buffer which contains the message to be sent
   * @return  Number of byte sent otherwise -1, this means error on trasmission data
   */
  int sendTcpFromBoard(const board_id_t boardId,
      const CharBuff* packetToBeSent) const
  {
    return (
        (boardId < this->numConfiguredBoards) ?
            this->boards[boardId]->sendTcp(packetToBeSent) : -1);
  }

  /**
   * @brief This function receive message over TCP protocol by specific Board
   * @param boardId Identifier of specific board
   * @param packetToBeReceived  Buffer which contains the received message
   * @return Number of byte received otherwise -1, this means error .
   */
  int recvTcpFromBoard(const board_id_t boardId,
      CharBuff* packetToBeReceived) const
  {
    return (
        (boardId < this->numConfiguredBoards) ?
            this->boards[boardId]->recvTcp(packetToBeReceived) : -1);
  }

  /**
   * @brief This function send message over UDP protocol by specific Board
   * @param boardId Identifier of specific board
   * @param packetToBeSent Buffer which contains the message to be sent
   * @return  Number of byte sent otherwise -1, this means error on trasmission data
   */
  int sendUdpFromBoard(const uint16_t boardId,
      const CharBuff* packetToBeSent) const
  {
    return (
        (boardId < this->numConfiguredBoards) ?
            this->boards[boardId]->sendUdp(packetToBeSent) : -1);
  }

  int recvUdp(CharBuff* packetToBeReceived) ;

  void installUdpDatagramHandler(const uint16_t boardId,
      udpMessageHandler_t&& msgHandle);

  void installTimeoutHandler(timeoutHandler_t&& msgHandle);

private:
  std::string localAddress;
  std::array<std::unique_ptr<Board>, MAX_NUM_BOARDS> boards;
  std::array< std::chrono::time_point<std::chrono::high_resolution_clock>, MAX_NUM_BOARDS> lastTimePoint;
  uint32_t boardNumIp[MAX_NUM_BOARDS];
  board_id_t numConfiguredBoards;
  int sockId;
  int port;

  rehab::RC bind();

  // Receiver Thread Management
  void worker();
  std::thread workerThread;
  bool workerStarted;
  CharBuff cbUdpRec;
  std::atomic<bool> continuing;
  timeoutHandler_t timeoutHandler;
  std::chrono::milliseconds timeout;
};

} //Ethservice

#endif // ETHSERVICE_H
