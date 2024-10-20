// include all maze related variables here.

#define MAZE_SIZE_X 16
#define MAZE_SIZE_Y 16

int startX = 0;
int startY = 0;

int finishX = 7;
int finishY = 7;

struct tile
{
  bool nWall = false;
  bool eWall = false;
  bool sWall = false;
  bool wWall = false;
};



tile* maze[30][30];
