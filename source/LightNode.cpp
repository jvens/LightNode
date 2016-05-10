#include "LightNode.hpp"

#define LED_COUNT		27

using namespace std;

void cbUpdate(boost::asio::io_service& ioService, vector<Color>& pixels, LightStrip& strip);

int main() {
	cout << "Starting ioService" << endl;
	boost::asio::io_service ioService;

	cout << "Starting LightStrip" << endl;
	LightStrip strip(LED_COUNT);

	cout << "Starting Communicator" << endl;
	LightNode::Communicator comm(LED_COUNT,
		[&ioService, &strip](vector<Color>& pixels) {
			cbUpdate(ioService, pixels, strip);
		});

	cout << "Updating the LED strip" << endl;
	//Clear the strip
	strip.Display();
	
	cout << "Entering ioService loop" << endl;
	while(1) {
		ioService.run();
		ioService.reset();

		this_thread::sleep_for(chrono::milliseconds(10));
	}


	return 0;
}

void cbUpdate(boost::asio::io_service& ioService, vector<Color>& pixels, LightStrip& strip) {
	//auto cbLambda = [&pixels, &strip]() {
		//cout << "Updating lights!" << endl;

		strip.Set(pixels);
		strip.Display();
	//};

	//ioService.post(cbLambda);
}
