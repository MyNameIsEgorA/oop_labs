#include "./ability_manager.h"
#include <algorithm>
#include <random>

AbilityManager::AbilityManager() {
    abilities_.push_back(std::make_unique<DoubleDamage>());
    abilities_.push_back(std::make_unique<Scanner>());
    abilities_.push_back(std::make_unique<RandomAttack>());

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);

    std::shuffle(abilities_.begin(), abilities_.end(), rng);
}

void AbilityManager::applyAbility(Field& field, int x, int y) {
    if (!abilities_.empty()) {
        abilities_.front()->apply(field, x, y);
        abilities_.erase(abilities_.begin());
    }
}

void AbilityManager::addAbility() {

    using AbilityCreator = std::function<std::unique_ptr<Ability>()>;

    static const std::vector<AbilityCreator> allAbilities = {
        []() { return std::make_unique<DoubleDamage>(); },
        []() { return std::make_unique<Scanner>(); },
        []() { return std::make_unique<RandomAttack>(); }
    };


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, allAbilities.size() - 1);

    int randomIndex = dis(gen);
    abilities_.push_back(allAbilities[randomIndex]());
}