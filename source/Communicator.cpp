#include "Communicator.hpp"

LightNode::Communicator::Communicator(unsigned int pixelCount,
	function<void(vector<Color>&)> cbUpdate)
	: udpSocket(ioService, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), PORT))
	, asyncThread(bind(&LightNode::Communicator::threadRoutine, this))
	, aliveTimer(ioService){

	connected = false;

	this->pixelCount = pixelCount;
	this->cbUpdate = cbUpdate;

	startAliveTimer();

	startListening();
}

LightNode::Communicator::~Communicator() {
	runThread = false;

	asyncThread.join();
}

bool LightNode::Communicator::isConnected() {
	return connected;
}

void LightNode::Communicator::startListening() {
	udpSocket.async_receive_from(boost::asio::buffer(readBuf),
		udpEndpoint,
		boost::bind(&LightNode::Communicator::handleReceive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void LightNode::Communicator::handleReceive(const boost::system::error_code& error,
	size_t bytesTransferred) {

	if(bytesTransferred > 2) {
		//Check for the correct header
		int readHeader = readBuf[0] << 8 | readBuf[1];
		unsigned char readId = readBuf[2];

		//If the packet header is correct
		if(readHeader == HEADER) {
			switch(readId) {
				case PACKET_ID::INIT:
					sendConfigMessage();
					connected = true;
				break;

				case PACKET_ID::UPDATE:
					processUpdate(bytesTransferred);
				break;

				default:
					cerr << "Unimplemented packet id received: " << readId << endl;
				break;
			}
		}
		else { //The packet header was not correct
			cout << "Packet received with incorrect header: " << readHeader << endl;
		}
	}

	startListening();
}

void LightNode::Communicator::threadRoutine() {
	while(runThread) {
		this_thread::sleep_for(chrono::milliseconds(10));
		
		ioService.run();
		ioService.reset();
	}
}

void LightNode::Communicator::handleAliveTimer() {
	if(!connected) {
		cout << "Alive timer: not connected" << endl;
	}
	else {
	vector<unsigned char> message;

	//Push the header
	message.push_back( (HEADER >> 8) & 0xFF );
	message.push_back( HEADER & 0xFF );

	//Push the identification
	message.push_back( PACKET_ID::ALIVE );

	try {
		udpSocket.send_to(boost::asio::buffer(message), udpEndpoint);
	}
	catch(exception& e) {
		cerr << "sendAck exception caught: " << e.what() << endl;
	}

	cout << "Alive message sent" << endl;
	}

	startAliveTimer();
}

void LightNode::Communicator::startAliveTimer() {
	aliveTimer.expires_from_now(boost::posix_time::seconds(1));
	aliveTimer.async_wait([this](const boost::system::error_code& e) {
		handleAliveTimer();
	});
}


void LightNode::Communicator::sendAck() {
	vector<unsigned char> message;

	//Push the header
	message.push_back( (HEADER >> 8) & 0xFF );
	message.push_back( HEADER & 0xFF );

	//Push the identification
	message.push_back( PACKET_ID::ACK );

	try {
		udpSocket.send_to(boost::asio::buffer(message), udpEndpoint);
	}
	catch(exception& e) {
		cerr << "sendAck exception caught: " << e.what() << endl;
	}
}

void LightNode::Communicator::sendNack() {
	vector<unsigned char> message;

	//Push the header
	message.push_back( (HEADER >> 8) & 0xFF );
	message.push_back( HEADER & 0xFF );

	//Push the identification
	message.push_back( PACKET_ID::NACK );

	try {
		udpSocket.send_to(boost::asio::buffer(message), udpEndpoint);
	}
	catch(exception& e) {
		cerr << "sendAck exception caught: " << e.what() << endl;
	}
}

void LightNode::Communicator::sendConfigMessage() {
	vector<unsigned char> message;

	//Push the header
	message.push_back( (HEADER >> 8) & 0xFF );
	message.push_back( HEADER & 0xFF );

	//Push the identification
	message.push_back( PACKET_ID::CONFIG );

	//Push the pixel count
	message.push_back( (pixelCount >> 8) & 0xFF );
	message.push_back( pixelCount & 0xFF );

	try {
		udpSocket.send_to(boost::asio::buffer(message), udpEndpoint);
	}
	catch(exception& e) {
		cerr << "sendConfigMessage exception caught: " << e.what() << endl;
	}
}

void LightNode::Communicator::processUpdate(int bytesTransferred) {
	int payloadLength = bytesTransferred - 3;

	if(payloadLength != (3*pixelCount)) {
		cerr << "processUpdate error: invalid payload length " << payloadLength << endl;

		sendNack();

		return;
	}

	vector<Color> pixels;

	for(int i = 0; i < payloadLength; i += 3) {
		pixels.emplace_back(readBuf[i], readBuf[i+1], readBuf[i+2]);
	}

	try {
	//Call external callback for update event
	cbUpdate(pixels);
	}
	catch(exception &e) {
		cerr << "processUpdate error: " << e.what() << endl;
	}

	//Signal that packet processing is complete
	//sendAck();
}
