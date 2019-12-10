/*
You've just said goodbye to the rebooted rover and left Mars when you receive a faint distress signal coming from the asteroid belt. It must be the Ceres monitoring station!

In order to lock on to the signal, you'll need to boost your sensors. The Elves send up the latest BOOST program - Basic Operation Of System Test.

While BOOST (your puzzle input) is capable of boosting your sensors, for tenuous safety reasons, it refuses to do so until the computer it runs on passes some checks to demonstrate it is a complete Intcode computer.

Your existing Intcode computer is missing one key feature: it needs support for parameters in relative mode.

Parameters in mode 2, relative mode, behave very similarly to parameters in position mode: the parameter is interpreted as a position. Like position mode, parameters in relative mode can be read from or written to.

The important difference is that relative mode parameters don't count from address 0. Instead, they count from a value called the relative base. The relative base starts at 0.

The address a relative mode parameter refers to is itself plus the current relative base. When the relative base is 0, relative mode parameters and position mode parameters with the same value refer to the same address.

For example, given a relative base of 50, a relative mode parameter of -7 refers to memory address 50 + -7 = 43.

The relative base is modified with the relative base offset instruction:

- Opcode 9 adjusts the relative base by the value of its only parameter. The relative base increases (or decreases, if the value is negative) by the value of the parameter.

For example, if the relative base is 2000, then after the instruction 109,19, the relative base would be 2019. If the next instruction were 204,-34, then the value at address 1985 would be output.

Your Intcode computer will also need a few other capabilities:

- The computer's available memory should be much larger than the initial program. Memory beyond the initial program starts with the value 0 and can be read or written like any other memory. (It is invalid to try to access memory at a negative address, though.)
- The computer should have support for large numbers. Some instructions near the beginning of the BOOST program will verify this capability.

Here are some example programs that use these features:

- 109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99 takes no input and produces a copy of itself as output.
- 1102,34915192,34915192,7,4,7,99,0 should output a 16-digit number.
- 104,1125899906842624,99 should output the large number in the middle.

The BOOST program will ask for a single input; run it in test mode by providing it the value 1. It will perform a series of checks on each opcode, output any opcodes (and the associated parameter modes) that seem to be functioning incorrectly, and finally output a BOOST keycode.

Once your Intcode computer is fully functional, the BOOST program should report no malfunctioning opcodes when run in test mode; it should only output a single value, the BOOST keycode. What BOOST keycode does it produce?
*/
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <fstream>
#include <cmath>

#define EXTRA_MEMORY_FACTOR 1000

using namespace std;

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
  cout << "Waiting for input..." << endl;
  cin >> val;
  SetValue(1, val, m, sp, rb);
  sp += 2;
}

void Output(vector<long long>& m, long& sp, long& rb)
{
  long long v1 = GetValue(1, m, sp, rb);
  cout << v1 << endl;
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
  
  return 0;
}

