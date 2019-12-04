/*
It turns out that this circuit is very timing-sensitive; you actually need to minimize the signal delay.

To do this, calculate the number of steps each wire takes to reach each intersection; choose the intersection where the sum of both wires' steps is lowest. If a wire visits a position on the grid multiple times, use the steps value from the first time it visits that position when calculating the total value of a specific intersection.

The number of steps a wire takes is the total number of grid squares the wire has entered to get to that location, including the intersection being considered. Again consider the example from above:

...........
.+-----+...
.|.....|...
.|..+--X-+.
.|..|..|.|.
.|.-X--+.|.
.|..|....|.
.|.......|.
.o-------+.
...........

In the above example, the intersection closest to the central port is reached after 8+5+5+2 = 20 steps by the first wire and 7+6+4+3 = 20 steps by the second wire for a total of 20+20 = 40 steps.

However, the top-right intersection is better: the first wire takes only 8+5+2 = 15 and the second wire takes only 7+6+2 = 15, a total of 15+15 = 30 steps.

Here are the best steps for the extra examples from above:

- R75,D30,R83,U83,L12,D49,R71,U7,L72
  U62,R66,U55,R34,D71,R55,D58,R83 = 610 steps

- R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51
  U98,R91,D20,R16,D67,R40,U7,R15,U6,R7 = 410 steps

What is the fewest combined steps the wires must take to reach an intersection?
*/
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

int main()
{
  // We can use the same input as before for this question
  freopen("day3_q1_input.txt", "r", stdin);

  // Read the data into a useable format
  // Quite painful.
  string inputA, inputB;
  getline(cin, inputA);
  getline(cin, inputB);

  stringstream ssA, ssB;
  ssA << inputA;
  ssB << inputB;

  vector< pair<char, int> > wireA, wireB;
  string s;
  while (getline(ssA, s, ','))
    {
      wireA.push_back({s.at(0), stoi(s.substr(1, s.length()-1))});
    }
  while (getline(ssB, s, ','))
    {
      wireB.push_back({s.at(0), stoi(s.substr(1, s.length()-1))});
    }

  // This time we use a map of location -> distance
  // Maps will not insert elements if they already exist.
  // Distance is always increasing and we want the min to be in
  // the map, so this is perfect!
  map< pair<int, int>, long > visited;
  map< char, pair<int, int> > dirs =
    {{'R',{1,0}}, {'L',{-1,0}}, {'U',{0,1}}, {'D',{0,-1}}}; 

  int x = 0, y = 0;
  long steps = 0;
  for (pair<char, int> p : wireA)
    {
      pair<int, int> d = dirs[p.first];
      for (int i = p.second; i > 0; i-- )
	{
	  x += d.first;
	  y += d.second;
	  visited.insert({{x, y}, ++steps});
	}
    }

  // Now use wireB and find the minimum intersection
  long minIntersection = LONG_MAX;
  x = 0, y = 0, steps = 0;
  for (pair<char, int> p : wireB)
    {
      pair<int, int> d = dirs[p.first];
      for (int i = p.second; i > 0; i-- )
	{
	  x += d.first;
	  y += d.second;
	  steps++;

	  pair<int, int> p = {x, y};
	  if (visited.find(p) != visited.end())
	    {
	      minIntersection = min(minIntersection,
				    (*visited.find(p)).second + steps);
	    }
	}
    }

  cout << minIntersection << "\n";

  return 0;
}
