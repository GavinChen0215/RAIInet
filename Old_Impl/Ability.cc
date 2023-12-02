#include "Ability.h"

bool Ability::getState() const { return state; }

void Ability::toggleState() { state = false; }

Ability::~Ability() {}