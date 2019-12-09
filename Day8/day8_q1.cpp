/*
The Elves' spirits are lifted when they realize you have an opportunity to reboot one of their Mars rovers, and so they are curious if you would spend a brief sojourn on Mars. You land your ship near the rover.

When you reach the rover, you discover that it's already in the process of rebooting! It's just waiting for someone to enter a BIOS password. The Elf responsible for the rover takes a picture of the password (your puzzle input) and sends it to you via the Digital Sending Network.

Unfortunately, images sent via the Digital Sending Network aren't encoded with any normal encoding; instead, they're encoded in a special Space Image Format. None of the Elves seem to remember why this is the case. They send you the instructions to decode it.

Images are sent as a series of digits that each represent the color of a single pixel. The digits fill each row of the image left-to-right, then move downward to the next row, filling rows top-to-bottom until every pixel of the image is filled.

Each image actually consists of a series of identically-sized layers that are filled in this way. So, the first digit corresponds to the top-left pixel of the first layer, the second digit corresponds to the pixel to the right of that on the same layer, and so on until the last digit, which corresponds to the bottom-right pixel of the last layer.

For example, given an image 3 pixels wide and 2 pixels tall, the image data 123456789012 corresponds to the following image layers:

Layer 1: 123
         456

Layer 2: 789
         012

The image you received is 25 pixels wide and 6 pixels tall.

To make sure the image wasn't corrupted during transmission, the Elves would like you to find the layer that contains the fewest 0 digits. On that layer, what is the number of 1 digits multiplied by the number of 2 digits?
*/
#include <iostream>
#include <fstream>
#include <vector>

#define PIXEL_WIDTH 25
#define PIXEL_HEIGHT 6

using namespace std;

class Layer
{
public:
  // Method to add a number to the layer.
  // Assumes that we still have room for more data.
  void add_data(int num)
  {
    // Add new row if necessary
    if (data.empty() || data.back().size() == PIXEL_WIDTH)
      {
	vector<int> newRow;
	data.push_back(newRow);
      }
    // Add data
    data.back().push_back(num);
  }

  // Check if data is full.
  bool is_full()
  {
    return (data.size() == PIXEL_HEIGHT &&
	    data.back().size() == PIXEL_WIDTH);
  }

  // Count occurences of a certain number.
  int count_occurences(int num)
  {
    int occ = 0;
    for (auto row: data)
      {
	for (auto n: row)
	  {
	    if (n == num) occ++;
	  }
      }
    return occ;
  }

private:
  vector<vector<int> > data;
};

int main()
{
  fstream f;
  f.open("input.txt");

  // Data for this problem
  vector<Layer> layers;
  Layer currentLayer;
  
  // Populate data
  char n;
  while (f >> n)
    {
      int num = n - '0';
      if (!currentLayer.is_full())
	currentLayer.add_data(num);
      else
	{
	  layers.push_back(currentLayer);
	  Layer newLayer;
	  newLayer.add_data(num);
	  currentLayer = newLayer;
	}
    }
  layers.push_back(currentLayer);

  // Run analysis
  int output;
  int minCount = INT_MAX;
  for (auto l: layers)
    {
      cout << "Occurences: " << l.count_occurences(0) << endl;
      if (l.count_occurences(0) < minCount)
	{
	  minCount = l.count_occurences(0);
	  output = l.count_occurences(1) * l.count_occurences(2);
	}
    }

  cout << "Output: " << output << endl;
  
  return 0;
}
