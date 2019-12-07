/*
Based on the navigational maps, you're going to need to send more power to your ship's thrusters to reach Santa in time. To do this, you'll need to configure a series of amplifiers already installed on the ship.

There are five amplifiers connected in series; each one receives an input signal and produces an output signal. They are connected such that the first amplifier's output leads to the second amplifier's input, the second amplifier's output leads to the third amplifier's input, and so on. The first amplifier's input value is 0, and the last amplifier's output leads to your ship's thrusters.

    O-------O  O-------O  O-------O  O-------O  O-------O
0 ->| Amp A |->| Amp B |->| Amp C |->| Amp D |->| Amp E |-> (to thrusters)
    O-------O  O-------O  O-------O  O-------O  O-------O

The Elves have sent you some Amplifier Controller Software (your puzzle input), a program that should run on your existing Intcode computer. Each amplifier will need to run a copy of the program.

When a copy of the program starts running on an amplifier, it will first use an input instruction to ask the amplifier for its current phase setting (an integer from 0 to 4). Each phase setting is used exactly once, but the Elves can't remember which amplifier needs which phase setting.

The program will then call another input instruction to get the amplifier's input signal, compute the correct output signal, and supply it back to the amplifier with an output instruction. (If the amplifier has not yet received an input signal, it waits until one arrives.)

Your job is to find the largest output signal that can be sent to the thrusters by trying every possible combination of phase settings on the amplifiers. Make sure that memory is not shared or reused between copies of the program.

For example, suppose you want to try the phase setting sequence 3,1,2,4,0, which would mean setting amplifier A to phase setting 3, amplifier B to setting 1, C to 2, D to 4, and E to 0. Then, you could determine the output signal that gets sent from amplifier E to the thrusters with the following steps:

- Start the copy of the amplifier controller software that will run on amplifier A. At its first input instruction, provide it the amplifier's phase setting, 3. At its second input instruction, provide it the input signal, 0. After some calculations, it will use an output instruction to indicate the amplifier's output signal.

- Start the software for amplifier B. Provide it the phase setting (1) and then whatever output signal was produced from amplifier A. It will then produce a new output signal destined for amplifier C.

- Start the software for amplifier C, provide the phase setting (2) and the value from amplifier B, then collect its output signal.

-Run amplifier D's software, provide the phase setting (4) and input value, and collect its output signal.

- Run amplifier E's software, provide the phase setting (0) and input value, and collect its output signal.

- The final output signal from amplifier E would be sent to the thrusters. However, this phase setting sequence may not have been the best one; another sequence might have sent a higher signal to the thrusters.

Here are some example programs:

- Max thruster signal 43210 (from phase setting sequence 4,3,2,1,0):

  3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0

- Max thruster signal 54321 (from phase setting sequence 0,1,2,3,4):

  3,23,3,24,1002,24,10,24,1002,23,-1,23,
  101,5,23,23,1,24,23,23,4,23,99,0,0

- Max thruster signal 65210 (from phase setting sequence 1,0,4,3,2):

  3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,
  1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0

Try every combination of phase settings on the amplifiers. What is the highest signal that can be sent to the thrusters?
*/
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <fstream>

using namespace std;

// Returns output of final amplifier
int RunAmplifiers(vector<int> m, int phase, int signal)
{
  // Store the values that will be called by the input instruction
  // in this deque.
  deque<int> inputDeq;
  inputDeq.push_back(phase);
  inputDeq.push_back(signal);

  int out;
  int sp = 0;
  while (m[sp] != 99)
    {
      // Originally, my switch statement had leading zeros, but then
      // the program wasn't using the case 0101: for m[sp] == 101.
      // Idk why this happens.
      int val;
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
	  val = inputDeq.front(); inputDeq.pop_front();
	  m[m[sp+1]] = val;
	  sp += 2; break;
	  
	  // Output
	case 4:
	  out = m[m[sp+1]]; sp += 2; break;
	case 104:
	  out = m[sp+1];    sp += 2; break;
	  
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
	  cout << "Unimplemented code: " << m[sp] << endl;
	  return 0;
	}
    }
  return out;
}

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

  // Create vector and try all permutations
  int best = INT_MIN;
  deque<int> phases = {0, 1, 2, 3, 4};
  do {
    // Create a deep copy to mutate
    deque<int> p = phases;

    // Input signal is the input to to the next module, which is
    // equivalent to the output of the previous module
    int input_signal = 0;
    while (!p.empty()){
      input_signal = RunAmplifiers(m, p.front(), input_signal);
      p.pop_front();
    }
    best = max(best, input_signal);
  } while (next_permutation(phases.begin(), phases.end()));
  
  cout << "The best amplification is " << best << endl;
  
  return 0;
}

