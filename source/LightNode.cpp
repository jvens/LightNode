#include "LightNode.hpp"

#define LED_COUNT		(32*24)

#define SEND_PORT	54924
#define RECV_PORT	54923

using namespace std;

void cbUpdate(boost::asio::io_service& ioService, vector<Color>& pixels, LightStrip& strip);

int main(int argc, char* argv[]) {
	int ledCount = LED_COUNT;

	if(argc == 2) {
		ledCount = std::stoi(argv[1]);

		std::cout << "[Info] LED count " << ledCount << std::endl;
	}
	else {
		std::cout << "[Info] Using default LED count " << ledCount << std::endl;
	}


	boost::asio::io_service ioService;

	LightStrip strip(ledCount);

	cout << "[Info] Starting Communicator" << endl;
	LightNode::Communicator comm(LightNode::Communicator::NODE_STRIP_DIGITAL,
		ledCount, SEND_PORT, RECV_PORT,
		[&ioService, &strip](vector<Color>& pixels) {
			cbUpdate(ioService, pixels, strip);
		});

	//Clear the strip
	strip.Display();
	
	while(1) {
		ioService.run();
		ioService.reset();

		this_thread::sleep_for(chrono::milliseconds(1000));
	}


	return 0;
}

void cbUpdate(boost::asio::io_service& ioService, vector<Color>& pixels, LightStrip& strip) {
	//auto cbLambda = [&pixels, &strip]() {
		cout << "Updating lights: " << pixels[0].ToString() << endl;

		strip.Set(pixels);
		strip.Display();
	//};

	//ioService.post(cbLambda);
}
