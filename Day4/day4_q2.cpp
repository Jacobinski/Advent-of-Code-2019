/*
An Elf just remembered one more important detail: the two adjacent matching digits are not part of a larger group of matching digits.

Given this additional criterion, but still ignoring the range rule, the following are now true:

- 112233 meets these criteria because the digits never decrease and all repeated digits are exactly two digits long.
- 123444 no longer meets the criteria (the repeated 44 is part of a larger group of 444).
- 111122 meets the criteria (even though 1 is repeated more than twice, it still contains a double 22).

How many different passwords within the range given in your puzzle input meet all of the criteria?
*/

#include <iostream>
#include <string>

using namespace std;

bool isSixDigits(int x)
{
  return (x == x % 1000000);
}

bool containsExclusivePair(int x)
{
  // This code is hacky; we assume six digits.
  // A number is represented as 'abcdef', where each letter is a variable
  // representing its digit.
  int a, b, c, d, e, f;
  int* digits []= {&f, &e, &d, &c, &b, &a};

  // Populate the variables in our given ordering using pointer stuff.
  for (int i=0; i<6; i++)
    {
      *(digits[i]) = x % 10;
      x /= 10;
    }

  // Do the case comparions
  return ((a == b && b != c)           ||
	  (a != b && b == c && c != d) ||
	  (b != c && c == d && c != e) ||
	  (c != d && d == e && e != f) ||
	  (d != e && e == f));
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
      if (isSixDigits(i) && containsExclusivePair(i) && isIncreasing(i))
	total++;
    }
  cout << total << endl;
  return 0;
}
