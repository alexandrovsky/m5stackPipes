
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
    grid[coord.x][coord.y] = pipe;
  }  

    
  
};

