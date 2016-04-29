#include "LightNode.hpp"

#define LED_COUNT		240

using namespace std;

int main() {
	LightStrip strip(LED_COUNT);

	strip.Set(0, Color(255, 0, 0));
	
	while(1) {
		Color lastColor = strip.GetColor(LED_COUNT-1);

		strip.ShiftUp(1);
		strip.Set(0, lastColor);

		strip.Display();

		//this_thread::sleep_for(chrono::milliseconds(10));
	}


	return 0;
}
