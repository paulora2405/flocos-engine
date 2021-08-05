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

const int dataSize = 3;
struct Data {
  std::array<float, dataSize - 1> vec;
};

struct DeadAntHeterodata {
  AntState m_State;
  Pos m_Pos;
  Data m_Data;
  uint32_t m_Group = 0;

  DeadAntHeterodata(Pos pos, Data data, uint32_t group)
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

class AntHeterodata {
private:
  AntState m_State;
  Pos m_Pos;
  std::unique_ptr<DeadAntHeterodata> m_Carrying;
  static u_short s_VisionRadius;

public:
  AntHeterodata(Pos pos);
  AntHeterodata(uint32_t x, uint32_t y);
  AntHeterodata(Pos pos, AntState state);
  AntHeterodata(uint32_t x, uint32_t y, AntState state);
  AntHeterodata(Pos pos, AntState state, std::unique_ptr<DeadAntHeterodata> carrying);
  AntHeterodata(uint32_t x,
                uint32_t y,
                AntState state,
                std::unique_ptr<DeadAntHeterodata> carrying);
  /* ~AntHeterodata(); */

  std::unique_ptr<DeadAntHeterodata> drop();
  void take(std::unique_ptr<DeadAntHeterodata> &&toCarry);
  Pos walk(const std::vector<std::unique_ptr<SIM_HETERODATA::AntHeterodata>> &ants,
           const u_short &gridM,
           const u_short &gridN);
  uint32_t lookAndCount(const std::vector<std::unique_ptr<DeadAntHeterodata>> &deadAnts,
                        const u_short &gridM,
                        const u_short &gridN);

  AntState getState() const;
  Pos getPos() const;
  std::unique_ptr<DeadAntHeterodata> transferCarrying();

  static void setRadius(u_short radius);
  static u_short getRadius();
};

}  // namespace SIM_HETERODATA

#endif