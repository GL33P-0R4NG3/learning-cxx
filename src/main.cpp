#include <cstdint>
#include <iostream>

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
    bool isMagic;           // For now, I will use just bool, and later I can use of the whole byte for flags including the isMagic option since bool is just waste of byte
    //std::uint8_t flags;
};

struct Health
{
    explicit Health(const std::uint16_t hp): currentHP(hp), maxHP(hp) {}

    std::uint16_t currentHP;
    std::uint16_t maxHP;

    std::uint32_t getMaxHP() const { return this->maxHP + this->modifiersHP; }

    void setModifierHP(const std::int16_t modifier)
    {
        if (this->maxHP + modifier < currentHP)
        {
            this->currentHP = this->maxHP + modifier;
        }

        modifiersHP += modifier;
    }

    void dealDamage(uint32_t damage)
    {

    }

private:
    std::int16_t modifiersHP{0};
    std::uint16_t temporaryHP{0};
};

class Character
{

};

int main()
{
    const auto lang = "C++";
    std::cout << "Hello and welcome to " << lang << "!\n";

    for (int i = 1; i <= 5; i++)
    {
        std::cout << "i = " << i << std::endl;
    }

    return 0;
}