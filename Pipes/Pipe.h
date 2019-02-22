
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

  Pipe(eConnectionType conns, ...){
    va_list lArg;
    va_start(lArg, conns);
    while(conns != -1){
      connections[conns] = true;
    }
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
    Pipe p = Pipe(eConnectionType::East, eConnectionType::North, eConnectionType::West);
    return p;
  }

  static Pipe PipeI(){
    Pipe p = Pipe(eConnectionType::East, eConnectionType::West);
    return p;
  }
  
};


class PipesGameField {
public:
  Coordinate2D start;
  Coordinate2D goal;

  

  Pipe grid [GRID_WIDTH][GRID_HEIGHT];

  PipesGameField(){}

  void setPipe(Pipe pipe, Coordinate2D coord){
    pipe.pos = coord;
    pipe.isSet = true;
    grid[coord.x][coord.y] = pipe;
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

