/*
Once you give them the coordinates, the Elves quickly deploy an Instant Monitoring Station to the location and discover the worst: there are simply too many asteroids.

The only solution is complete vaporization by giant laser.

Fortunately, in addition to an asteroid scanner, the new monitoring station also comes equipped with a giant rotating laser perfect for vaporizing asteroids. The laser starts by pointing up and always rotates clockwise, vaporizing any asteroid it hits.

If multiple asteroids are exactly in line with the station, the laser only has enough power to vaporize one of them before continuing its rotation. In other words, the same asteroids that can be detected can be vaporized, but if vaporizing one asteroid makes another one detectable, the newly-detected asteroid won't be vaporized until the laser has returned to the same position by rotating a full 360 degrees.

For example, consider the following map, where the asteroid with the new monitoring station (and laser) is marked X:

.#....#####...#..
##...##.#####..##
##...#...#.#####.
..#.....X...###..
..#.#.....#....##

The first nine asteroids to get vaporized, in order, would be:

.#....###24...#..
##...##.13#67..9#
##...#...5.8####.
..#.....X...###..
..#.#.....#....##

Note that some asteroids (the ones behind the asteroids marked 1, 5, and 7) won't have a chance to be vaporized until the next full rotation. The laser continues rotating; the next nine to be vaporized are:

.#....###.....#..
##...##...#.....#
##...#......1234.
..#.....X...5##..
..#.9.....8....76

The next nine to be vaporized are then:

.8....###.....#..
56...9#...#.....#
34...7...........
..2.....X....##..
..1..............

Finally, the laser completes its first full rotation (1 through 3), a second rotation (4 through 8), and vaporizes the last asteroid (9) partway through its third rotation:

......234.....6..
......1...5.....7
.................
........X....89..
.................

In the large example above (the one with the best monitoring station location at 11,13):

- The 1st asteroid to be vaporized is at 11,12.
- The 2nd asteroid to be vaporized is at 12,1.
- The 3rd asteroid to be vaporized is at 12,2.
- The 10th asteroid to be vaporized is at 12,8.
- The 20th asteroid to be vaporized is at 16,0.
- The 50th asteroid to be vaporized is at 16,9.
- The 100th asteroid to be vaporized is at 10,16.
- The 199th asteroid to be vaporized is at 9,6.
- The 200th asteroid to be vaporized is at 8,2.
- The 201st asteroid to be vaporized is at 10,9.
- The 299th and final asteroid to be vaporized is at 11,1.

The Elves are placing bets on which will be the 200th asteroid to be vaporized. Win the bet by determining which asteroid that will be; what do you get if you multiply its X coordinate by 100 and then add its Y coordinate? (For example, 8,2 becomes 802.)
*/

#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <cmath>
#include <utility>
#include <algorithm>
#include <queue>

using namespace std;

// THE LOCATION FROM THE LAST QUESTION WAS x=31, y=20
// We use this as the laser's coordinates
#define LASER_X 31
#define LASER_Y 20

#define PI 3.14159265

// Custom comparator to sort priority queue as a min_heap
// using vector distance
struct CompareDistance { 
  bool operator()(pair<int,int> const& p1, pair<int,int> const& p2) 
    {
      // True is p1 goes before p2
      // However, priority queues output the last element in the
      // queue first, so instead of returning d1 < d2, we return
      // d1 > d2
      double d1 = sqrt(pow(p1.first  - LASER_X, 2) +
		       pow(p1.second - LASER_Y, 2));
      double d2 = sqrt(pow(p2.first  - LASER_X, 2) +
		       pow(p2.second - LASER_Y, 2));
      
      return d1 > d2; 
    } 
}; 

int main()
{
  fstream f;
  string line;
  f.open("asteroids.txt");
  
  // Populate the asteroids map
  set<pair<int, int>> asteroids;
  int y = 0;
  while (getline(f, line))
    {
      for (int x=0; x<line.length(); x++)
	{
	  if (line[x] == '#') asteroids.emplace(x, y);
	}
      y++;
    }

  // Determine what the observatory sees by keeing a min heap for every
  // angle. The first element on the min heap will tell us the closest
  // asteroid. Each sweep of the laser should pop an item off this heap
  // Map: angle -> coordinate
  map<double,
      priority_queue<pair<int,int>,
		     vector<pair<int,int> >,
		     CompareDistance>
      > angles;
  for (auto a: asteroids)
    {
      // Ignore the base's asteroid
      if (a.first == LASER_X && a.second == LASER_Y) continue;
      double ang = atan2(a.second - LASER_Y,
	        	 a.first  - LASER_X);

      // X increases to the right
      // Y increases downwards
      // So the point directly above our asteroid on the ASCII chart is at
      // ang = -PI/2. A clockwise rotation from this pt increases the angle.

      // Change the branch [-pi, pi] -> [-pi/2, 3pi/2]
      // No modification needs to be made to the angle
      double epsilon = 0.000001;
      if (ang + epsilon < -PI/2) ang += 2*PI;

      // The map will automatically store the values from lowest -> biggest
      // angle. So using a branch of [-pi/2, 3pi/2] automatically sorts the
      // asteroid map in the same manner as the laser will sweep!
      angles[ang].emplace(a.first, a.second);
    }

  // Find the 200th asteroid
  int count = 0;
  pair<int, int> loc;
  while(count < 200)
    {
      for(auto ptr = angles.begin(); ptr != angles.end(); ptr++)
	{
	  // Still have things in the priority queue
	  if(ptr->second.empty() == false)
	    {
	      cout << count << ": vaporizing x = " << ptr->second.top().first
		   << " y = " << ptr->second.top().second
		   << " (angle = " << ptr->first << ")" << endl;
	      count++;
	      if (count == 200) loc = ptr->second.top();
	      ptr->second.pop();
	    }
	}
    }
  
  cout << "The 200th asteroid is (x=" << loc.first << ", y=" << loc.second
       << "). x * 100 + y = " << loc.first*100+loc.second << endl;

  return 0;
}
