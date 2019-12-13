/*
You're not sure what it's trying to paint, but it's definitely not a registration identifier. The Space Police are getting impatient.

Checking your external ship cameras again, you notice a white panel marked "emergency hull painting robot starting panel". The rest of the panels are still black, but it looks like the robot was expecting to start on a white panel, not a black one.

Based on the Space Law Space Brochure that the Space Police attached to one of your windows, a valid registration identifier is always eight capital letters. After starting the robot on a single white panel instead, what registration identifier does it paint on your hull?
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

  // Print the results of the paint job to the console
  void display()
  {
    char white_pixel = '#';
    char black_pixel = ' ';

    // Determine bounds
    int lb_x = INT_MAX, lb_y = INT_MAX;
    int ub_x = INT_MIN, ub_y = INT_MIN;
    for (auto pos: hull)
      {
	int x = pos.first.first, y = pos.first.second;
	lb_x = min(lb_x, x);
	lb_y = min(lb_y, y);
	ub_x = max(ub_x, x);
	ub_y = max(ub_y, y);
      }

    // Display
    // The coordinate grid is defined such that the minumum y is at
    // the top and y increases downwards. Printing reflects this
    for (int y = ub_y; y >= lb_y; y--)
      {
	for (int x = lb_x; x <= ub_x; x++)
	  {
	    pair<int, int> pos = {x, y};
	    if (color(pos) == 0) cout << black_pixel;
	    else                 cout << white_pixel;
	  }
	cout << "\n";
      }
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
      if (v1 == 0) // Turn left
	{
	  switch(robot_dir)
	    {
	    case _up:    robot_dir = _left; break;
	    case _down:  robot_dir = _right; break;
	    case _left:  robot_dir = _down; break;
	    case _right: robot_dir = _up; break;
	    }
	}
      else // Turn right
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

  // Paint the first tile white
  hull.paint(robot_pos, 1);
  
  // Run the computer
  long sp = 0, rb = 0;
  IntComputer(m, sp, rb);

  // Display the resulting paint job
  hull.display();
    
  return 0;
}

