/*
Now you're ready to decode the image. The image is rendered by stacking the layers and aligning the pixels with the same positions in each layer. The digits indicate the color of the corresponding pixel: 0 is black, 1 is white, and 2 is transparent.

The layers are rendered with the first layer in front and the last layer in back. So, if a given position has a transparent pixel in the first and second layers, a black pixel in the third layer, and a white pixel in the fourth layer, the final image would have a black pixel at that position.

For example, given an image 2 pixels wide and 2 pixels tall, the image data 0222112222120000 corresponds to the following image layers:

Layer 1: 02
         22

Layer 2: 11
         22

Layer 3: 22
         12

Layer 4: 00
         00

Then, the full image can be found by determining the top visible pixel in each position:

- The top-left pixel is black because the top layer is 0.
- The top-right pixel is white because the top layer is 2 (transparent), but the second layer is 1.
- The bottom-left pixel is white because the top two layers are 2, but the third layer is 1.
- The bottom-right pixel is black because the only visible pixel in that position is 0 (from layer 4).

So, the final image looks like this:

01
10

What message is produced after decoding your image?
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

  // This is now public since we'll be reading in in main()
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
  // Master will have the final image
  Layer master = layers.front();
  for (auto l: layers)
    {
      for (int col = 0; col < PIXEL_HEIGHT; col++)
	{
	  for (int row = 0; row < PIXEL_WIDTH; row++)
	  {
	    // Replace transparent pixels
	    if (master.data[col][row] == 2)
	      master.data[col][row] = l.data[col][row];
	  }
	}
    }

  // Now display the images
  cout << "Here is the decoded image:" << endl;
  for (auto row: master.data)
    {
      for (auto n: row)
	{
	  // These characters were chosen to make the image as readable
	  // as possble. The hashtag is sufficiently dense to be visible.
	  // Because I have a dark terminal, I have black as the empty
	  // background and white displaying the image. If you use a
	  // light theme, swap ' ' <-> '#'
	  if (n == 0) cout << ' '; // Black 
	  else        cout << '#'; // White
	}
      cout << endl;
    }
  
  return 0;
}

