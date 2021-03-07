#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : grid_width_(grid_width),
      grid_height_(grid_height),
      snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height))
{
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running)
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000)
    {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration)
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood()
{
  int x, y;
  while (true)
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y) && (x != grid_width_) && (y != grid_height_))
    {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update()
{
  if (!snake.alive)
    return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y)
  {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

GameNewVersion::GameNewVersion(std::size_t grid_width, std::size_t grid_height)
    : Game(grid_width, grid_height),
      power_food(),
      power_food_timer_start(0)
{
  /*Do nothing*/
}

void GameNewVersion::Update()
{

  Game::Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  /*Check if the score is multiple of 10 and no other power food is placed 
  if placed and the score is up by 1 then then reset the  is_power_food_placed 
  flag so that it can be placed in next time*/
  if (score % 10 == 0 && score != 0 && is_power_food_placed == false)
  {
    PlacePowerFood();
    is_power_food_placed = true;
    power_food_timer_start = SDL_GetTicks();
  }
  else if (score % 10 != 0)
  {
    is_power_food_placed = false;
  }

  /*if the snake head matches the power food then
   double the score and reduce the speed by 2 times the original i.e. 0.4*/
  if (power_food.x == new_x && power_food.y == new_y)
  {
    score = score + 2;
    snake.speed = snake.speed - 2 * 0.02;
    snake.ReduceBody();
    power_food.x = 0;
    power_food.y = 0;
  }
  
  /*Clear power food position it the time lapses*/
  duration = SDL_GetTicks() - power_food_timer_start;
  if (duration > 10000 && power_food_timer_start != 0)
  {
    ClearPowerFood();
    power_food_timer_start = 0;
  }
}

/*function to clear power food position to render*/
void GameNewVersion::ClearPowerFood()
{
  power_food.x = 0;
  power_food.y = 0;
}

/*function to generate power food position to render*/
void GameNewVersion::PlacePowerFood()
{
  int x, y;
  while (true)
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y) && food.x != x && food.y != y && x != 0 && y != 0 && (x != grid_width_ && y != grid_height_))
    {
      power_food.x = x;
      power_food.y = y;
      return;
    }
  }
}

void GameNewVersion::Run(Controller const &controller, Renderer &renderer,
                         std::size_t target_frame_duration)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running)
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, power_food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000)
    {
      if (power_food.x == 0 && power_food.y == 0)
      {
        renderer.UpdateWindowTitle(score, frame_count);
      }
      else
      {
        /*if power food is placed then display the time left for the powerfood*/
        renderer.UpdateWindowTitle(score, frame_count, 10000 - duration);
      }
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration)
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}