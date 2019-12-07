/*
Now, you just need to figure out how many orbital transfers you (YOU) need to take to get to Santa (SAN).

You start at the object YOU are orbiting; your destination is the object SAN is orbiting. An orbital transfer lets you move from any object to an object orbiting or orbited by that object.

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
K)YOU
I)SAN

Visually, the above map of orbits looks like this:

                          YOU
                         /
        G - H       J - K - L
       /           /
COM - B - C - D - E - F
               \
                I - SAN

In this example, YOU are in orbit around K, and SAN is in orbit around I. To move from K to I, a minimum of 4 orbital transfers are required:

- K to J
- J to E
- E to D
- D to I

Afterward, the map of orbits looks like this:

        G - H       J - K - L
       /           /
COM - B - C - D - E - F
               \
                I - SAN
                 \
                  YOU
What is the minimum number of orbital transfers required to move from the object YOU are orbiting to the object SAN is orbiting? (Between the objects they are orbiting - not between YOU and SAN.)
 */
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <deque>

using namespace std;

typedef struct node {
  vector<node*> children;
} node;

// DFS to find the distance between start and end and return distance
// NOTE: We find the distance from SAN <-> YOU, but really you want
//       to jump from your orbit to santa's orbit (so we subtract 2,
//       since we don't need to count YOU and SAN).
int dfs (node* start, node* end)
{
  set<node*> seen;
  map<node*, int> dist;
  deque<node*> q;  // This queue will only have non-seen items
  
  q.push_back(start);
  seen.insert(start);
  dist[start] = 0;

  while (!q.empty())
    {
      node* cur = q.front();
      q.pop_front();
      if (cur == end) return dist[cur];

      for (auto c: cur->children)
	{
	  if (seen.find(c) != seen.end()) continue;
	  seen.insert(c);
	  dist[c] = dist[cur] + 1;
	  q.push_back(c);
	}
    }

  // This should never happen since we have a connected graph.
  cout << "Couldn't find Santa!" << endl;
  return 0;
}

int main()
{
  ifstream f;
  f.open("input.txt");

  string s;
  map<string,node*> m;

  // Create an undirected graph to represent the connections between planets
  while (getline(f, s))
    {
      size_t div = s.find(")");
      string s_center = s.substr(0, div);
      string s_orbiter = s.substr(div+1, string::npos);

      // Same logic here as Q1
      node* center;
      node* orbiter;
      if (m.find(s_center) != m.end()) center = m[s_center];
      else { center = new node(); m[s_center] = center; }

      if (m.find(s_orbiter) != m.end()) orbiter = m[s_orbiter];
      else { orbiter = new node(); m[s_orbiter] = orbiter; }

      center->children.push_back(orbiter);
      orbiter->children.push_back(center);
    }

  // Now use the tree to compute the total number of orbits.
  node* start = m["YOU"];
  node* end = m["SAN"];
  cout << "Santa is " << dfs(start, end)-2 << " jumps away!" << endl;

  // XXX: There is a memory leak for the nodes
  
  return 0;
}
