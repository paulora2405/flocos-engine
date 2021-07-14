#include "simulation/ant.hpp"

#include <random>

#include "logging/gl_error.hpp"

namespace SIM {

u_short Ant::s_VisionRadius = 0;

Ant::Ant(uint x, uint y) : m_State{SIM::AntState::Free}, m_Pos{x, y} {}

Ant::Ant(uint x, uint y, AntState s) : m_State{s}, m_Pos{x, y} {}

Ant::~Ant() {}

Pos Ant::move(const std::vector<std::unique_ptr<SIM::Ant>> &ants,
              const u_short &gridM,
              const u_short &gridN) {
  static std::default_random_engine gen;
  static std::uniform_int_distribution<uint> directionChoice(1, 4);
  uint direction = directionChoice(gen);
  Pos newPos = m_Pos;

  if(direction == 0)
    return m_Pos;

  // TODO: Diagonal Movement
  if(direction == 1)  // UP
    newPos.y = (m_Pos.y + 1) % gridN;
  else if(direction == 2)  // RIGHT
    newPos.x = (m_Pos.x + 1) % gridM;
  else if(direction == 3)  // DOWN
    newPos.y = (m_Pos.y > 0) * (m_Pos.y - 1) + (m_Pos.y == 0) * (gridN - 1);
  else if(direction == 4)  // LEFT
    newPos.x = (m_Pos.x > 0) * (m_Pos.x - 1) + (m_Pos.x == 0) * (gridM - 1);

  if(ants[newPos.x * gridM + newPos.y] != nullptr)
    return m_Pos;

  return newPos;
}

void Ant::drop() {
  m_State = AntState::Free;
}

void Ant::take() {
  m_State = AntState::Busy;
}

uint Ant::lookAndCount(const std::vector<std::unique_ptr<DeadAnt>> &deadAnts,
                       const u_short &gridM,
                       const u_short &gridN) {
  uint x = m_Pos.x;
  uint y = m_Pos.y;
  uint closeDeadAnt = 0;
  for(size_t i = 0; i <= s_VisionRadius; i++) {
    for(size_t j = 0; j <= s_VisionRadius; j++) {
      if(i == 0 and j == 0)
        continue;

      if(i == 0) {
        if((x + i) < gridM and (y >= j) and deadAnts[(x + i) * gridM + (y - j)] != nullptr)
          closeDeadAnt++;
        if((x >= i) and (y + j) < gridN and deadAnts[(x - i) * gridM + (y + j)] != nullptr)
          closeDeadAnt++;

      } else if(j == 0) {
        if((x + i) < gridM and (y + j) < gridN and deadAnts[(x + i) * gridM + (y + j)] != nullptr)
          closeDeadAnt++;
        if((x >= i) and (y >= j) and deadAnts[(x - i) * gridM + (y - j)] != nullptr)
          closeDeadAnt++;

      } else {
        if((x + i) < gridM and (y + j) < gridN and deadAnts[(x + i) * gridM + (y + j)] != nullptr)
          closeDeadAnt++;

        if((x + i) < gridM and (y >= j) and deadAnts[(x + i) * gridM + (y - j)] != nullptr)
          closeDeadAnt++;

        if((x >= i) and (y >= j) and deadAnts[(x - i) * gridM + (y - j)] != nullptr)
          closeDeadAnt++;

        if((x >= i) and (y + j) < gridN and deadAnts[(x - i) * gridM + (y + j)] != nullptr)
          closeDeadAnt++;
      }
    }
  }

  return closeDeadAnt;
}

AntState Ant::getState() const {
  return m_State;
}

Pos Ant::getPos() const {
  return m_Pos;
}

u_short Ant::getRadius() {
  return Ant::s_VisionRadius;
}

void Ant::setRadius(u_short radius) {
  Ant::s_VisionRadius = radius;
}

}  // namespace SIM