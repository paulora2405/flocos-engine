#ifndef COLONY_HPP
#define COLONY_HPP

#include <memory>
#include <simulation/ant.hpp>
#include <vector>

namespace SIM {

enum class GridState : u_char { Empty, DeadFree, AliveFree, AliveBusy, BothFree };

struct DeadAnt {
  AntState m_State;
  Pos m_Pos;

  DeadAnt(uint x, uint y) : m_State{AntState::Free}, m_Pos{x, y} {}
  Pos getPos() { return m_Pos; }
  AntState getState() { return m_State; }
};

class Colony {
private:
  const u_short m_AliveAntsQnt;
  const u_short m_DeadAntsQnt;
  const u_short m_AntVisionRadius;
  u_short m_GridM;
  u_short m_GridN;
  std::vector<std::unique_ptr<SIM::Ant>> m_AliveAnts;
  std::vector<std::unique_ptr<SIM::DeadAnt>> m_DeadAnts;

public:
  Colony(const u_short &gridM,
         const u_short &gridN,
         const u_short &aliveAnts,
         const u_short &deadAnts,
         const u_short &AntVisionRadius);
  ~Colony();

  void iterate();
  GridState query(uint x, uint y);
};

}  // namespace SIM

#endif