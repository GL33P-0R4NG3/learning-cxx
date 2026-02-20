#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

/**
 * Using 1 byte sized enum since more is not needed
 */
enum class DamageType : std::uint8_t
{
    Force,
    Bludgeoning,
    Slashing,
    Piercing,
    Fire,
    Cold,
    Acid,
    Poison,
    Lightning,
    Thunder,
    Necrotic,
    Radiant,
    Psychic
};

struct Damage
{
    std::int16_t value;
    DamageType type;
    bool isMagic;
    // For now, I will use just bool, and later I can use of the whole byte for flags including the isMagic option
    // since bool is just waste of byte
    //std::uint8_t flags;
};

struct Health
{
    using HitPoints = std::uint16_t;
    explicit Health(const HitPoints hp): currentHP(hp), maxHP(hp) {}

    HitPoints getMaxHP() const noexcept { return this->maxHP + this->modifiersHP; }
    const std::int16_t& getModifiersHP() const noexcept { return this->modifiersHP; }
    const HitPoints& getCurrentHP() const noexcept { return this->currentHP; }
    const HitPoints& getTemporaryHP() const noexcept { return this->temporaryHP; }

    void setModifierHP(const std::int16_t& modifier) noexcept
    {
        if (this->maxHP + modifier < this->currentHP) {
            this->currentHP = this->maxHP + modifier;
        }

        this->modifiersHP += modifier;
    }

    void setTemporaryHP(const HitPoints& temp) noexcept
    {
        if (temp == 0) return;

        this->temporaryHP = temp;
    }

    void applyDamage(const HitPoints& damage) noexcept
    {
        if (damage == 0) return;

        auto temp = static_cast<HitPoints>(damage);

        if (this->temporaryHP > 0) {
            if (this->temporaryHP < temp) {
                temp -= this->temporaryHP;
                this->temporaryHP = 0;
            } else {
                this->temporaryHP -= temp;
            }
        }

        if (this->currentHP < temp) {
            this->currentHP = 0;
        } else {
            this->currentHP -= temp;
        }
    }

    void applyHeal(const HitPoints& heal) noexcept
    {
        if (heal == 0) return;

        if (this->currentHP + heal > this->getMaxHP()) {
            this->currentHP = this->getMaxHP();
        } else {
            this->currentHP += heal;
        }
    }

    void applyTrueDamage(const std::uint32_t damage) noexcept
    {
        if (damage == 0) return;

        if (this->currentHP < damage) {
            this->currentHP = 0;
        } else {
            this->currentHP -= damage;
        }
    }

    std::string getHealthString() const noexcept
    {
        auto const temp = static_cast<HitPoints>(this->getTemporaryHP());
        auto str = std::to_string(this->currentHP) + "/" + std::to_string(this->getMaxHP());

        if (temp > 0) {
            str += " (+" + std::to_string(temp) + " temp)";
        }
        return str;
    }

private:
    HitPoints currentHP;
    HitPoints maxHP;
    HitPoints temporaryHP{0};
    std::int16_t modifiersHP{0};

};

class Character
{

public:
    explicit Character(std::string str) : name(std::move(str)), health(8), level(1) {}
    Character(std::string str, const Health h) : name(std::move(str)), health(h), level(1) {}
    std::string name;
    Health health;
    std::uint8_t level;

};

std::string readLine(const std::string& prompt)
{
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int readInt(const std::string& prompt)
{
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);

        try {
            return std::stoi(line);
        } catch (...) {
            std::cout << "Invalid number, try again.\n";
        }
    }
}

class TextRPGSimulator
{
    std::vector<Character> characters;

    void static printMenu()
    {
        std::cout << "\n=== Text Base RPG Simulator ===\n";
        std::cout << "1. Add character\n";
        std::cout << "2. Remove character\n";
        std::cout << "3. List characters\n";
        std::cout << "0. Exit\n";
    }

    void addCharacter()
    {
        std::string name = readLine("Enter name: ");
        const Health::HitPoints hp = readInt("Enter max HP: ");

        this->characters.emplace_back( name, Health{hp} );

        std::cout << "Character added.\n";
    }

    void removeCharacter()
    {
        if (this->characters.empty()) {
            std::cout << "Party is empty.\n";
            return;
        }

        this->listCharacters();

        const std::size_t index = readInt("Enter ID to remove: ");

        if (index >= this->characters.size()) {
            std::cout << "Invalid index.\n";
            return;
        }

        this->characters.erase(this->characters.begin() + index);
        std::cout << "Character removed.\n";
    }

    void listCharacters()
    {
        if (this->characters.empty()) {
            std::cout << "Party is empty.\n";
            return;
        }

        for (uint16_t i = 0; const auto& [name, health, level] : this->characters) {
            std::cout << "ID(" << i++ << "): " << name << " |Lvl. " << static_cast<int>(level) << "| - HP " << health.getHealthString() << '\n';
        }
    }

    public:
    TextRPGSimulator(): running(true) {}

    bool running;

    void run()
    {
        while (running) {
            printMenu();

            const int choice = readInt("Choice: ");
            system("CLS");  //Inefficient but compared to 1 line vs. about 40 lines of code in this case its better

            switch (choice) {
            case 1:
                addCharacter();
                break;
            case 2:
                removeCharacter();
                break;
            case 3:
                listCharacters();
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "Unknown option.\n";
                break;
            }
        }

        std::cout << "Goodbye!\n";
    }
};

int main()
{
    TextRPGSimulator rpg;
    rpg.run();

    return 0;
}