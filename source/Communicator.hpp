#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <thread>
#include <chrono>
#include <functional>
#include <iostream>
#include "Color.hpp"

using namespace std;

namespace LightNode
{
	class Communicator
	{
		public:
			enum NodeType_e {
				NODE_STRIP_DIGITAL = 1,
				NODE_STRIP_ANALOG = 2,
				NODE_STRIP_MATRIX = 3
			};

			Communicator(NodeType_e, uint16_t pixelCount,
				uint16_t sendPort, uint16_t recvPort,
				function<void(vector<Color>&)> cbUpdate);
			~Communicator();

			bool isConnected();

		private:
			static const int BUFFER_SIZE = 65535;
			static const int PORT = 30962;
			static const int HEADER = 0xAA55;

			enum PACKET_ID {
				PING = 0x00,
				INIT = 0x01,
				INFO = 0x02,
				UPDATE = 0x03,
				ALIVE = 0x04,
				ACK = 0xFE,
				NACK = 0xFF};

			void startListening();

			void handleReceive(const boost::system::error_code& error,
				size_t bytesTransferred);

			void handleAliveTimer();

			void startAliveTimer();

			void threadRoutine();

			void sendAck();
			void sendNack();
			void sendInfo();

			void processUpdate(int bytesTransferred);

			//Network stuff
			boost::asio::io_service ioService;
			boost::asio::ip::udp::socket udpSocket;
			boost::asio::ip::udp::endpoint sendEndpoint, recvEndpoint;
			uint16_t sendPort, recvPort;

			//Timer stuff
			boost::asio::deadline_timer aliveTimer;

			thread asyncThread;

			bool connected;
			bool runThread = true;
			uint16_t pixelCount;
			NodeType_e nodeType;
			boost::array<unsigned char, BUFFER_SIZE> readBuf;
			function<void(vector<Color>&)> cbUpdate;
	};
} //namespace LightNode
