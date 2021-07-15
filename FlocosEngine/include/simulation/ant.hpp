#ifndef ANT_HPP
#define ANT_HPP

#include <memory>
#include <vector>

namespace SIM {

enum class AntState : u_char { Free, Busy };

struct Pos {
  uint x;
  uint y;
};

struct DeadAnt {
  AntState m_State;
  Pos m_Pos;

  DeadAnt(Pos pos) : m_State{AntState::Free}, m_Pos{pos} {}
  DeadAnt(uint x, uint y) : m_State{AntState::Free}, m_Pos{x, y} {}
  DeadAnt(Pos pos, AntState state) : m_State{state}, m_Pos{pos} {}
  DeadAnt(uint x, uint y, AntState state) : m_State{state}, m_Pos{x, y} {}
  Pos getPos() { return m_Pos; }
  AntState getState() { return m_State; }
  void setState(AntState state) { m_State = state; }
};

class Ant {
private:
  AntState m_State;
  Pos m_Pos;
  std::unique_ptr<DeadAnt> m_Carrying;
  static u_short s_VisionRadius;

public:
  Ant(Pos pos);
  Ant(uint x, uint y);
  Ant(Pos pos, AntState state);
  Ant(uint x, uint y, AntState state);
  Ant(Pos pos, AntState state, std::unique_ptr<DeadAnt> carrying);
  Ant(uint x, uint y, AntState state, std::unique_ptr<DeadAnt> carrying);
  ~Ant();

  std::unique_ptr<DeadAnt> drop();
  void take(std::unique_ptr<DeadAnt> toCarry);
  Pos walk(const std::vector<std::unique_ptr<SIM::Ant>> &ants,
           const u_short &gridM,
           const u_short &gridN);
  uint lookAndCount(const std::vector<std::unique_ptr<DeadAnt>> &deadAnts,
                    const u_short &gridM,
                    const u_short &gridN);

  AntState getState() const;
  Pos getPos() const;
  std::unique_ptr<DeadAnt> transferCarrying();

  static void setRadius(u_short radius);
  static u_short getRadius();
};

}  // namespace SIM

#endif