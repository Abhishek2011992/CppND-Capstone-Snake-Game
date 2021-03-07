#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game
{
public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

protected:
  std::size_t grid_width_;
  std::size_t grid_height_;
  Snake snake;
  SDL_Point food;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  int score{0};

  void PlaceFood();
  void Update();
};

class GameNewVersion : public Game
{
public:
  GameNewVersion(std::size_t grid_width, std::size_t grid_height);
  void PlaceFood();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);

private:
  SDL_Point power_food;
  Uint32 power_food_timer_start;
  Uint32 duration;
  bool is_power_food_placed{false};
  void Update();
  void ClearPowerFood();
  void PlacePowerFood();
};

#endif