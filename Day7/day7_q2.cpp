/*
It's no good - in this configuration, the amplifiers can't generate a large enough output signal to produce the thrust you'll need. The Elves quickly talk you through rewiring the amplifiers into a feedback loop:

      O-------O  O-------O  O-------O  O-------O  O-------O
0 -+->| Amp A |->| Amp B |->| Amp C |->| Amp D |->| Amp E |-.
   |  O-------O  O-------O  O-------O  O-------O  O-------O |
   |                                                        |
   '--------------------------------------------------------+
                                                            |
                                                            v
                                                     (to thrusters)

Most of the amplifiers are connected as they were before; amplifier A's output is connected to amplifier B's input, and so on. However, the output from amplifier E is now connected into amplifier A's input. This creates the feedback loop: the signal will be sent through the amplifiers many times.

In feedback loop mode, the amplifiers need totally different phase settings: integers from 5 to 9, again each used exactly once. These settings will cause the Amplifier Controller Software to repeatedly take input and produce output many times before halting. Provide each amplifier its phase setting at its first input instruction; all further input/output instructions are for signals.

Don't restart the Amplifier Controller Software on any amplifier during this process. Each one should continue receiving and sending signals until it halts.

All signals sent or received in this process will be between pairs of amplifiers except the very first signal and the very last signal. To start the process, a 0 signal is sent to amplifier A's input exactly once.

Eventually, the software on the amplifiers will halt after they have processed the final loop. When this happens, the last output signal from amplifier E is sent to the thrusters. Your job is to find the largest output signal that can be sent to the thrusters using the new phase settings and feedback loop arrangement.
o
Here are some example programs:

- Max thruster signal 139629729 (from phase setting sequence 9,8,7,6,5):

  3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,
  27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5

- Max thruster signal 18216 (from phase setting sequence 9,7,8,5,6):

  3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,
  -5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,
  53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10

Try every combination of the new phase settings on the amplifier feedback loop. What is the highest signal that can be sent to the thrusters?
*/
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <fstream>

using namespace std;

// Output the signal from the amplifier
// Inputs are a queue of inputs. This is usually a single item; the
// output of the previous amplifier. However, this can also have two
// items if we need to first initialize the phase.
// If output = INT_MAX, the program has stopped
int RunAmplifiers(vector<int>& m, int& sp, deque<int>& inputs)
{
  // Store the values that will be called by the input instruction
  // in this deque.
  int out;
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
	  val = inputs.front(); inputs.pop_front();
	  m[m[sp+1]] = val;
	  sp += 2; break;
	  
	  // Output
	case 4:
	  out = m[m[sp+1]]; sp += 2; return out; break;
	case 104:
	  out = m[sp+1];    sp += 2; return out; break;
	  
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
  return INT_MAX;
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
  deque<int> phases = {5, 6, 7, 8, 9};
  do {
    // Create a deep copy to mutate
    deque<int> p = phases;

    // Initialize all of the amplifier variables
    int signal = 0;
    int spA = 0, spB = 0, spC = 0, spD = 0, spE = 0;
    vector<int> mA = m, mB = m, mC = m, mD = m, mE = m;
    deque<int> deque_sp = {spA, spB, spC, spD, spE};
    deque< vector<int> > deque_m = {mA, mB, mC, mD, mE}; 

    // Initialize amplifiers
    deque<int> inputs;  // Elements removed by RunAmplifiers
    while (!p.empty())
      {
	inputs.push_back(p.front()); p.pop_front();
	inputs.push_back(signal);
	signal = RunAmplifiers(deque_m.front(),
			       deque_sp.front(),
			       inputs);
	// Now cycle the deques
	deque_m.push_back(deque_m.front());   deque_m.pop_front();
	deque_sp.push_back(deque_sp.front()); deque_sp.pop_front();
      }

    // Run the system until completion
    int output;
    while (true)
      {
	inputs.push_back(signal);
	output = RunAmplifiers(deque_m.front(),
			       deque_sp.front(),
			       inputs);
	if (output == INT_MAX)
	  break; // The computers are done
	else
	  signal = output;
	
	// Now cycle the deques
	deque_m.push_back(deque_m.front()); deque_m.pop_front();
	deque_sp.push_back(deque_sp.front()); deque_sp.pop_front();	
      }
    
    best = max(best, signal);
  } while (next_permutation(phases.begin(), phases.end()));
  
  cout << "The best amplification is " << best << endl;
  
  return 0;
}

