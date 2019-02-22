
#define NUM_CON = 4

enum eConnectionType {
  North = 0,
  East = 1,
  South = 2,
  West = 3
}

struct Coordinate2D {
  unsigned int x;
  unsigned int y;
  
}


struct Pipe{
 unsigned int id = -1;
 bool visited = false;
 bool connections[NUM_CON] = {0};

  public inline RotateLeft(){
    for(int i = 0; i < NUM_CON; i++){
      connections[(i-1)%NUM_CON] = connections[i];
    }
  }


  public inline RotateRight(){
    for(int i = NUM_CON-1; i >= 0 ; i--){
      connections[i] = connections[(i-1)%NUM_CON];
    }
  }
};


public class PipesGameFiled {

  Coordinate2D start;
  Coordinate2D goal;


  Pipe[][] grid;
  unsigned int width;
  unsigned int height;

  public PipesGameFiled(int w, int h):width(w), height(h)
  {
    grid = Pipe[width][height]; 
  }
  
};
