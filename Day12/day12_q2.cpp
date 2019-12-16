/*
All this drifting around in space makes you wonder about the nature of the universe. Does history really repeat itself? You're curious whether the moons will ever return to a previous state.

Determine the number of steps that must occur before all of the moons' positions and velocities exactly match a previous point in time.

For example, the first example above takes 2772 steps before they exactly match a previous point in time; it eventually returns to the initial state:

After 0 steps:
pos=<x= -1, y=  0, z=  2>, vel=<x=  0, y=  0, z=  0>
pos=<x=  2, y=-10, z= -7>, vel=<x=  0, y=  0, z=  0>
pos=<x=  4, y= -8, z=  8>, vel=<x=  0, y=  0, z=  0>
pos=<x=  3, y=  5, z= -1>, vel=<x=  0, y=  0, z=  0>

After 2770 steps:
pos=<x=  2, y= -1, z=  1>, vel=<x= -3, y=  2, z=  2>
pos=<x=  3, y= -7, z= -4>, vel=<x=  2, y= -5, z= -6>
pos=<x=  1, y= -7, z=  5>, vel=<x=  0, y= -3, z=  6>
pos=<x=  2, y=  2, z=  0>, vel=<x=  1, y=  6, z= -2>

After 2771 steps:
pos=<x= -1, y=  0, z=  2>, vel=<x= -3, y=  1, z=  1>
pos=<x=  2, y=-10, z= -7>, vel=<x= -1, y= -3, z= -3>
pos=<x=  4, y= -8, z=  8>, vel=<x=  3, y= -1, z=  3>
pos=<x=  3, y=  5, z= -1>, vel=<x=  1, y=  3, z= -1>

After 2772 steps:
pos=<x= -1, y=  0, z=  2>, vel=<x=  0, y=  0, z=  0>
pos=<x=  2, y=-10, z= -7>, vel=<x=  0, y=  0, z=  0>
pos=<x=  4, y= -8, z=  8>, vel=<x=  0, y=  0, z=  0>
pos=<x=  3, y=  5, z= -1>, vel=<x=  0, y=  0, z=  0>

Of course, the universe might last for a very long time before repeating. Here's a copy of the second example from above:

<x=-8, y=-10, z=0>
<x=5, y=5, z=10>
<x=2, y=-7, z=3>
<x=9, y=-8, z=-3>

This set of initial positions takes 4686774924 steps before it repeats a previous state! Clearly, you might need to find a more efficient way to simulate the universe.

How many steps does it take to reach the first state that exactly matches a previous state?
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <stdlib.h>
#include <stdio.h>
#include <numeric>

#define STEPS 1000

struct planet
{
  int x;
  int y;
  int z;
  int vx;
  int vy;
  int vz;
};

int main()
{
  std::vector<planet> planets;
  std::string str;
  std::fstream f;
  f.open("input.txt");

  while (std::getline(f, str))
    {
      // Parse <x=-4, y=-14, z=8> by finding substrings between commas
      // and equals signs.
      planet p;
      int equals, comma;
      p.vx = 0; p.vy = 0; p.vz = 0;

      equals = str.find('=');
      comma = str.find(',');
      p.x = std::stoi(str.substr(equals+1, comma-equals-1));
      
      equals = str.find('=', equals+1);
      comma = str.find(',', comma+1);
      p.y = std::stoi(str.substr(equals+1, comma-equals-1));

      equals = str.find('=', equals+1);
      comma = str.find(',', comma+1);
      p.z = std::stoi(str.substr(equals+1, comma-equals-1));
      
      planets.push_back(p);
    }

  // Run the simulation
  long long steps = 0;
  // A set of vectors (length 4) which contain pairs of (pos, vel) for each
  // planet
  std::set<std::vector<std::pair<int, int> > > sx, sy, sz;
  long long px = 0, py = 0, pz = 0; // Period
  while(true)
    {
      // Check if we have found all periods
      if (px != 0 && py != 0 && pz != 0)
	break;
      
      std::vector<std::pair<int, int> > vecx, vecy, vecz;
      for (auto p: planets)
	{
	  vecx.emplace_back(p.x, p.vx);
	  vecy.emplace_back(p.y, p.vy);
	  vecz.emplace_back(p.z, p.vz);
	}

      // Find periodic motion
      if (px == 0 && sx.find(vecx) != sx.end()) px = steps;
      else if (px == 0) sx.insert(vecx);
      
      if (py == 0 && sy.find(vecy) != sy.end()) py = steps;
      else if (py == 0) sy.insert(vecy);
      
      if (pz == 0 && sz.find(vecz) != sz.end()) pz = steps;
      else if (pz == 0) sz.insert(vecz);
      
      steps++;
      
      // Update gravity. This requires creating a new vector of planets
      // since all planet updates happen based on previous state
      // information, and so it may not be mutated.
      std::vector<planet> update;
      for (auto p1: planets)
	{
	  planet p = p1;
	  
	  for (auto p2: planets)
	    {
	      // If p1 == p2 then nothing happens     
	      if (p1.x < p2.x) p.vx += 1;
	      if (p1.x > p2.x) p.vx -= 1;
	      if (p1.y < p2.y) p.vy += 1;
	      if (p1.y > p2.y) p.vy -= 1;
	      if (p1.z < p2.z) p.vz += 1;
	      if (p1.z > p2.z) p.vz -= 1;
	    }
	  update.push_back(p);
	}
      
      // Now we may update the planets and apply velocity
      planets = update;
      for (int i = 0; i < planets.size(); i++)
	{
	  planet* p = &planets[i];
	  p->x += p->vx;
	  p->y += p->vy;
	  p->z += p->vz;	  
	}
    }

  // Now we have periods for all the individual axes. We may find their
  // period motion using a LCM
  long long p = std::lcm(px, std::lcm(py, pz));

  printf("px: %lld, py: %lld, pz: %lld\n", px, py,pz);
  
  std::cout << "It takes " << p << " to return to a known state"
	    << std::endl;
}
