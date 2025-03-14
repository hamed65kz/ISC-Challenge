#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>


using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std::chrono;

constexpr int MAX_MEMBERS = 999;
constexpr size_t MESSAGE_LENGTH = 32;

/**
 * @brief Represents a session for a connected member
 * 
 * This class manages the communication between the server and a member,
 * handling message processing, member identification, and message forwarding
 * to other members in the session.
 */
class MemberSession : public std::enable_shared_from_this<MemberSession> {
    public:
     /**
      * @brief Constructs a MemberSession with the given socket and member list.
      * @param socket The TCP socket for the session.
      * @param members A reference to the unordered map of active member sessions.
      */
     MemberSession(
         tcp::socket socket,
         std::unordered_map<int, std::shared_ptr<MemberSession>>& members)
         : socket_(std::move(socket)), members_(members) {
     }

     /**
      * @brief Starts the session by initiating the handshake process.
      */
     void start();

    private:
     /**
      * @brief Handles the initial handshake with the connected member.
      */
     void handle_handshake();

     /**
      * @brief Begins reading messages from the socket.
      */
     void start_reading();

     /**
      * @brief Processes an incoming message from the member.
      */
     void process_message();

     /**
      * @brief Forwards a message to the member in the session.
      * @param message The message to be forwarded.
      */
     void forward_message(const std::string& message);

     tcp::socket socket_; ///< The TCP socket for this member's session.
     std::array<char, 3> id_buffer_; ///< Buffer for storing the member ID.
     std::array<char, MESSAGE_LENGTH> message_buffer_; ///< Buffer for incoming messages.
     int id_ = -1; ///< Unique identifier for the member.
     std::unordered_map<int, std::shared_ptr<MemberSession>>& members_; ///< Reference to active members.
};


   #endif