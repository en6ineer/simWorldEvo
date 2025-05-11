#pragma once
#include "Organism.h"

class Evo {

private:
  int level; // 0, 1, 2, 3, 4 etc;

public:
  void LevelUp(Organism *Bot);
  void HighView(Organism *Bot);
  void GiveAbility(Organism *Bot);
  int GetLevel(Organism *Bot);
};
