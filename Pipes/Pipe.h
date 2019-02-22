
#define NUM_CON 4
#define GRID_WIDTH 4
#define GRID_HEIGHT 4
enum eConnectionType {
  North = 0,
  East = 1,
  South = 2,
  West = 3
};

struct Coordinate2D {
  unsigned int x;
  unsigned int y;
};


struct Pipe{

  public:
  Coordinate2D pos;
  bool isSet;
  bool visited = false;
  bool connections[NUM_CON] = {0};

  Pipe(){}

  Pipe(bool north, bool east, bool south, bool west){
    connections[eConnectionType::North] = north;
    connections[eConnectionType::East] = east;
    connections[eConnectionType::South] = south;
    connections[eConnectionType::West] = west;
  }
  
  inline void RotateLeft(){
    for(int i = 0; i < NUM_CON; i++){
      connections[(i-1)%NUM_CON] = connections[i];
    }
  }

  inline void RotateRight(){
    for(int i = NUM_CON-1; i >= 0 ; i--){
      connections[i] = connections[(i-1)%NUM_CON];
    }
  }


  static Pipe PipeT(){
    Pipe p = Pipe(true, true, false, true);
    return p;
  }

  static Pipe PipeCross(){
    Pipe p = Pipe(true, true, true, true);
    return p;
  }

  static Pipe PipeHorizontal(){
    Pipe p = Pipe(false, true, false, true);
    return p;
  }

  static Pipe PipeVertical(){
    Pipe p = Pipe(true, false, true, false);
    return p;
  }

  static Pipe PipeLeftL(){
    Pipe p = Pipe(true, true, false, false);
    return p;
  }
  static Pipe PipeRightL(){
    Pipe p = Pipe(true, false, false, true);
    return p;
  }
  
};


class PipesGameField {
public:
  Coordinate2D start;
  Coordinate2D goal;

  

  Pipe grid [GRID_WIDTH][GRID_HEIGHT];

  Pipe getPipe(int x, int y) {

    return grid[x][y];
  }
  
  PipesGameField(){}

  void setPipe(Pipe pipe, Coordinate2D coord){
    pipe.pos = coord;
    pipe.isSet = true;
    grid[coord.x][coord.y] = pipe;
  }  

  bool traversePipes(){
    return traversePipes(start);
  }

  bool traversePipes(Coordinate2D pos){
    bool reachedGoal = false;
    grid[pos.x][pos.y].visited = true;
    if(grid[pos.x][pos.y].isSet){
      if(pos.x == goal.x && pos.y == goal.y){
        reachedGoal = true;
      }else{
        // traverse north:
        bool north,east,south,west = false;
        if(pos.y -1 >= 0){ // we can go north
          if(grid[pos.x][pos.y-1].isSet){ // there is a pipe
            if(grid[pos.x][pos.y-1].connections[eConnectionType::South]){ // the other pipe has a corresponding connection
              Coordinate2D newPos;
              newPos.x = pos.x;
              newPos.y = pos.y-1;
              north = traversePipes(newPos);
            }
          }
        }

        if(pos.y +1 < GRID_HEIGHT){ // we can go south
          if(grid[pos.x][pos.y+1].isSet){ // there is a pipe
            if(grid[pos.x][pos.y-1].connections[eConnectionType::North]){ // the other pipe has a corresponding connection
              Coordinate2D newPos;
              newPos.x = pos.x;
              newPos.y = pos.y+1;
              south = traversePipes(newPos);
            }
          }
        }

        if(pos.x+1 < GRID_WIDTH){ // we can go east
          if(grid[pos.x+1][pos.y].isSet){ // there is a pipe
            if(grid[pos.x][pos.y-1].connections[eConnectionType::West]){ // the other pipe has a corresponding connection
              Coordinate2D newPos;
              newPos.x = pos.x+1;
              newPos.y = pos.y;
              east = traversePipes(newPos);
            }
          }
        }

        if(pos.x-1 >= 0){ // we can go west
          if(grid[pos.x-1][pos.y].isSet){ // there is a pipe
            if(grid[pos.x][pos.y-1].connections[eConnectionType::East]){ // the other pipe has a corresponding connection
              Coordinate2D newPos;
              newPos.x = pos.x-1;
              newPos.y = pos.y;
              west = traversePipes(newPos);
            }
          }
        }
        reachedGoal = north || east || south || west;
      }
    }
    return reachedGoal;
  }
  
};
