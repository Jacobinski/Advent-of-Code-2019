/*
The gravity assist was successful, and you're well on your way to the Venus refuelling station. During the rush back on Earth, the fuel management system wasn't completely installed, so that's next on the priority list.

Opening the front panel reveals a jumble of wires. Specifically, two wires are connected to a central port and extend outward on a grid. You trace the path each wire takes as it leaves the central port, one wire per line of text (your puzzle input).

The wires twist and turn, but the two wires occasionally cross paths. To fix the circuit, you need to find the intersection point closest to the central port. Because the wires are on a grid, use the Manhattan distance for this measurement. While the wires do technically cross right at the central port where they both start, this point does not count, nor does a wire count as crossing with itself.

For example, if the first wire's path is R8,U5,L5,D3, then starting from the central port (o), it goes right 8, up 5, left 5, and finally down 3:

...........
...........
...........
....+----+.
....|....|.
....|....|.
....|....|.
.........|.
.o-------+.
...........

Then, if the second wire's path is U7,R6,D4,L4, it goes up 7, right 6, down 4, and left 4:

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

These wires cross at two locations (marked X), but the lower-left one is closer to the central port: its distance is 3 + 3 = 6.

Here are a few more examples:

R75,D30,R83,U83,L12,D49,R71,U7,L72
U62,R66,U55,R34,D71,R55,D58,R83 = distance 159

R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51
U98,R91,D20,R16,D67,R40,U7,R15,U6,R7 = distance 135

What is the Manhattan distance from the central port to the closest intersection?
*/
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <map>

using namespace std;

int main()
{
  // For the longest time, I couldn't get this to work on OSX
  // It turns out that there is TTY limit of 1024 characters
  // that can be input before the terminal stops accepting inputs. :(
  // https://superuser.com/questions/264407/macosx-10-6-7-cuts-off-stdin-at-1024-chars
  // This is a work-around
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

  set< pair<int, int> > visited;
  map< char, pair<int, int> > dirs =
    {{'R',{1,0}}, {'L',{-1,0}}, {'U',{0,1}}, {'D',{0,-1}}};

  int x = 0, y = 0;
  for (pair<char, int> p : wireA)
    {
      pair<int, int> d = dirs[p.first];
      for (int i = p.second; i > 0; i-- )
	{
	  x += d.first;
	  y += d.second;
	  visited.insert({x, y});
	}
    }

  // Now use wireB and find the minimum intersection
  int minIntersection = INT_MAX;
  x = 0, y = 0;
  for (pair<char, int> p : wireB)
    {
      pair<int, int> d = dirs[p.first];
      for (int i = p.second; i > 0; i-- )
	{
	  x += d.first;
	  y += d.second;

	  pair<int, int> p = {x, y};
	  if (visited.find(p) != visited.end())
	    {
	      minIntersection = min(minIntersection, abs(x) + abs(y));
	    }
	}
    }

  cout << minIntersection << "\n";

  return 0;
}
