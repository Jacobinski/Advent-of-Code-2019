/*
You're starting to sweat as the ship makes its way toward Mercury. The Elves suggest that you get the air conditioner working by upgrading your ship computer to support the Thermal Environment Supervision Terminal.

The Thermal Environment Supervision Terminal (TEST) starts by running a diagnostic program (your puzzle input). The TEST diagnostic program will run on your existing Intcode computer after a few modifications:

First, you'll need to add two new instructions:

- Opcode 3 takes a single integer as input and saves it to the position given by its only parameter. For example, the instruction 3,50 would take an input value and store it at address 50.
- Opcode 4 outputs the value of its only parameter. For example, the instruction 4,50 would output the value at address 50.

Programs that use these instructions will come with documentation that explains what should be connected to the input and output. The program 3,0,4,0,99 outputs whatever it gets as input, then halts.

Second, you'll need to add support for parameter modes:

Each parameter of an instruction is handled based on its parameter mode. Right now, your ship computer already understands parameter mode 0, position mode, which causes the parameter to be interpreted as a position - if the parameter is 50, its value is the value stored at address 50 in memory. Until now, all parameters have been in position mode.

Now, your ship computer will also need to handle parameters in mode 1, immediate mode. In immediate mode, a parameter is interpreted as a value - if the parameter is 50, its value is simply 50.

Parameter modes are stored in the same value as the instruction's opcode. The opcode is a two-digit number based only on the ones and tens digit of the value, that is, the opcode is the rightmost two digits of the first value in an instruction. Parameter modes are single digits, one per parameter, read right-to-left from the opcode: the first parameter's mode is in the hundreds digit, the second parameter's mode is in the thousands digit, the third parameter's mode is in the ten-thousands digit, and so on. Any missing modes are 0.

For example, consider the program 1002,4,3,4,33.

The first instruction, 1002,4,3,4, is a multiply instruction - the rightmost two digits of the first value, 02, indicate opcode 2, multiplication. Then, going right to left, the parameter modes are 0 (hundreds digit), 1 (thousands digit), and 0 (ten-thousands digit, not present and therefore zero):

---------------------------------------------------------------------
ABCDE
 1002

DE - two-digit opcode,      02 == opcode 2
 C - mode of 1st parameter,  0 == position mode
 B - mode of 2nd parameter,  1 == immediate mode
 A - mode of 3rd parameter,  0 == position mode,
                                  omitted due to being a leading zero
---------------------------------------------------------------------

This instruction multiplies its first two parameters. The first parameter, 4 in position mode, works like it did before - its value is the value stored at address 4 (33). The second parameter, 3 in immediate mode, simply has value 3. The result of this operation, 33 * 3 = 99, is written according to the third parameter, 4 in position mode, which also works like it did before - 99 is written to address 4.

Parameters that an instruction writes to will never be in immediate mode.

Finally, some notes:

- It is important to remember that the instruction pointer should increase by the number of values in the instruction after the instruction finishes. Because of the new instructions, this amount is no longer always 4.
- Integers can be negative: 1101,100,-1,4,0 is a valid program (find 100 + -1, store the result in position 4).

The TEST diagnostic program will start by requesting from the user the ID of the system to test by running an input instruction - provide it 1, the ID for the ship's air conditioner unit.

It will then perform a series of diagnostic tests confirming that various parts of the Intcode computer, like parameter modes, function correctly. For each test, it will run an output instruction indicating how far the result of the test was from the expected value, where 0 means the test was successful. Non-zero outputs mean that a function is not working correctly; check the instructions that were run before the output instruction to see which one failed.

Finally, the program will output a diagnostic code and immediately halt. This final output isn't an error; an output followed immediately by a halt means the program finished. If all outputs were zero except the diagnostic code, the diagnostic program ran successfully.

After providing 1 to the only input instruction and passing all the tests, what diagnostic code does the program produce?
*/
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main()
{
  vector<int> m;  // m = memory
  string str;

  ifstream f;
  f.open("program.txt");

  // Load the memory
  while (getline(f, str, ','))
    {
      m.push_back(stoi(str));
    }

  // Begin the Intcode computer
  int sp = 0;
  while (m[sp] != 99)
    {
      // Originally, my switch statement had leading zeros, but then
      // the program wasn't using the case 0101: for m[sp] == 101.
      // Idk why this happens.
      switch (m[sp])
	{
	// Addition
	case 1:
	  m[m[sp+3]] = m[m[sp+1]] + m[m[sp+2]]; sp += 4; break;
	case 101:
	  m[m[sp+3]] = m[sp+1]    + m[m[sp+2]]; sp += 4; break;
	case 1001:
	  m[m[sp+3]] = m[m[sp+1]] + m[sp+2];    sp += 4; break;
	case 1101:
	  m[m[sp+3]] = m[sp+1]    + m[sp+2];    sp += 4; break;

	// Multiplication
	case 2:
	  m[m[sp+3]] = m[m[sp+1]] * m[m[sp+2]]; sp += 4; break;
	case 102:
	  m[m[sp+3]] = m[sp+1]    * m[m[sp+2]]; sp += 4; break;
	case 1002:
	  m[m[sp+3]] = m[m[sp+1]] * m[sp+2];    sp += 4; break;
	case 1102:
	  m[m[sp+3]] = m[sp+1]    * m[sp+2];    sp += 4; break;

	// Input
	case 3:
	  int val; cin >> val; m[m[sp+1]] = val; sp += 2; break;

	// Output
	case 4:
	  cout << m[m[sp+1]] << endl; sp += 2; break;
	case 104:
	  cout << m[sp+1]    << endl; sp += 2; break;

	// Jump-if-true
	case 5:
	  if (m[m[sp+1]] != 0) sp = m[m[sp+2]];
	  else                 sp += 3;          break;
	  break;
	case 105:
	  if (m[sp+1] != 0)    sp = m[m[sp+2]];
	  else                 sp += 3;          break;
	  break;
	case 1005:
	  if (m[m[sp+1]] != 0) sp = m[sp+2];
	  else                 sp += 3;          break;
	  break;
	case 1105:
	  if (m[sp+1] != 0)    sp = m[sp+2];
	  else                 sp += 3;          break;

	// Jump-if-false
	case 6:
	  if (m[m[sp+1]] == 0) sp = m[m[sp+2]];
	  else                 sp += 3;          break;
	  break;
	case 106:
	  if (m[sp+1] == 0)    sp = m[m[sp+2]];
	  else                 sp += 3;          break;
	  break;
	case 1006:
	  if (m[m[sp+1]] == 0) sp = m[sp+2];
	  else                 sp += 3;          break;
	  break;
	case 1106:
	  if (m[sp+1] == 0)    sp = m[sp+2];
	  else                 sp += 3;          break;

	// Less than
	case 7:
	  if (m[m[sp+1]] < m[m[sp+2]]) m[m[sp+3]] = 1;
	  else                         m[m[sp+3]] = 0;
	  sp += 4; break;
	case 107:
	  if (m[sp+1]    < m[m[sp+2]]) m[m[sp+3]] = 1;
	  else                         m[m[sp+3]] = 0;
	  sp += 4; break;
	case 1007:
	  if (m[m[sp+1]] < m[sp+2])    m[m[sp+3]] = 1;
	  else                         m[m[sp+3]] = 0;
	  sp += 4; break;
	case 1107:
	  if (m[sp+1]    < m[sp+2])    m[m[sp+3]] = 1;
	  else                         m[m[sp+3]] = 0;
	  sp += 4; break;
	case 10007:
	  if (m[m[sp+1]] < m[m[sp+2]]) m[sp+3] = 1;
	  else                         m[sp+3] = 0;
          sp += 4; break;
	case 10107:
	  if (m[sp+1]    < m[m[sp+2]]) m[sp+3] = 1;
	  else                         m[sp+3] = 0;
          sp += 4; break;
	case 11007:
	  if (m[m[sp+1]] < m[sp+2])    m[sp+3] = 1;
	  else                         m[sp+3] = 0;
          sp += 4; break;
	case 11107:
	  if (m[sp+1]    < m[sp+2])    m[sp+3] = 1;
	  else                         m[sp+3] = 0;
          sp += 4; break;

	// Equals
	case 8:
	  if (m[m[sp+1]] == m[m[sp+2]]) m[m[sp+3]] = 1;
	  else                          m[m[sp+3]] = 0;
	  sp += 4; break;
	case 108:
	  if (m[sp+1]    == m[m[sp+2]]) m[m[sp+3]] = 1;
	  else                          m[m[sp+3]] = 0;
	  sp += 4; break;
	case 1008:
	  if (m[m[sp+1]] == m[sp+2])    m[m[sp+3]] = 1;
	  else                          m[m[sp+3]] = 0;
	  sp += 4; break;
	case 1108:
	  if (m[sp+1]    == m[sp+2])    m[m[sp+3]] = 1;
	  else                          m[m[sp+3]] = 0;
	  sp += 4; break;
	case 10008:
	  if (m[m[sp+1]] == m[m[sp+2]]) m[sp+3] = 1;
	  else                          m[sp+3] = 0;
          sp += 4; break;
	case 10108:
	  if (m[sp+1]    == m[m[sp+2]]) m[sp+3] = 1;
	  else                          m[sp+3] = 0;
          sp += 4; break;
	case 11008:
	  if (m[m[sp+1]] == m[sp+2])    m[sp+3] = 1;
	  else                          m[sp+3] = 0;
          sp += 4; break;
	case 11108:
	  if (m[sp+1]    == m[sp+2])    m[sp+3] = 1;
	  else                          m[sp+3] = 0;
          sp += 4; break;

	// Default
	default:
	  cout << "Unimplemented code: " << m[sp] << endl; return 0;
	}
    }

  return 0;
}
