#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

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
			Communicator(unsigned int pixelCount, function<void(vector<Color>&)> cbUpdate);
			~Communicator();

			bool isConnected();

		private:
			static const int BUFFER_SIZE = 65535;
			static const int PORT = 30962;
			static const int HEADER = 0xAA55;

			enum PACKET_ID {
				INIT = 0x00,
				CONFIG = 0x01,
				UPDATE = 0x02,
				ACK = 0x03,
				NACK = 0xFF};

			void startListening();

			void handleReceive(const boost::system::error_code& error,
				size_t bytesTransferred);

			void threadRoutine();

			void sendAck();
			void sendNack();
			void sendConfigMessage();

			void processUpdate(int bytesTransferred);

			boost::asio::io_service ioService;
			boost::asio::ip::udp::socket udpSocket;
			boost::asio::ip::udp::endpoint udpEndpoint;

			thread asyncThread;

			bool connected;
			bool runThread = true;
			unsigned int pixelCount;
			boost::array<unsigned char, BUFFER_SIZE> readBuf;
			function<void(vector<Color>&)> cbUpdate;
	};
} //namespace LightNode

#endif
