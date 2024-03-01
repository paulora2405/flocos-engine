#ifndef ANT_HETERODATA_HPP
#define ANT_HETERODATA_HPP

#include <memory>
#include <vector>

typedef uint8_t u_char;
typedef uint32_t uint32_t;
typedef uint16_t u_short;

namespace SIM_HETERODATA {

enum class AntState : u_char { Free, Busy };

struct Pos {
  uint32_t x, y;
};

struct Data {
  std::vector<float> vec;
};

struct DeadAnt {
  AntState m_State;
  Pos m_Pos;
  Data m_Data;
  uint32_t m_Group = 0;

  DeadAnt(Pos pos, Data data, uint32_t group)
      : m_State{AntState::Free},
        m_Pos{pos},
        m_Data{data},
        m_Group{group} {}
  AntState getState() const { return m_State; }
  Pos getPos() const { return m_Pos; }
  Data getData() const { return m_Data; }
  uint32_t getGroup() const { return m_Group; }
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
  Ant(uint32_t x, uint32_t y);
  Ant(Pos pos, AntState state);
  Ant(uint32_t x, uint32_t y, AntState state);
  Ant(Pos pos, AntState state, std::unique_ptr<DeadAnt> carrying);
  Ant(uint32_t x, uint32_t y, AntState state, std::unique_ptr<DeadAnt> carrying);
  /* ~Ant(); */

  std::unique_ptr<DeadAnt> drop();
  void take(std::unique_ptr<DeadAnt> &&toCarry);
  Pos walk(const std::vector<std::unique_ptr<SIM_HETERODATA::Ant>> &ants,
           const u_short &gridM,
           const u_short &gridN);
  uint32_t lookAndCount(const std::vector<std::unique_ptr<DeadAnt>> &deadAnts,
                        const u_short &gridM,
                        const u_short &gridN);

  AntState getState() const;
  Pos getPos() const;
  uint8_t getCarryingGroup() const;
  std::unique_ptr<DeadAnt> transferCarrying();

  static void setRadius(u_short radius);
  static u_short getRadius();
};

}  // namespace SIM_HETERODATA

#endif