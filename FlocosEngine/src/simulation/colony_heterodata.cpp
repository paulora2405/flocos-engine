#include "simulation/colony_heterodata.hpp"

#include <chrono>
#include <random>

#include "logging/gl_error.hpp"

namespace SIM_HETERODATA {

// std::string GridStateToString(GridState s) {
//   switch(s) {
//     case GridState::Empty:
//       return "Empty";
//     case GridState::DeadFree:
//       return "DeadFree";
//     case GridState::AliveFree:
//       return "AliveFree";
//     case GridState::AliveBusy:
//       return "AliveBusy";
//     case GridState::AliveBusyDeadFree:
//       return "AliveBusyDeadFree";
//     case GridState::BothFree:
//       return "BothFree";
//     default:
//       return "Unknow State";
//   }
// }

bool Colony::willDrop(const uint32_t &closeDeadAnts) {
  static std::default_random_engine gen(
      std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution dist(0.0f, 1.0f);

  const float radius = static_cast<float>(Ant::getRadius());
  float density = closeDeadAnts / ((radius * 2 + 1) * (radius * 2 + 1) - 1);
  density *= density * density;

  if(dist(gen) < density)
    return true;
  return false;
}

bool Colony::willTake(const uint32_t &closeDeadAnts) {
  static std::default_random_engine gen(
      std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution dist(0.0f, 1.0f);

  const float radius = static_cast<float>(Ant::getRadius());
  float density = closeDeadAnts / ((radius * 2 + 1) * (radius * 2 + 1) - 1);

  if(dist(gen) > density)
    return true;
  return false;
}

void Colony::action(std::unique_ptr<Ant> &ant) {
  /* State Change: dropping and taking */
  /* If there is a free alive ant and a free dead ant */
  if(this->query(ant->getPos()) == GridState::BothFree) {
    /* If the alive ants decides to take dead ant */
    if(willTake(ant->lookAndCount(m_DeadAnts, m_GridM, m_GridN)))
      ant->take(std::move(m_DeadAnts[ant->getPos().x * m_GridM + ant->getPos().y]));

    /* If there is an alive ant carrying a dead ant */
  } else if(this->query(ant->getPos()) == GridState::AliveBusy) {
    /* If the alive ant decides to drop the dead ant */
    if(willDrop(ant->lookAndCount(m_DeadAnts, m_GridM, m_GridN)))
      m_DeadAnts.at(ant->getPos().x * m_GridM + ant->getPos().y) = ant->drop();

    /* If there is an AliveAnt busy and a DeadAnt free */
  } else if(this->query(ant->getPos()) == GridState::AliveBusyDeadFree) {
    /* Do nothing  */
  }
}

void Colony::movement(std::unique_ptr<Ant> &ant, std::vector<std::unique_ptr<Ant>> &newAnts) {
  /* Movement */
  Pos newPos = ant->walk(m_AliveAnts, m_GridM, m_GridN);

  /* If there isn's an alive ant already going to new position */
  if(!newAnts[newPos.x * m_GridM + newPos.y]) {
    newAnts[newPos.x * m_GridM + newPos.y] =  // move from old to new instead of creating a new one
        std::make_unique<Ant>(newPos, ant->getState(), ant->transferCarrying());

    /* If an alive ant already is going to the new position, it stays in the old position */
  } else {
    newAnts[ant->getPos().x * m_GridM + ant->getPos().y] =
        std::make_unique<Ant>(ant->getPos(), ant->getState(), ant->transferCarrying());
  }
}

void Colony::iterate() {
  std::vector<std::unique_ptr<Ant>> newAnts{static_cast<std::size_t>(m_GridM * m_GridN)};
  std::vector<std::unique_ptr<DeadAnt>> newDeadAnts{static_cast<std::size_t>(m_GridM * m_GridN)};
  uint32_t i = 0, j = 0;
  bool foundOne = false;
  for(auto &ant : m_AliveAnts) {
    /* If an alive ant is in this position */
    if(ant) {
      foundOne = true;
      if(m_Finishing and ant->getState() == AntState::Free)
        ant = nullptr;
      else {
        this->action(ant);
        this->movement(ant, newAnts);
      }
    }

    /* If there is a dead ant in this position, it stays there */
    if(m_DeadAnts[i * m_GridM + j]) {
      Data tempD = m_DeadAnts[i * m_GridM + j]->getData();
      uint32_t tempG = m_DeadAnts[i * m_GridM + j]->getGroup();
      newDeadAnts[i * m_GridM + j] = std::make_unique<DeadAnt>(Pos{i, j}, tempD, tempG);
    }

    if(++j >= m_GridN)
      j = 0, ++i;
  }
  if(!foundOne)
    m_Finished = true;
  /* Replace old vectors with the new ones */
  m_AliveAnts = std::move(newAnts);
  m_DeadAnts = std::move(newDeadAnts);
}

std::vector<bool> Colony::getGridState() const {
  std::vector<bool> ret(static_cast<std::size_t>(m_GridM * m_GridN), false);

  uint32_t i = 0, j = 0;
  for(const auto &deadAnt : m_DeadAnts) {
    if(deadAnt)
      ret[i * m_GridM + j] = true;
    if(++j >= m_GridN)
      j = 0, ++i;
  }

  return ret;
}

uint8_t Colony::getGroupAt(uint32_t x, uint32_t y) {
  if(m_DeadAnts[x * m_GridM + y])
    return static_cast<uint8_t>(m_DeadAnts[x * m_GridM + y]->getGroup());
  return static_cast<uint8_t>(m_AliveAnts[x * m_GridM + y]->getCarryingGroup());
}

GridState Colony::query(Pos pos) {
  return this->query(pos.x, pos.y);
}

GridState Colony::query(uint32_t x, uint32_t y) {
  if(!m_AliveAnts[x * m_GridM + y] and !m_DeadAnts[x * m_GridM + y])
    return GridState::Empty;

  if(m_AliveAnts[x * m_GridM + y]) {
    if(m_DeadAnts[x * m_GridM + y]) {
      if(m_AliveAnts[x * m_GridM + y]->getState() == AntState::Busy)
        return GridState::AliveBusyDeadFree;
      return GridState::BothFree;

    } else if(m_AliveAnts[x * m_GridM + y]->getState() == AntState::Busy)
      return GridState::AliveBusy;
    return GridState::AliveFree;
  }
  return GridState::DeadFree;
}

Colony::Colony(const u_short &gridM,
               const u_short &gridN,
               const u_short &aliveAnts,
               const u_short &deadAnts,
               const u_short &AntVisionRadius,
               std::vector<std::vector<float>> dataWithGroup)
    : m_QntAliveAnts{aliveAnts},
      m_QntDeadAnts{dataWithGroup.empty() ? deadAnts : static_cast<uint16_t>(dataWithGroup.size())},
      m_AntVisionRadius{AntVisionRadius},
      m_GridM{gridM},
      m_GridN{gridN},
      m_Finishing{false},
      m_Finished{false},
      m_AliveAnts{static_cast<std::size_t>(m_GridM * m_GridN)},
      m_DeadAnts{static_cast<std::size_t>(m_GridM * m_GridN)} {
  LOG(DEBUG) << "Colony Constructed (" << gridM * gridN << " cells)";

  SIM_HETERODATA::Ant::setRadius(m_AntVisionRadius);

  std::default_random_engine gen;
  gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<uint32_t> distM(0, gridM - 1);
  std::uniform_int_distribution<uint32_t> distN(0, gridN - 1);

  for(uint32_t k = 0; k < m_QntAliveAnts; k++) {
    uint32_t i, j;
    do {
      i = distM(gen), j = distN(gen);
    } while(m_AliveAnts[i * m_GridM + j]);
    m_AliveAnts[i * m_GridM + j] = std::make_unique<SIM_HETERODATA::Ant>(i, j);
  }

  if(dataWithGroup.empty()) {
    for(uint32_t k = 0; k < m_QntDeadAnts; k++) {
      uint32_t i, j;
      do {
        i = distM(gen), j = distN(gen);
      } while(m_DeadAnts[i * m_GridM + j]);
      m_DeadAnts[i * m_GridM + j] =
          std::make_unique<SIM_HETERODATA::DeadAnt>(Pos{i, j}, Data{{0.0f, 0.0f}}, 0);
    }

  } else {
    for(uint32_t k = 0; k < m_QntDeadAnts; k++) {
      uint32_t i, j;
      do {
        i = distM(gen), j = distN(gen);
      } while(m_DeadAnts[i * m_GridM + j]);
      m_DeadAnts[i * m_GridM + j] = std::make_unique<SIM_HETERODATA::DeadAnt>(
          Pos{i, j}, Data{{dataWithGroup.at(k).at(0), dataWithGroup.at(k).at(1)}},
          static_cast<uint32_t>(dataWithGroup.at(k).at(2)));
    }
  }
}

}  // namespace SIM_HETERODATA