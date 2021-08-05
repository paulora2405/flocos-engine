#ifndef COLONY_HPP
#define COLONY_HPP

#include <memory>
#include <vector>

#include "simulation/ant.hpp"

namespace SIM {

enum class GridState : u_char {
  Empty,
  DeadFree,
  AliveFree,
  AliveBusy,
  AliveBusyDeadFree,
  BothFree,
};

class Colony {
private:
  const u_short m_QntAliveAnts;
  const u_short m_QntDeadAnts;
  const u_short m_AntVisionRadius;
  const u_short m_GridM;
  const u_short m_GridN;
  bool m_Finishing;
  bool m_Finished;
  std::vector<std::unique_ptr<SIM::Ant>> m_AliveAnts;
  std::vector<std::unique_ptr<SIM::DeadAnt>> m_DeadAnts;

public:
  Colony(const u_short &gridM,
         const u_short &gridN,
         const u_short &aliveAnts,
         const u_short &deadAnts,
         const u_short &AntVisionRadius);
  /* ~Colony(); */

  inline bool hasFinished() const { return m_Finished; }
  inline void endIterations() { m_Finishing = true; }
  std::vector<bool> getGridState() const;
  void action(std::unique_ptr<Ant> &ant);
  void movement(std::unique_ptr<Ant> &ant, std::vector<std::unique_ptr<Ant>> &newAnts);
  void iterate();
  bool willDrop(const uint32_t &closeDeadAnts);
  bool willTake(const uint32_t &closeDeadAnts);
  GridState query(Pos pos);
  GridState query(uint32_t x, uint32_t y);
};

}  // namespace SIM

#endif