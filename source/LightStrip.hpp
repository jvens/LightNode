#ifndef LIGHTSTRIP_HPP
#define LIGHTSTRIP_HPP

#include "Color.hpp"
#include "SPI.h"
#include <exception>
#include <string>
#include <vector>

using namespace std;

class LightStrip
{
public:
	LightStrip() {};
	LightStrip(int count);
	~LightStrip();

	void Set(vector<Color>& colors);
	void Set(int id, Color c);
	void SetAll(Color c);

	void ShiftUp(int amount);
	void ShiftUp(int start, int amount);

	void ShiftDown(int amount);
	void ShiftDown(int start, int amount);

	int GetSize();
	Color GetColor(int id);

	void DumpColors(unsigned char colorArray[]);

	void Display();
private:
	vector<Color> colors;
	int size;

	void MoveColors(int start, int end, int amount);
};

#endif
