/*
You've landed at the Universal Orbit Map facility on Mercury. Because navigation in space often involves transferring between orbits, the orbit maps here are useful for finding efficient routes between, for example, you and Santa. You download a map of the local orbits (your puzzle input).

Except for the universal Center of Mass (COM), every object in space is in orbit around exactly one other object. An orbit looks roughly like this:

                  \
                   \
                    |
                    |
AAA--> o            o <--BBB
                    |
                    |
                   /
                  /

In this diagram, the object BBB is in orbit around AAA. The path that BBB takes around AAA (drawn with lines) is only partly shown. In the map data, this orbital relationship is written AAA)BBB, which means "BBB is in orbit around AAA".

Before you use your map data to plot a course, you need to make sure it wasn't corrupted during the download. To verify maps, the Universal Orbit Map facility uses orbit count checksums - the total number of direct orbits (like the one shown above) and indirect orbits.

Whenever A orbits B and B orbits C, then A indirectly orbits C. This chain can be any number of objects long: if A orbits B, B orbits C, and C orbits D, then A indirectly orbits D.

For example, suppose you have the following map:

COM)B
B)C
C)D
D)E
E)F
B)G
G)H
D)I
E)J
J)K
K)L

Visually, the above map of orbits looks like this:

        G - H       J - K - L
       /           /
COM - B - C - D - E - F
               \
                I

In this visual representation, when two objects are connected by a line, the one on the right directly orbits the one on the left.

Here, we can count the total number of orbits as follows:

- D directly orbits C and indirectly orbits B and COM, a total of 3 orbits.
- L directly orbits K and indirectly orbits J, E, D, C, B, and COM, a total of 7 orbits.
- COM orbits nothing.

The total number of direct and indirect orbits in this example is 42.

What is the total number of direct and indirect orbits in your map data?
*/
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

typedef struct node {
  vector<node*> children;
} node;

// Run through the tree recursively and return the total number of orbits
// (direct and indirect).
long count_orbits(node* planet, long depth)
{
  // The depth specifies how far we are from the COM, which is equivalent
  // to the number of orbits.
  long out = depth;
  for (node* n : planet->children)
    {
      out += count_orbits(n, depth+1);
    }
  return out;
}

int main()
{
  ifstream f;
  f.open("input.txt");

  string s;
  map<string,node*> m;

  // Construct a tree to represent the orbits
  // Each orbit is around exactly one object, so cycles are impossible
  // due to the existence of a universal Center of Mass that has no
  // orbits (therefore, a cycle cannot exist).
  while (getline(f, s))
    {
      size_t div = s.find(")");
      string s_center = s.substr(0, div);
      string s_orbiter = s.substr(div+1, string::npos);

      // Retrieve the node(s) if they have been seen before, or else
      // make them. We're making a tree that has X children, with the
      // root at "COM"
      node* center;
      node* orbiter;
      if (m.find(s_center) != m.end()) center = m[s_center];
      else { center = new node(); m[s_center] = center; }

      if (m.find(s_orbiter) != m.end()) orbiter = m[s_orbiter];
      else { orbiter = new node(); m[s_orbiter] = orbiter; }

      center->children.push_back(orbiter);
    }

  // Now use the tree to compute the total number of orbits.
  node*  c = m["COM"];
  cout << "The total number of orbits is: " << count_orbits(c,0) << endl;

  // XXX: There is a memory leak for the nodes
  
  return 0;
}
