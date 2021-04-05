#include <iostream>
#include <fstream>
#include <array>
#include <random>
#include <chrono>

using namespace std;

float map(float input, float input_begin, float input_end, float output_begin, float output_end);

int mandelbrot(float x_0, float y_0);

/******************************************************************************
Function Title: main

inputs: none
outputs: nothing important

Summary: Generates a bitmap image demonstrating the Mandelbrot function. Picks
randomly between 1 of 3 different color schemes
*******************************************************************************
Pseudocode:

Begin
	Declare int and float variables needed
	Set width and height
	Calculate size in byes (width x height x 3)
	Generate a random number between 1 and 3
	Create bmpheader array and fill with appropriate bytes
	Use the calculated size, add 54 and insert into bmpheader
	bitshifting to line up correct bytes
	Create dibheader array and fill with appropriate bytes
	Use width and insert into dibheader bitshifting to line up bytes
	Use height and insert into dibheader bitshifting to line up bytes
	Use size and insert into dibheader bitshifting to line up bytes
	Declare r, b, and g as unsigned char(byte) for color
	Declare a new ofstream called ofile
	Switch on random number to open the correct file name
	Write bmpheader array to output file
	Write dibheader array to output file
	Set r b and g to 0 (default black)
	Start nested loop using i and j for writing each pixel
	Loop while i < height
		Loop while j < width
			Scale pixel location to scaledx and y using map function
			Pass the scaledx and y value to mandelbrot function,
			setting color to the return value (iteration number)
			Map color value to 0-255 range using map function
			Switch on random to pick appropriate color scheme
			Write each color byte to output file
		EndLoop
		Print percentage done every 10 rows
	EndLoop
	Close output file
End
******************************************************************************/
int main(void)
{
	// Declare int and float variables needed
	int i, j, width, height, size, color, random;
	float scaledx, scaledy;
	// Set width and height
	width = 8000;
	height = 4571;
	// Calculate size in byes (width x height x 3)
	size = (width * height * 3);

	// Generate a random number between 1 and 3
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed1);
	uniform_int_distribution<int> distribution(1,3);
	random = distribution(generator);

	cout << random << endl;

	// Create bmpheader array and fill with appropriate bytes
	array<unsigned char, 14> bmpheader = {
		'B', 'M',
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00,
		0x00, 0x00,
		0x36, 0x00, 0x00, 0x00
	};

	// Use the calculated size, add 54 and insert into bmpheader
	// bitshifting to line up correct bytes
	bmpheader[2] = (unsigned char)((size + 54));
	bmpheader[3] = (unsigned char)((size + 54) >> 8);
	bmpheader[4] = (unsigned char)((size + 54) >> 16);
	bmpheader[5] = (unsigned char)((size + 54) >> 24);

	// Create dibheader array and fill with appropriate bytes
	array<unsigned char, 40> dibheader = {
		0x28, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x01, 0x00,
		0x18, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x13, 0x0B, 0x00, 0x00,
		0x13, 0x0B, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	};

	// Use width and insert into dibheader bitshifting to line up bytes
	dibheader[4] = (unsigned char)(width);
	dibheader[5] = (unsigned char)(width >> 8);
	dibheader[6] = (unsigned char)(width >> 16);
	dibheader[7] = (unsigned char)(width >> 24);

	// Use height and insert into dibheader bitshifting to line up bytes
	dibheader[8] = (unsigned char)(height);
	dibheader[9] = (unsigned char)(height >> 8);
	dibheader[10] = (unsigned char)(height >> 16);
	dibheader[11] = (unsigned char)(height >> 24);

	// Use size and insert into dibheader bitshifting to line up bytes
	dibheader[20] = (unsigned char)(size);
	dibheader[21] = (unsigned char)(size >> 8);
	dibheader[22] = (unsigned char)(size >> 16);
	dibheader[23] = (unsigned char)(size >> 24);

	// Declare r, b, and g as unsigned char(byte) for color
	unsigned char r, b, g;

	// Declare a new ofstream called ofile
	ofstream ofile;
	// Switch on random number to open the correct file name
	switch (random) {
		case 1:
			ofile.open("mandelbrot_1.bmp", ios::out | ios::binary);
			break;
		case 2:
			ofile.open("mandelbrot_2.bmp", ios::out | ios::binary);
			break;
		case 3:
			ofile.open("mandelbrot_3.bmp", ios::out | ios::binary);
			break;
	}

	// Write bmpheader array to output file
	for (auto it = bmpheader.begin(); it != bmpheader.end(); it++)
	{
		ofile << *it;
	}

	// Write dibheader array to output file
	for (auto jt = dibheader.begin(); jt != dibheader.end(); jt++)
	{
		ofile << *jt;
	}


	// Set r b and g to 0 (default black)
	r = 0;
	b = 0;
	g = 0;
	// Start nested loop using i and j for writing each pixel
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			// Scale pixel location to scaledx and y using map function
			scaledx = map(j, 0, 8000, -2.5, 1);
			scaledy = map(i, 0, 4571, -1, 1);

			// Pass the scaledx and y value to mandelbrot function,
			// Setting color to the return value (iteration number)
			color = mandelbrot(scaledx, scaledy);
			// Map color value to 0-255 range using map function
			color = map(color, 0, 1000, 0, 255);

			// Switch on random to pick appropriate color scheme
			switch (random) {
				case 1:
					r = color;
					b = 0;
					g = 0;
					break;
				case 2:
					r = 0;
					b = color;
					g = 0;
					break;
				case 3:
					r = 0;
					b = 0;
					g = color;
					break;
			}

			// Write each color byte to output file
			ofile << b << g << r;
		}
		// Print percentage done every 10 rows
		if (i % 10 == 0) {
			cout << ((float)i / 4570.00) * 100 << "%" << endl;
		}
	}

	// Close output file
	ofile.close();

	return 0;
}

/*
Title: map
Inputs: input to convert
		intput_begin -> beginning of range to convert from
		intput_end -> end of range to convert from
		output_begin -> beginning of range to convert to
		output_end -> end of range to convert to
Outputs: output mapped to new range
 */
float map(float input, float input_begin, float input_end, float output_begin, float output_end)
{
	float output;

	output = (output_begin + ((output_end - output_begin) / (input_end - input_begin)) * (input - input_begin));

	return output;
}

// mandelbrot function taking in the scaled x and y
// returns iteration value
/*
Title: mandelbrot
Inputs: a scaled float x_0
		a scaled float y_0
Outputs: an int containing the iteration of the mandelbrot function reached
Pseudocode: used https://en.wikipedia.org/wiki/Mandelbrot_set as a reference
Begin
	Create floats x and y and set to 0
	Create ints iteration and max_iteration and set to 0 and 1000, respectively
	Set x, y, and iteration to be 0
	Set max_iteration to be 1000
	Create float xtemp
	Loop while (x*x + y*y <= 2*2) && iteration < max_iteration
		Set xtemp to be x*x - y*y + x_0
		Set y to be 2*x*y + y_0
		Set x to be xtemp
		Increment iteration
	EndLoop
	Return iteration
End
*/
int mandelbrot(float x_0, float y_0)
{
	// Create floats x and y and set to 0
	float x = 0.0;
	float y = 0.0;
	// Create ints iteration and max_iteration
	// and set to 0 and 1000, respectively
	int iteration = 0;
	int max_iteration = 1000;
	// Create float xtemp
	float xtemp;

	// Loop while (x*x + y*y <= 2*2) && iteration < max_iteration
	while ((x*x + y*y <= 2*2) && iteration < max_iteration) {
		// Set xtemp to be x*x - y*y + x_0
		xtemp = x*x - y*y + x_0;
		// Set y to be 2*x*y + y_0
		y = 2*x*y + y_0;
		// Set x to be xtemp
		x = xtemp;
		// Increment iteration
		iteration++;
	}
	// Return iteration
	return iteration;
}
