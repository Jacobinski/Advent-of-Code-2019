/*
You arrive at the Venus fuel depot only to discover it's protected by a password. The Elves had written the password on a sticky note, but someone threw it out.

However, they do remember a few key facts about the password:

- It is a six-digit number.
- The value is within the range given in your puzzle input.
- Two adjacent digits are the same (like 22 in 122345).
- Going from left to right, the digits never decrease; they only ever increase or stay the same (like 111123 or 135679).

Other than the range rule, the following are true:

- 111111 meets these criteria (double 11, never decreases).
- 223450 does not meet these criteria (decreasing pair of digits 50).
- 123789 does not meet these criteria (no double).

How many different passwords within the range given in your puzzle input meet these criteria?
*/
#include <iostream>
#include <string>

using namespace std;

bool isSixDigits(int x)
{
  return (x == x % 1000000);
}

bool containsPair(int x)
{
  return (x       % 10 == x/10     % 10 ||
	  x/10    % 10 == x/100    % 10 ||
	  x/100   % 10 == x/1000   % 10 ||
	  x/1000  % 10 == x/10000  % 10 ||
	  x/10000 % 10 == x/100000 % 10 );
}

bool isIncreasing(int x)
{
  return (x       % 10 >= x/10     % 10 &&
	  x/10    % 10 >= x/100    % 10 &&
	  x/100   % 10 >= x/1000   % 10 &&
	  x/1000  % 10 >= x/10000  % 10 &&
	  x/10000 % 10 >= x/100000 % 10 );
}

int main()
{
  string lb, ub;
  getline(cin, lb, '-');
  getline(cin, ub);

  int lowerBound = stoi(lb);
  int upperBound = stoi(ub);

  int total = 0;
  for (int i=lowerBound; i <= upperBound; i++ )
    {
      if (isSixDigits(i) && containsPair(i) && isIncreasing(i))
	total++;
    }
  cout << total << endl;
  return 0;
}
