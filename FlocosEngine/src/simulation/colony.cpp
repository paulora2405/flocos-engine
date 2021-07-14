#include "simulation/colony.hpp"

#include <chrono>
#include <csignal>
#include <random>

#include "logging/gl_error.hpp"

namespace SIM {

std::string GridStateToString(GridState s) {
  switch(s) {
    case GridState::Empty:
      return "Empty";
    case GridState::DeadFree:
      return "DeadFree";
    case GridState::AliveFree:
      return "AliveFree";
    case GridState::AliveBusy:
      return "AliveBusy";
    case GridState::AliveBusyDeadFree:
      return "AliveBusyDeadFree";
    case GridState::BothFree:
      return "BothFree";
    default:
      return "Unknow State";
  }
}

bool Colony::willDrop(const uint &closeDeadAnts) {
  static std::default_random_engine gen(
      std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution dist(0.0f, 1.0f);

  // se tiver carregando, chance de dropar é 0
  float density = closeDeadAnts /
                  static_cast<float>((Ant::getRadius() * 2 + 1) * (Ant::getRadius() * 2 + 1) - 1);
  density *= density * density;

  if(dist(gen) < density)
    return true;
  return false;
}

bool Colony::willTake(const uint &closeDeadAnts) {
  static std::default_random_engine gen(
      std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution dist(0.0f, 1.0f);

  float density = closeDeadAnts /
                  static_cast<float>((Ant::getRadius() * 2 + 1) * (Ant::getRadius() * 2 + 1) - 1);
  // density *= density * density;

  if(dist(gen) > density)
    return true;
  return false;
}

void Colony::action() {
  /* State Change: dropping and taking */
  for(auto &ant : m_AliveAnts) {
    /* If an alive ant is in this position */
    if(ant != nullptr) {
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
  }
}

void Colony::movement() {
  /* Movement */
  std::vector<std::unique_ptr<Ant>> newAnts{static_cast<size_t>(m_GridM * m_GridN)};
  std::vector<std::unique_ptr<DeadAnt>> newDeadAnts{static_cast<size_t>(m_GridM * m_GridN)};
  Pos newPos;
  uint i = 0, j = 0;
  for(auto &ant : m_AliveAnts) {
    /* If an alive ant is in this position */
    if(ant != nullptr) {
      newPos = ant->walk(m_AliveAnts, m_GridM, m_GridN);

      /* If there isn's an alive ant already going to new position */
      if(newAnts[newPos.x * m_GridM + newPos.y] == nullptr) {
        newAnts[newPos.x * m_GridM + newPos.y] =
            std::make_unique<Ant>(newPos, ant->getState(), ant->transferCarrying());

        /* If an alive ant already is going to the new position, it stays in the old position */
      } else {
        newAnts[ant->getPos().x * m_GridM + ant->getPos().y] =
            std::make_unique<Ant>(ant->getPos(), ant->getState(), ant->transferCarrying());
      }
    }
    /* If there is a dead ant in this position, it stays there */
    if(m_DeadAnts[i * m_GridM + j] != nullptr)
      newDeadAnts[i * m_GridM + j] = std::make_unique<DeadAnt>(i, j);

    if(++j >= m_GridN)
      j = 0, i++;
  }

  /* Replace old vectors with the new ones */
  m_AliveAnts = std::move(newAnts);
  m_DeadAnts = std::move(newDeadAnts);
}

void Colony::iterate() {
  this->movement();
  this->action();
}

GridState Colony::query(Pos pos) {
  return this->query(pos.x, pos.y);
}

GridState Colony::query(uint x, uint y) {
  if(m_AliveAnts[x * m_GridM + y] == nullptr and m_DeadAnts[x * m_GridM + y] == nullptr)
    return GridState::Empty;

  if(m_AliveAnts[x * m_GridM + y] != nullptr) {
    if(m_DeadAnts[x * m_GridM + y] != nullptr) {
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

  SIM::Ant::setRadius(m_AntVisionRadius);

  std::default_random_engine gen;
  gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
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