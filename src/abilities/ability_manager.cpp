#include "./ability_manager.h"
#include <algorithm>
#include <random>

#include "exceptions.h"

AbilityManager::AbilityManager() {
    abilities_.push_back(std::make_unique<DoubleDamage>());
    abilities_.push_back(std::make_unique<Scanner>());
    abilities_.push_back(std::make_unique<RandomAttack>());

    const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);

    std::shuffle(abilities_.begin(), abilities_.end(), rng);
}

std::unique_ptr<Ability> AbilityManager::getAbility() {
    if (abilities_.empty()) {
        throw EmptyAbilityException();
    }
    std::unique_ptr<Ability> ability = std::move(abilities_.front());
    abilities_.erase(abilities_.begin());
    return ability;
}

void AbilityManager::addAbility() {

    std::cout << "Добавляем абилку\n";

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

void AbilityManager::printAbilities() const {
    if (this->abilities_.empty()) {
        std::cout << "Список способностей пуст\n";
        return;
    }
    std::cout << "Список способностей:\n\n";
    for (int i = 0; i < this->abilities_.size(); i++) {
        std::cout << i << ": ";
        this->abilities_[i].get()->printAbility();
    }
    std::cout << "\n\n";
}