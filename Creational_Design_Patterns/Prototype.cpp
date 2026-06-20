#include <bits/stdc++.h>
using namespace std;

class Enemy {
public:
    virtual Enemy* clone() const = 0;
    virtual void setHealth(int h) = 0;
    virtual void addItem(const string& item) = 0;
    virtual void printStats() const = 0;
    virtual ~Enemy() = default;
};

class ConcreteEnemy : public Enemy {
private:
    string type;
    int health;
    double speed;
    bool armored;
    string weapon;
    vector<string> inventory;

public:
    ConcreteEnemy(string type, int health, double speed,
                  bool armored, string weapon,
                  vector<string> inventory)
        : type(move(type)),
          health(health),
          speed(speed),
          armored(armored),
          weapon(move(weapon)),
          inventory(move(inventory)) {}

    ConcreteEnemy* clone() const override {
        return new ConcreteEnemy(type, health, speed, armored, weapon, inventory);
    }

    void setHealth(int h) override {
        health = h;
    }

    void addItem(const string& item) override {
        inventory.push_back(item);
    }

    void printStats() const override {
        cout << type
             << " [Health: " << health
             << ", Speed: " << speed
             << ", Armored: " << (armored ? "true" : "false")
             << ", Weapon: " << weapon
             << ", Inventory: [";

        for (size_t i = 0; i < inventory.size(); i++) {
            if (i > 0) cout << ", ";
            cout << inventory[i];
        }

        cout << "]]\n";
    }
};

class EnemyRegistry {
private:
    map<string, unique_ptr<Enemy>> prototypes;

public:
    void registerPrototype(const string& key,
                           unique_ptr<Enemy> prototype) {
        prototypes[key] = move(prototype);
    }

    unique_ptr<Enemy> get(const string& key) {
        auto it = prototypes.find(key);

        if (it == prototypes.end()) {
            throw invalid_argument("No prototype registered for: " + key);
        }

        return unique_ptr<Enemy>(it->second->clone());
    }
};

int main() {
    EnemyRegistry registry;

    registry.registerPrototype(
        "flying",
        make_unique<ConcreteEnemy>(
            "FlyingEnemy",
            100,
            12.0,
            false,
            "Laser",
            vector<string>{"Speed Boost"}));

    registry.registerPrototype(
        "armored",
        make_unique<ConcreteEnemy>(
            "ArmoredEnemy",
            300,
            6.0,
            true,
            "Cannon",
            vector<string>{"Shield", "Helmet"}));

    auto e1 = registry.get("flying");
    auto e2 = registry.get("flying");

    e2->setHealth(80);
    e2->addItem("Smoke Bomb");

    auto e3 = registry.get("armored");

    e1->printStats();
    e2->printStats();
    e3->printStats();

    return 0;
}
    //                             main()
    //                                │
    //                                ▼
    //                  Create EnemyRegistry object
    //                                │
    //                                ▼
    //             ┌──────────────────────────────────┐
    //             │ registerPrototype("flying", ...) │
    //             └──────────────────────────────────┘
    //                                │
    //                                ▼
    //               Create FlyingEnemy prototype object
    //                                │
    //                                ▼
    //               Store in map with key "flying"
    //                                │
    //                                ▼
    //             ┌──────────────────────────────────┐
    //             │ registerPrototype("armored",...) │
    //             └──────────────────────────────────┘
    //                                │
    //                                ▼
    //              Create ArmoredEnemy prototype object
    //                                │
    //                                ▼
    //              Store in map with key "armored"
    //                                │
    //                                ▼
    //                        registry.get("flying")
    //                                │
    //                                ▼
    //                   Search map for key "flying"
    //                                │
    //                     ┌──────────┴──────────┐
    //                     │                     │
    //                  Found?                  No
    //                     │                     │
    //                    Yes                    ▼
    //                     │            throw invalid_argument
    //                     ▼
    //           it->second points to prototype
    //                     │
    //                     ▼
    //          Enemy* clone() called polymorphically
    //                     │
    //                     ▼
    //    ConcreteEnemy::clone() executes and creates

    //          new ConcreteEnemy(
    //             type,
    //             health,
    //             speed,
    //             armored,
    //             weapon,
    //             inventory
    //          )
    //                     │
    //                     ▼
    //              Return pointer to copy
    //                     │
    //                     ▼
    //       unique_ptr<Enemy> e1 receives cloned object
    //                     │
    //                     ▼
    //              registry.get("flying")
    //                     │
    //                     ▼
    //           Repeat same process → create e2
    //                     │
    //                     ▼
    //              e2->setHealth(80)
    //                     │
    //                     ▼
    //          Only e2 health changes to 80
    //                     │
    //                     ▼
    //         e2->addItem("Smoke Bomb")
    //                     │
    //                     ▼
    //     Only e2 inventory gets new item added
    //                     │
    //                     ▼
    //             registry.get("armored")
    //                     │
    //                     ▼
    //        Clone ArmoredEnemy prototype → e3
    //                     │
    //                     ▼
    //       e1->printStats()
    //       e2->printStats()
    //       e3->printStats()
    //                     │
    //                     ▼
    //                    End