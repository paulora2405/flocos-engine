#include "simulation/ant.hpp"

#include "logging/gl_error.hpp"

namespace SIM {

u_short Ant::s_VisionRadius = 0;
std::default_random_engine Ant::gen;
std::uniform_int_distribution<uint> Ant::directionChoice(0, 4);

Ant::Ant(uint x, uint y) : m_State{SIM::AntState::Free}, m_Pos{x, y} {}

Ant::~Ant() {}

AntState Ant::getState() const {
  return m_State;
}

Pos Ant::move(const std::vector<std::unique_ptr<SIM::Ant>> &ants,
              const u_short &gridM,
              const u_short &gridN) {
  uint direction = Ant::directionChoice(Ant::gen);
  Pos newPos = m_Pos;

  if(direction == 0) {
    // LOG(DEBUG) << "STOP";
    return m_Pos;
  }

  if(direction == 1) {  // UP
    // LOG(DEBUG) << "UP";
    newPos.y = (m_Pos.y + 1) % gridN;
  } else if(direction == 2) {  // RIGHT
    // LOG(DEBUG) << "RIGHT";
    newPos.x = (m_Pos.x + 1) % gridM;
  } else if(direction == 3) {  // DOWN
    // LOG(DEBUG) << "DOWN";
    newPos.y = (m_Pos.y > 0) * (m_Pos.y - 1) + (m_Pos.y == 0) * (gridN - 1);
  } else if(direction == 4) {  // LEFT
    // LOG(DEBUG) << "LEFT";
    newPos.x = (m_Pos.x > 0) * (m_Pos.x - 1) + (m_Pos.x == 0) * (gridM - 1);
  }

  if(ants[newPos.x * gridM + newPos.y] != nullptr)
    return m_Pos;

  m_Pos = newPos;
  return m_Pos;
}

void Ant::drop() {}

void Ant::take() {}

uint Ant::lookAndCount() {
  return 0;
}

void Ant::setRadius(u_short radius) {
  Ant::s_VisionRadius = radius;
}

}  // namespace SIM