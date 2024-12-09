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

bool AbilityManager::isAbleToUseAbility() const {
    return !this->abilities_.empty();
}

std::unique_ptr<Ability> AbilityManager::createAbility(int type) const {
    switch (type) {
        case 0:
            return std::make_unique<DoubleDamage>();
        case 1:
            return std::make_unique<Scanner>();
        case 2:
            return std::make_unique<RandomAttack>();
        default:
            throw std::runtime_error("Неизвестный тип способности");
    }
}

AbilityManager::AbilityManager(const std::vector<int>& abilityTypes) {
    for (int type : abilityTypes) {
        abilities_.push_back(createAbility(type));
    }
}

std::vector<int> AbilityManager::getAvailableAbilities() const {
    std::vector<int> types;
    for (const auto& ability : abilities_) {
        if (dynamic_cast<DoubleDamage*>(ability.get())) {
            types.push_back(0);
        } else if (dynamic_cast<Scanner*>(ability.get())) {
            types.push_back(1);
        } else if (dynamic_cast<RandomAttack*>(ability.get())) {
            types.push_back(2);
        }
    }
    return types;
}

void AbilityManager::setAbilities(const std::vector<int>& abilityTypes) {
    abilities_.clear();
    for (int type : abilityTypes) {
        abilities_.push_back(createAbility(type));
    }
}
