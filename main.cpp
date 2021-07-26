#include <SFML/Graphics.hpp>
#define TILE_SIZE 18

const int M = 20;
const int N = 10;

int board[M][N] = {0};

enum Direction
{
  Stop = 0,
  Right = 1,
  Left = -1
};
struct Point
{
  int x, y;
} position[4], prevPosition[4];

/**
 * Pieces coordinates
 * |  0|  1|
 * |  2|  3|
 * |  4|  5|
 * |  6|  7|
*/

int pieces[7][4] = {
    1, 3, 5, 7, // I
    2, 4, 5, 7, // Z
    3, 5, 4, 6, // S
    3, 5, 4, 7, // T
    2, 3, 5, 7, // L
    3, 5, 7, 6, // J
    2, 3, 4, 5, // O
};

bool check()
{
  for (size_t i = 0; i < 4; i++)
  {
    if (position[i].x < 0 || position[i].x >= N || position[i].y >= M) // is insied the filed
      return false;
    else if (board[position[i].y][position[i].x]) // is not on the same spot as another piece
      return false;
  }

  return true;
}

void assembleNewPiece()
{
  int pieceIndex = rand() % 7;
  // Assemble next piece!
  for (size_t i = 0; i < 4; i++) // number of tile per piece
  {
    position[i].x = pieces[pieceIndex][i] % 2; // Col number
    position[i].y = pieces[pieceIndex][i] / 2; // Row number
  }
}

int main()
{
  using namespace sf;
  srand(time(NULL));

  RenderWindow window(VideoMode(320, 480), "Tetris");
  Texture t;
  t.loadFromFile("images/tiles.png");

  Sprite s(t);
  s.setTextureRect(IntRect(0, 0, TILE_SIZE, TILE_SIZE));

  int direction = Direction::Stop;
  bool rotate = false;
  int colorNum = 1;
  float timer = 0, delay = 0.3;

  Clock clock;

  assembleNewPiece(); // Starter Piece
  while (window.isOpen())
  {
    float time = clock.getElapsedTime().asSeconds();
    clock.restart();
    timer += time;

    Event event;
    while (window.pollEvent(event))
    {
      if (event.type == Event::Closed)
      {
        window.close();
      }

      if (event.type == Event::KeyPressed)
      {
        if (event.key.code == Keyboard::Up)
          rotate = true;
        else if (event.key.code == Keyboard::Left)
          direction = Direction::Left;
        else if (event.key.code == Keyboard::Right)
          direction = Direction::Right;
      }
    }

    // Logic
    if (Keyboard::isKeyPressed(Keyboard::Down))
      delay = 0.05;

    // Move piece
    for (size_t i = 0; i < 4; i++)
    {
      prevPosition[i] = position[i];
      position[i].x += direction;
    }
    if (!check())
    {
      // return position
      for (size_t i = 0; i < 4; i++)
      {
        position[i] = prevPosition[i];
      }
    }

    // Rotate Piece
    if (rotate)
    {
      Point p = position[1]; // center of rotation
      for (size_t i = 0; i < 4; i++)
      {
        int x = position[i].y - p.y;
        int y = position[i].x - p.x;

        position[i].x = p.x - x;
        position[i].y = p.y + y;
      }
      if (!check())
      {
        // return position
        for (size_t i = 0; i < 4; i++)
        {
          position[i] = prevPosition[i];
        }
      }
    }

    // Tick - Gravity
    if (timer > delay)
    {
      for (size_t i = 0; i < 4; i++)
      {
        prevPosition[i] = position[i];
        position[i].y += 1;
      }

      if (!check())
      {
        // return position
        for (size_t i = 0; i < 4; i++)
        {
          board[prevPosition[i].y][prevPosition[i].x] = colorNum;
        }
        colorNum = 1 + rand() % 7;

        assembleNewPiece();
      }

      timer = 0;
    }

    // Check lines
    int k = M - 1;
    for (int i = M - 1; i > 0; i--)
    {
      int count = 0;
      for (int j = 0; j < N; j++)
      {
        if (board[i][j])
          count++;
        board[k][j] = board[i][j];
      }
      if (count < N)
        k--;
    }

    direction = Direction::Stop;
    rotate = false;
    delay = 0.3;

    window.clear(Color::White);

    // Drawing Board
    for (size_t i = 0; i < M; i++)
    {
      for (size_t j = 0; j < N; j++)
      {
        if (board[i][j] == 0)
          continue;

        s.setTextureRect(IntRect(board[i][j] * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
        s.setPosition(j * TILE_SIZE, i * TILE_SIZE);
        window.draw(s);
      }
    }

    // Moving piece
    for (size_t i = 0; i < M; i++)
    {
      s.setTextureRect(IntRect(colorNum * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
      s.setPosition(position[i].x * TILE_SIZE, position[i].y * TILE_SIZE);
      window.draw(s);
    }

    window.display();
  }

  return 0;
}