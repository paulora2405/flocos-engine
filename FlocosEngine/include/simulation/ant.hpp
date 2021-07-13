#ifndef ANT_HPP
#define ANT_HPP

#include <memory>
#include <random>

#include "vendor/glm/glm.hpp"

namespace SIM {

enum class AntState : u_char { Free, Busy };

struct Pos {
  uint x;
  uint y;
};

class Ant {
private:
  AntState m_State;
  Pos m_Pos;
  static u_short s_VisionRadius;
  static std::default_random_engine gen;
  static std::uniform_int_distribution<uint> directionChoice;

public:
  Ant(uint x, uint y);
  ~Ant();

  static void setRadius(u_short radius);

  AntState getState() const;

  Pos move(const std::vector<std::unique_ptr<SIM::Ant>> &ants,
           const u_short &gridM,
           const u_short &gridN);
  void drop();
  void take();
  uint lookAndCount();
};

}  // namespace SIM

#endif