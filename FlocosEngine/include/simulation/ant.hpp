#ifndef ANT_HPP
#define ANT_HPP

#include <memory>
#include <vector>

#include "vendor/glm/glm.hpp"

namespace SIM {

enum class AntState : u_char { Free, Busy };

struct Pos {
  uint x;
  uint y;
};

struct DeadAnt {
  AntState m_State;
  Pos m_Pos;

  DeadAnt(uint x, uint y) : m_State{AntState::Free}, m_Pos{x, y} {}
  Pos getPos() { return m_Pos; }
  AntState getState() { return m_State; }
};

class Ant {
private:
  AntState m_State;
  Pos m_Pos;
  static u_short s_VisionRadius;

public:
  Ant(uint x, uint y);
  Ant(uint x, uint y, AntState);
  ~Ant();

  static void setRadius(u_short radius);

  static u_short getRadius();
  AntState getState() const;
  Pos getPos() const;

  Pos move(const std::vector<std::unique_ptr<SIM::Ant>> &ants,
           const u_short &gridM,
           const u_short &gridN);
  void drop();
  void take();
  uint lookAndCount(const std::vector<std::unique_ptr<DeadAnt>> &deadAnts,
                    const u_short &gridM,
                    const u_short &gridN);
};

}  // namespace SIM

#endif