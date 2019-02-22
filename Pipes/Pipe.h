
enum eConnectionType {
  North = 0,
  East = 1,
  South = 2,
  West = 3
}

struct Pipe{
 unsigned int id = -1;
 bool visited = false;
 bool connections[4] = {0};
};
