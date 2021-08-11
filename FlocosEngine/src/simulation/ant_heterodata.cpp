#include "simulation/ant_heterodata.hpp"

#include <random>

#include "logging/gl_error.hpp"

namespace SIM_HETERODATA {

u_short Ant::s_VisionRadius = 0;

Pos Ant::walk(const std::vector<std::unique_ptr<SIM_HETERODATA::Ant>> &ants,
              const u_short &gridM,
              const u_short &gridN) {
  static std::default_random_engine gen;
  static std::uniform_int_distribution<uint> directionChoice(1, 8);
  const uint direction = directionChoice(gen);
  Pos newPos = m_Pos;

  if(direction == 1)  // UP
    newPos.y = (m_Pos.y + 1) % gridN;

  else if(direction == 2)  // RIGHT
    newPos.x = (m_Pos.x + 1) % gridM;

  else if(direction == 3)  // DOWN
    newPos.y = (m_Pos.y > 0) * (m_Pos.y - 1) + (m_Pos.y == 0) * (gridN - 1);

  else if(direction == 4)  // LEFT
    newPos.x = (m_Pos.x > 0) * (m_Pos.x - 1) + (m_Pos.x == 0) * (gridM - 1);

  else if(direction == 5)  // UP RIGHT
    newPos = {(m_Pos.x + 1) % gridM, (m_Pos.y + 1) % gridN};

  else if(direction == 6)  // UP LEFT
    newPos = {(m_Pos.x > 0) * (m_Pos.x - 1) + (m_Pos.x == 0) * (gridM - 1), (m_Pos.y + 1) % gridN};

  else if(direction == 7)  // DOWN RIGHT
    newPos = {(m_Pos.x + 1) % gridM, (m_Pos.y > 0) * (m_Pos.y - 1) + (m_Pos.y == 0) * (gridN - 1)};

  else if(direction == 8)  // DOWN LEFT
    newPos = {(m_Pos.x > 0) * (m_Pos.x - 1) + (m_Pos.x == 0) * (gridM - 1),
              (m_Pos.y > 0) * (m_Pos.y - 1) + (m_Pos.y == 0) * (gridN - 1)};

  if(ants[newPos.x * gridM + newPos.y])
    return m_Pos;

  return newPos;
}

std::unique_ptr<DeadAnt> Ant::drop() {
  m_State = AntState::Free;
  m_Carrying->setState(AntState::Free);
  return std::move(m_Carrying);
}

void Ant::take(std::unique_ptr<DeadAnt> &&toCarry) {
  m_State = AntState::Busy;
  toCarry->setState(AntState::Busy);
  m_Carrying = std::move(toCarry);
}

uint Ant::lookAndCount(const std::vector<std::unique_ptr<DeadAnt>> &deadAnts,
                       const u_short &gridM,
                       const u_short &gridN) {
  uint x = m_Pos.x;
  uint y = m_Pos.y;
  uint closeDeadAnt = 0;
  for(u_char i = 0; i <= s_VisionRadius; i++) {
    for(u_char j = 0; j <= s_VisionRadius; j++) {
      if(i == 0 and j == 0)
        continue;

      /*
      (1 - (0) / 2) +
      (1 - (1) / 2) +
      (1 - (1) / 2) +
      (4 - (1) / 2) +
      (5 - (1) / 2) +
      (1 - (1) / 2) +
      s=6
      sum=10.5
      fi= (1/6**2) * 10.5
       */

      if(i == 0) {
        if((y >= j) and deadAnts[(x)*gridM + (y - j)])
          closeDeadAnt++;
        if((y + j) < gridN and deadAnts[(x)*gridM + (y + j)])
          closeDeadAnt++;

      } else if(j == 0) {
        if((x + i) < gridM and deadAnts[(x + i) * gridM + (y)])
          closeDeadAnt++;
        if((x >= i) and deadAnts[(x - i) * gridM + (y)])
          closeDeadAnt++;

      } else {
        if((x + i) < gridM and (y + j) < gridN and deadAnts[(x + i) * gridM + (y + j)])
          closeDeadAnt++;
        if((x + i) < gridM and (y >= j) and deadAnts[(x + i) * gridM + (y - j)])
          closeDeadAnt++;
        if((x >= i) and (y >= j) and deadAnts[(x - i) * gridM + (y - j)])
          closeDeadAnt++;
        if((x >= i) and (y + j) < gridN and deadAnts[(x - i) * gridM + (y + j)])
          closeDeadAnt++;
      }
    }
  }

  return closeDeadAnt;
}

std::unique_ptr<DeadAnt> Ant::transferCarrying() {
  return std::move(m_Carrying);
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

Ant::Ant(Pos pos) : m_State{SIM_HETERODATA::AntState::Free}, m_Pos{pos}, m_Carrying{} {}
Ant::Ant(uint x, uint y) : m_State{SIM_HETERODATA::AntState::Free}, m_Pos{x, y}, m_Carrying{} {}
Ant::Ant(Pos pos, AntState state) : m_State{state}, m_Pos{pos}, m_Carrying{} {}
Ant::Ant(uint x, uint y, AntState state) : m_State{state}, m_Pos{x, y}, m_Carrying{} {}
Ant::Ant(Pos pos, AntState state, std::unique_ptr<DeadAnt> carrying)
    : m_State{state},
      m_Pos{pos},
      m_Carrying{std::move(carrying)} {}
Ant::Ant(uint x, uint y, AntState state, std::unique_ptr<DeadAnt> carrying)
    : m_State{state},
      m_Pos{x, y},
      m_Carrying{std::move(carrying)} {}

}  // namespace SIM_HETERODATA