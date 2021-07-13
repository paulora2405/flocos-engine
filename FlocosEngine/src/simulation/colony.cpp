#include "simulation/colony.hpp"

#include <random>

#include "logging/gl_error.hpp"

namespace SIM {

void Colony::iterate() {
  Pos newPos;
  std::vector<std::unique_ptr<Ant>> newAnts{(size_t)(m_GridM * m_GridN)};

  for(auto &ant : m_AliveAnts) {
    if(ant != nullptr) {
      newPos = ant.get()->move(m_AliveAnts, m_GridM, m_GridN);

      if(newAnts[newPos.x * m_GridM + newPos.y] == nullptr)
        newAnts[newPos.x * m_GridM + newPos.y] =
            std::make_unique<Ant>(newPos.x, newPos.y, ant.get()->getState());
      else
        newAnts[ant.get()->getPos().x * m_GridM + ant.get()->getPos().y] = std::make_unique<Ant>(
            ant.get()->getPos().x, ant.get()->getPos().y, ant.get()->getState());
    }
  }
  m_AliveAnts = std::move(newAnts);
}

GridState Colony::query(uint x, uint y) {
  if(m_AliveAnts[x * m_GridM + y] == nullptr and m_DeadAnts[x * m_GridM + y] == nullptr)
    return GridState::Empty;

  if(m_AliveAnts[x * m_GridM + y] != nullptr) {
    if(m_DeadAnts[x * m_GridM + y] != nullptr) {
      if(m_AliveAnts[x * m_GridM + y].get()->getState() == AntState::Busy)
        return GridState::AliveBusy;
      return GridState::BothFree;
    }
    return GridState::AliveFree;
  }
  return GridState::DeadFree;
}

Colony::Colony(const u_short &gridM,
               const u_short &gridN,
               const u_short &aliveAnts,
               const u_short &deadAnts,
               const u_short &AntVisionRadius)
    : m_AliveAntsQnt{aliveAnts},
      m_DeadAntsQnt{deadAnts},
      m_AntVisionRadius{AntVisionRadius},
      m_GridM{gridM},
      m_GridN{gridN},
      m_AliveAnts{(size_t)(m_GridM * m_GridN)},
      m_DeadAnts{(size_t)(m_GridM * m_GridN)} {
  //

  /* TODO: Checar se quantidade de formiga eh maior que total de celulas */

  LOG(DEBUG) << "Colony Constructed (" << gridM * gridN << " cells)";
  // LOG(DEBUG) << "m_AliveAnts.size() = " << m_AliveAnts.size();
  // LOG(DEBUG) << "m_DeadAnts.size() = " << m_DeadAnts.size();

  SIM::Ant::setRadius(m_AntVisionRadius);

  std::default_random_engine gen;
  std::uniform_int_distribution<uint> distM(0, gridM - 1);
  std::uniform_int_distribution<uint> distN(0, gridN - 1);

  for(size_t k = 0; k < m_AliveAntsQnt; k++) {
    uint i, j;
    do {
      i = distM(gen), j = distN(gen);
    } while(m_AliveAnts[i * m_GridM + j] != nullptr);
    m_AliveAnts[i * m_GridM + j] = std::make_unique<SIM::Ant>(i, j);
  }

  for(size_t k = 0; k < m_DeadAntsQnt; k++) {
    uint i, j;
    do {
      i = distM(gen), j = distN(gen);
    } while(m_DeadAnts[i * m_GridM + j] != nullptr);
    m_DeadAnts[i * m_GridM + j] = std::make_unique<SIM::DeadAnt>(i, j);
  }
}

Colony::~Colony() {}

}  // namespace SIM