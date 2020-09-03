#include <iostream>
#include <string>
#include <vector>
#include <memory>

struct Vec2
{
  float x, y;

  Vec2 operator+(const Vec2& other){
    return {x + other.x, y + other.y};
  }
};

class Character
{
protected:
  float health, attackDamage;
  Vec2 position;
  std::string name;

public:
  Character(const std::string& name) : name(name) {
    health = 100;
    attackDamage = 10;
    position = {0, 0};
    std::cout << "Created" << std::endl;
  }

  virtual ~Character(){
    std::cout << "Destroyed" << std::endl;
  }

  virtual void takeDamage(float amount){
    health -= attackDamage;
  }

  virtual void attack(Character& target){
    target.takeDamage(attackDamage);
  }

  void move(Vec2 amount){
    position = position + amount;
  }

  virtual void printInfo(){
    std::cout << "Character: " << name << "  |  ";
    std::cout << "x: " << position.x << " y: " << position.y << "  |  ";
    std::cout << "health: " << health << std::endl;
  }
};

class Player : public Character
{
private:
  int level = 3;

public:
  Player(const std::string& name) : Character(name) {}

  void attack(Character& target) override {
    // Character::attack(target);
    target.takeDamage(float(attackDamage * level));
  }

  void takeDamage(float amount) override {
    health -= attackDamage/level;
  }
};

class Enemy : public Character
{
public:
  Enemy(const std::string& name) : Character(name) {}
};


void damageAll(std::vector<std::shared_ptr<Character>>& characters, float amount){
  for (auto characterPointer : characters){
    characterPointer->takeDamage(amount);
  }
}

void damageAllEnemies(std::vector<std::shared_ptr<Character>>& characters, float amount){
  for (auto characterPointer : characters){
    auto enemyPointer = dynamic_cast<Enemy*>(characterPointer.get());
    if (enemyPointer != nullptr){
      enemyPointer->takeDamage(amount);
    }
  }
}

void printAll(std::vector<std::shared_ptr<Character>>& characters){
  for (auto characterPointer : characters){
    characterPointer->printInfo();
  }
}

int main() {
  auto player = std::make_shared<Player>("Jose");
  auto enemy = std::make_shared<Enemy>("Steven");
  auto character = std::make_shared<Character>("Kelly");

  std::vector<std::shared_ptr<Character>> characters;
  characters.reserve(10);

  characters.push_back(player);
  characters.push_back(enemy);
  characters.push_back(character);
  characters.push_back(std::make_shared<Enemy>("Peter"));
  characters.push_back(std::make_shared<Enemy>("Jackson"));

  enemy->attack(*player);
  player->attack(*enemy);

  damageAll(characters, 10.0f);
  printAll(characters);
  damageAllEnemies(characters, 15.0f);
  printAll(characters);

  std::cout << characters[0].use_count() << std::endl;
  std::cout << characters[3].use_count() << std::endl;

  Character* castCharacter = dynamic_cast<Character*>(enemy.get());
  Enemy* castEnemy = dynamic_cast<Enemy*>(character.get());
  Player* castPlayer = dynamic_cast<Player*>(enemy.get());
  std::cout << castCharacter << std::endl;
  std::cout << castEnemy << std::endl;
  std::cout << castPlayer << std::endl;

  return 1;
}