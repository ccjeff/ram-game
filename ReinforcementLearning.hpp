#include "Items.hpp"

class ReinforcementLearning : public Items {
public:
  ReinforcementLearning(std::shared_ptr<Player> player);
  
  void on_kill() override;
};
