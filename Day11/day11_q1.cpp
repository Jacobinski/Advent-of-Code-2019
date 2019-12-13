/*
On the way to Jupiter, you're pulled over by the Space Police.

"Attention, unmarked spacecraft! You are in violation of Space Law! All spacecraft must have a clearly visible registration identifier! You have 24 hours to comply or be sent to Space Jail!"

Not wanting to be sent to Space Jail, you radio back to the Elves on Earth for help. Although it takes almost three hours for their reply signal to reach you, they send instructions for how to power up the emergency hull painting robot and even provide a small Intcode program (your puzzle input) that will cause it to paint your ship appropriately.

There's just one problem: you don't have an emergency hull painting robot.

You'll need to build a new emergency hull painting robot. The robot needs to be able to move around on the grid of square panels on the side of your ship, detect the color of its current panel, and paint its current panel black or white. (All of the panels are currently black.)

The Intcode program will serve as the brain of the robot. The program uses input instructions to access the robot's camera: provide 0 if the robot is over a black panel or 1 if the robot is over a white panel. Then, the program will output two values:

- First, it will output a value indicating the color to paint the panel the robot is over: 0 means to paint the panel black, and 1 means to paint the panel white.
- Second, it will output a value indicating the direction the robot should turn: 0 means it should turn left 90 degrees, and 1 means it should turn right 90 degrees.

After the robot turns, it should always move forward exactly one panel. The robot starts facing up.

The robot will continue running for a while like this and halt when it is finished drawing. Do not restart the Intcode computer inside the robot during this process.

For example, suppose the robot is about to start running. Drawing black panels as ., white panels as #, and the robot pointing the direction it is facing (< ^ > v), the initial state and region near the robot looks like this:

.....
.....
..^..
.....
.....
The panel under the robot (not visible here because a ^ is shown instead) is also black, and so any input instructions at this point should be provided 0. Suppose the robot eventually outputs 1 (paint white) and then 0 (turn left). After taking these actions and moving forward one panel, the region now looks like this:

.....
.....
.<#..
.....
.....

Input instructions should still be provided 0. Next, the robot might output 0 (paint black) and then 0 (turn left):

.....
.....
..#..
.v...
.....

After more outputs (1,0, 1,0):

.....
.....
..^..
.##..
.....

The robot is now back where it started, but because it is now on a white panel, input instructions should be provided 1. After several more outputs (0,1, 1,0, 1,0), the area looks like this:

.....
..<#.
...#.
.##..
.....

Before you deploy the robot, you should probably have an estimate of the area it will cover: specifically, you need to know the number of panels it paints at least once, regardless of color. In the example above, the robot painted 6 panels at least once. (It painted its starting panel twice, but that panel is still only counted once; it also never painted the panel it ended on.)

Build a new emergency hull painting robot and run the Intcode program on it. How many panels does it paint at least once?
 */
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <set>
#include <map>

#define EXTRA_MEMORY_FACTOR 1000

using namespace std;

enum direction { _up, _down, _left, _right };

class Hull
{
public:  
  void paint(pair<int, int> pos, int color)
  {
    cout << "Painting " << pos.first << ", " << pos.second << "the color "
	 << color << endl;
    hull[pos] = color;
  }

  // Color of a position
  int color(pair<int, int> pos)
  {
    if (hull.find(pos) == hull.end()) return 0;
    else                              return hull[pos];
  }
    
  int count_painted_tiles()
  {
    return hull.size();
  }
  
private:
  map<pair<int, int>, int> hull;  // Position -> color
};

// Global variables to store information about the robot
Hull hull;
direction robot_dir = _up;
pair<int, int> robot_pos;

// This is a helper function to retrieve on input given a certain
// parameter value. Position is 1-indexed. The first position is pos=1,
// the next pos=2, and so on...
long long GetValue(int pos, vector<long long>& m, long& sp, long& rb)
{
  long long val;
  switch ( (long long)(m[sp]/pow(10,pos+1))%10 )
    {
    case 0: val = m[m[sp+pos]];    break;
    case 1: val = m[sp+pos];       break;
    case 2: val = m[rb+m[sp+pos]]; break;
    }
  // cout << "[Get] SP: " << sp << " V: " << val << endl;
  return val;
}

// Similar to GetValue, this is 1-indexed. However, we also count inputs
// in the index, so if we have two inputs, one output, we would set pos=3
//
// Output should always be in position mode, where their mode = 0.
// Input can be in any of the modes (input being opcode 3), so we need to
// check the mode before setting the position. Checking the mode for an
// non-parameter (output variable) will always result in 0 due to the way
// The opcodes are designed.
void SetValue(int pos, long long val, vector<long long>& m,
	      long& sp, long& rb)
{
  switch ( (long long)(m[sp]/pow(10,pos+1))%10 )
    {
    case 0: m[m[sp+pos]]    = val; break;
    case 1: m[sp+pos]       = val; break;
    case 2: m[rb+m[sp+pos]] = val; break;
    }
}

void Add(vector<long long>& m, long& sp, long& rb)
{
  long long v1 = GetValue(1, m, sp, rb);
  long long v2 = GetValue(2, m, sp, rb);
  SetValue(3, v1 + v2, m, sp, rb);
  sp += 4;
}

void Multiply(vector<long long>& m, long& sp, long& rb)
{
  long long v1 = GetValue(1, m, sp, rb);
  long long v2 = GetValue(2, m, sp, rb);
  SetValue(3, v1 * v2, m, sp, rb);
  sp += 4;
}

void Input(vector<long long>& m, long& sp, long& rb)
{
  long long val;
  val = hull.color(robot_pos); 
  SetValue(1, val, m, sp, rb);
  sp += 2;
}

void Output(vector<long long>& m, long& sp, long& rb)
{
  // Two output modes:
  // 1. Color to paint hull
  // 2. Direction to turn robot
  static bool first_output_mode = true;
  long long v1 = GetValue(1, m, sp, rb);
  if (first_output_mode)
    {
      hull.paint(robot_pos, (int)v1);
    }
  else
    {
      if (v1 == 0) // Turn _left
	{
	  switch(robot_dir)
	    {
	    case _up:    robot_dir = _left; break;
	    case _down:  robot_dir = _right; break;
	    case _left:  robot_dir = _down; break;
	    case _right: robot_dir = _up; break;
	    }
	}
      else // Turn _right
	{
	  switch(robot_dir)
	    {
	    case _up:    robot_dir = _right; break;
	    case _down:  robot_dir = _left; break;
	    case _left:  robot_dir = _up; break;
	    case _right: robot_dir = _down; break;
	    }
	}
      // Move forward
      switch(robot_dir)
	{
	case _up:    robot_pos.second += 1; break;
	case _down:  robot_pos.second -= 1; break;
	case _left:  robot_pos.first -= 1; break;
	case _right: robot_pos.first += 1; break;
	}
    }
  first_output_mode = !first_output_mode;
  sp += 2;
}

void JumpIfTrue(vector<long long>& m, long& sp, long& rb)
{
  long long v1 = GetValue(1, m, sp, rb);
  long long v2 = GetValue(2, m, sp, rb);
  if (v1 != 0) sp = v2;
  else         sp += 3;
} 

void JumpIfFalse(vector<long long>& m, long& sp, long& rb)
{
  long long v1 = GetValue(1, m, sp, rb);
  long long v2 = GetValue(2, m, sp, rb);
  if (v1 == 0) sp = v2;
  else         sp += 3;
}

void LessThan(vector<long long>& m, long& sp, long& rb)
{
  long long v1 = GetValue(1, m, sp, rb);
  long long v2 = GetValue(2, m, sp, rb);
  if (v1 < v2) SetValue(3, 1, m, sp, rb);
  else         SetValue(3, 0, m, sp, rb);
  sp += 4;
}

void Equals(vector<long long>& m, long& sp, long& rb)
{
  long long v1 = GetValue(1, m, sp, rb);
  long long v2 = GetValue(2, m, sp, rb);
  if (v1 == v2) SetValue(3, 1, m, sp, rb);
  else          SetValue(3, 0, m, sp, rb);
  sp += 4;
}

void AdjustRelativeBase(vector<long long>& m, long& sp, long& rb)
{
  long long v1 = GetValue(1, m, sp, rb);
  rb += (long) v1;
  sp += 2;
}

// Outputs the signal from the amplifier
// m = memory, sp = stack pointer, rb = relative base
void IntComputer(vector<long long>& m, long& sp, long& rb)
{
  while (m[sp] != 99)
    {
      // cout << "CODE: " << m[sp] % 100 << endl;
      switch (m[sp] % 100)
	{
	case 1:	Add(m, sp, rb);                break;
	case 2: Multiply(m, sp, rb);           break;
	case 3: Input(m, sp, rb);              break;
	case 4: Output(m, sp, rb);             break;
	case 5: JumpIfTrue(m, sp, rb);         break;
	case 6: JumpIfFalse(m, sp, rb);        break;
	case 7: LessThan(m, sp, rb);           break;
	case 8: Equals(m, sp, rb);             break;
	case 9: AdjustRelativeBase(m, sp, rb); break;
	default:
	  cout << "Unimplemented code: " << m[sp] << endl;
	}
      // cout << "SP: " << sp << " RB: " << rb << endl;
    }
  // cout << "CODE: 99" << endl;
}

int main()
{
  vector<long long> m;  // m = memory
  string str;
  
  ifstream f;
  f.open("program.txt");
  
  // Load the memory
  while (getline(f, str, ','))
    {
      m.push_back(stoll(str));
    }
  
  // Add a bunch of memory for operations
  int size = EXTRA_MEMORY_FACTOR * m.size();
  for (int i = 0; i < size; i++)
    m.push_back(0);
  
  // Run the computer
  long sp = 0, rb = 0;
  IntComputer(m, sp, rb);

  cout << "The robot painted " << hull.count_painted_tiles()
       << " tiles at least once!" << endl;
  
  return 0;
}

