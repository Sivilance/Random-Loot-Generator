#include <iostream>  // For standard input and output operations (cin and cout)
#include <random>    // For generating random numbers and distributions
#include <limits>    // For handling input stream limits (clearCin function)
#include <vector>    // For using the vector container to store loot items
#include <map>       // For using the map container to associate material pools
#include <algorithm> // For std::find to use item tags

using namespace std;

struct Item {
    string name;
    int quantity;
};

// Define the item pool with tags, materials, and values
struct ItemInfo {
    std::string name;
    std::string locationTags;
    int value;
};

// Global random number generator
random_device rd;
mt19937 gen(rd());

double generateRandomNumber(int input, int multiplier) {
    uniform_real_distribution<> dis(0.9, 1.1);  // Apply 10% deviation
    double baseResult = input * multiplier;
    return baseResult * dis(gen);  // Return result after applying deviation
}

double applyDeviation(int baseValue) {
    uniform_real_distribution<> dis(0.8, 1.2);  // Apply 20% deviation
    return baseValue * dis(gen);
}

struct CombatDiceResult {
    int result;
    bool effect;
};

CombatDiceResult rollCombatDice() {
    uniform_int_distribution<> dis(1, 6); // d6 distribution

    int roll = dis(gen);
    CombatDiceResult combatDiceResult;

    if (roll == 1) {
        combatDiceResult.result = 1;
        combatDiceResult.effect = false;
    }
    else if (roll == 2) {
        combatDiceResult.result = 2;
        combatDiceResult.effect = false;
    }
    else if (roll == 3 || roll == 4) {
        combatDiceResult.result = 0;
        combatDiceResult.effect = false;
    }
    else if (roll == 5 || roll == 6) {
        combatDiceResult.result = 1;
        combatDiceResult.effect = true;
    }

    return combatDiceResult;
}


void clearCin() {
    cin.clear(); // Clear error flag
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore wrong input
}

// Ammunition pool
std::vector<ItemInfo> ammoPool = {
    {".38", "Everywhere", 1},
    {"10mm", "Everywhere", 2},
    {".308", "Everywhere", 3},
    {"Flare", "Everywhere", 1},
    {"Shotgun Shell", "Everywhere", 3},
    {".45", "Everywhere", 3},
    {"Flamer Fuel", "Everywhere", 1},
    {"Fusion Cell", "Everywhere", 3},
    {"Gamma Round", "Military, Industry, Medical, Agriculture", 3},
    {"Railway Spike", "Industry", 3},
    {"Syringer Ammo", "Commercial, Medical, Agriculture", 45},
    {".44 Magnum", "Everywhere", 3},
    {".50", "Military", 4},
    {"5.56mm", "Everywhere", 2},
    {"5mm x10", "Military", 3},  // Drops as x10 packs
    {"Fusion Core", "Military, Commercial, Industry, Medical", 200},
    {"Missile", "Military", 25},
    {"Plasma Cartridge", "Everywhere", 5},
    {"2mm", "Military", 10},
    {"Mini-Nuke", "Military", 100}
};

vector<ItemInfo> armorPool = {
    {"Raider Chest Piece", "Everywhere", 18},
    {"Raider Leg", "Everywhere", 8},
    {"Raider Arm", "Everywhere", 6},
    {"Sturdy Raider Chest Piece", "Everywhere", 33},
    {"Sturdy Raider Leg", "Everywhere", 13},
    {"Sturdy Raider Arm", "Everywhere", 8},
    {"Heavy Raider Chest Piece", "Everywhere", 48},
    {"Heavy Raider Leg", "Everywhere", 18},
    {"Heavy Raider Arm", "Everywhere", 15},
    {"Leather Chest Piece", "Everywhere", 25},
    {"Leather Leg", "Everywhere", 10},
    {"Leather Arm", "Everywhere", 8},
    {"Sturdy Leather Chest Piece", "Everywhere", 50},
    {"Sturdy Leather Leg", "Everywhere", 20},
    {"Sturdy Leather Arm", "Everywhere", 18},
    {"Heavy Leather Chest Piece", "Everywhere", 75},
    {"Heavy Leather Leg", "Everywhere", 30},
    {"Heavy Leather Arm", "Everywhere", 28},
    {"Metal Helmet", "Everywhere", 15},
    {"Metal Chest Piece", "Everywhere", 40},
    {"Metal Leg", "Everywhere", 15},
    {"Metal Arm", "Everywhere", 15},
    {"Sturdy Metal Helmet", "Everywhere", 65},
    {"Sturdy Metal Chest Piece", "Everywhere", 115},
    {"Sturdy Metal Leg", "Everywhere", 65},
    {"Sturdy Metal Arm", "Everywhere", 65},
    {"Heavy Metal Helmet", "Everywhere", 115},
    {"Heavy Metal Chest Piece", "Everywhere", 190},
    {"Heavy Metal Leg", "Everywhere", 115},
    {"Heavy Metal Arm", "Everywhere", 115},
    {"Combat Helmet", "Residential, Commercial, Industry, Medical, Agriculture", 25},
    {"Combat Helmet", "Military", 15},
    {"Combat Chest Piece", "Residential, Commercial, Industry, Medical, Agriculture", 60},
    {"Combat Chest Piece", "Military", 45},
    {"Combat Leg", "Residential, Commercial, Industry, Medical, Agriculture", 25},
    {"Combat Leg", "Military", 15},
    {"Combat Arm", "Residential, Commercial, Industry, Medical, Agriculture", 25},
    {"Combat Arm", "Military", 15},
    {"Sturdy Combat Helmet", "Residential, Commercial, Industry, Medical, Agriculture", 105},
    {"Sturdy Combat Helmet", "Military", 90},
    {"Sturdy Combat Chest Piece", "Residential, Commercial, Industry, Medical, Agriculture", 140},
    {"Sturdy Combat Chest Piece", "Military", 120},
    {"Sturdy Combat Leg", "Residential, Commercial, Industry, Medical, Agriculture", 105},
    {"Sturdy Combat Leg", "Military", 90},
    {"Sturdy Combat Arm", "Residential, Commercial, Industry, Medical, Agriculture", 105},
    {"Sturdy Combat Arm", "Military", 90},
    {"Heavy Combat Helmet", "Residential, Commercial, Industry, Medical, Agriculture", 185},
    {"Heavy Combat Helmet", "Military", 150},
    {"Heavy Combat Chest Piece", "Residential, Commercial, Industry, Medical, Agriculture", 220},
    {"Heavy Combat Chest Piece", "Military", 175},
    {"Heavy Combat Leg", "Residential, Commercial, Industry, Medical, Agriculture", 185},
    {"Heavy Combat Leg", "Military", 150},
    {"Heavy Combat Arm", "Residential, Commercial, Industry, Medical, Agriculture", 145},
    {"Heavy Combat Arm", "Military", 130}
};

// Define material pools
map<string, vector<string>> materialPools = {
    {"Common", {"Bone", "Ceramic", "Cloth", "Concrete", "Leather", "Plastic", "Rubber", "Steel", "Wood"}},
    {"Uncommon", {"Adhesive", "Aluminum", "Copper", "Cork", "Fertilizer", "Fiberglass", "Gears", "Glass", "Lead", "Oil", "Screw", "Silver", "Spring"}},
    {"Rare", {"Acid", "Antiseptic", "Asbestos", "Ballistic fiber", "Circuitry", "Fiber optics", "Nuclear material"}}
};

// Specific pools for each creature
map<string, vector<string>> creatureMaterialPools = {
    {"Bloodbug", {"Bone", "Fiberglass"}},
    {"Bloatfly", {"Bone", "Fiberglass"}},
    {"Brahmin", {"Bone", "Leather", "Fertilizer"}},
    {"Deathclaw", {"Bone", "Leather"}},
    {"Mirelurk Queen", {"Bone"}},
    {"Molerat", {"Bone", "Leather"}},
    {"RadRoach", {"Bone", "Fiberglass"}},
    {"RadScorpion", {"Acid", "Antiseptic"}},
    {"Stingwing", {"Bone", "Fiberglass"}}
};

// Robot material pools
map<string, vector<string>> robotMaterialPools = {
    {"Common", {"Steel", "Rubber", "Plastic"}},
    {"Uncommon", {"Adhesive", "Aluminum", "Cork", "Fiberglass", "Gears", "Glass", "Lead", "Oil", "Screw", "Silver", "Spring"}},
    {"Rare", {"Ballistic fiber", "Circuitry", "Fiber optics"}}
};

// Synth material pools
map<string, vector<string>> synthMaterialPools = {
    {"Common", {"Ceramic", "Plastic", "Steel"}},
    {"Uncommon", {"Aluminum", "Copper", "Gears", "Lead", "Oil", "Screw"}},
    {"Rare", {"Ballistic fiber", "Circuitry", "Fiber optics", "Nuclear material"}}
};

// Turret material pools
map<string, vector<string>> turretMaterialPools = {
    {"Common", {"Plastic", "Rubber", "Steel"}},
    {"Uncommon", {"Aluminum", "Copper", "Gears", "Lead", "Oil", "Screw", "Spring"}},
    {"Rare", {"Ballistic fiber", "Circuitry", "Fiber optics"}}
};


string getRandomMaterial(const vector<string>& pool) {
    uniform_int_distribution<> dis(0, pool.size() - 1);
    return pool[dis(gen)];
}

pair<vector<Item>, int> generateLootForPlace(int inputNumber, int multiplier, const string& placeCategory) {
    vector<Item> loot;
    int totalLootValue = 0;
    int lootValue = generateRandomNumber(inputNumber, multiplier);

    vector<ItemInfo> potentialLoot;

    // Filter potential loot based on place category
    for (const auto& item : ammoPool) {
        if (item.locationTags.find(placeCategory) != string::npos || item.locationTags == "Everywhere") {
            potentialLoot.push_back(item);
        }
    }

    for (const auto& item : armorPool) {
        if (item.locationTags.find(placeCategory) != string::npos || item.locationTags == "Everywhere") {
            potentialLoot.push_back(item);
        }
    }

    // Generate loot until the total value is reached
    while (lootValue > 0 && !potentialLoot.empty()) {
        // Select a random item from the potential loot
        uniform_int_distribution<> dis(0, potentialLoot.size() - 1);
        const auto& selectedItem = potentialLoot[dis(gen)];

        // Add the item to the loot
        loot.push_back({ selectedItem.name, 1 });

        // Subtract the item's value from the total loot value
        lootValue -= selectedItem.value;
        totalLootValue += selectedItem.value;
    }

    // If there's remaining value, convert it to caps
    if (lootValue > 0) {
        loot.push_back({ "Caps", lootValue });
        totalLootValue += lootValue;
    }

    return { loot, totalLootValue };
}



vector<Item> generateLootForAnimal(int animalChoice, int luck) {
    vector<Item> loot;
    int effectCount = 0;
    CombatDiceResult dcResult;
    vector<string> materialPool;

    switch (animalChoice) {
    case 1: // Bloodbug
        loot.push_back({ "Bloodbug Meat", 1 });
        dcResult = rollCombatDice();
        if (dcResult.result > 0) {
            materialPool = creatureMaterialPools["Bloodbug"];
            loot.push_back({ getRandomMaterial(materialPool), dcResult.result });
        }
        break;
    case 2: // Bloatfly
        loot.push_back({ "Bloatfly Meat", 1 });
        dcResult = rollCombatDice();
        if (dcResult.result > 0) {
            materialPool = creatureMaterialPools["Bloatfly"];
            loot.push_back({ getRandomMaterial(materialPool), dcResult.result });
        }
        break;
    case 3: // Brahmin
        loot.push_back({ "Brahmin Meat", 3 });
        dcResult = rollCombatDice();
        if (dcResult.result > 0) {
            materialPool = creatureMaterialPools["Brahmin"];
            loot.push_back({ getRandomMaterial(materialPool), dcResult.result });
        }
        break;
    case 4: // Deathclaw
        loot.push_back({ "Deathclaw Meat", 5 });
        for (int i = 0; i < 2; ++i) {
            dcResult = rollCombatDice();
            if (dcResult.result > 0) {
                materialPool = creatureMaterialPools["Deathclaw"];
                loot.push_back({ getRandomMaterial(materialPool), dcResult.result });
            }
        }
        break;
    case 5: // Mirelurk
        for (int i = 0; i < 2; ++i) {
            dcResult = rollCombatDice();
            loot.push_back({ "Mirelurk Meat", dcResult.result });
            if (dcResult.effect) {
                loot.push_back({ "Softshell Mirelurk Meat", 1 });
            }
        }
        break;
    case 6: // Mirelurk Queen
        loot.push_back({ "Mirelurk Queen Meat", 10 });
        dcResult = rollCombatDice();
        if (dcResult.result > 0) {
            materialPool = creatureMaterialPools["Mirelurk Queen"];
            loot.push_back({ getRandomMaterial(materialPool), dcResult.result });
        }
        loot.push_back({ "Junk", 3 });
        loot.push_back({ "Caps", static_cast<int>(applyDeviation(luck * 8)) });
        loot.push_back({ "Ammunition", static_cast<int>(applyDeviation(luck * 3)) });
        break;
    case 7: // Mole Rat
        loot.push_back({ "Mole Rat Meat", 1 });
        dcResult = rollCombatDice();
        if (dcResult.result > 0) {
            materialPool = creatureMaterialPools["Molerat"];
            loot.push_back({ getRandomMaterial(materialPool), dcResult.result });
        }
        break;
    case 8: // RadRoach
        loot.push_back({ "RadRoach Meat", 1 });
        dcResult = rollCombatDice();
        if (dcResult.result > 0) {
            materialPool = creatureMaterialPools["RadRoach"];
            loot.push_back({ getRandomMaterial(materialPool), dcResult.result });
        }
        break;
    case 9: // RadScorpion
        for (int i = 0; i < 2; ++i) {
            dcResult = rollCombatDice();
            loot.push_back({ "RadScorpion Meat", dcResult.result });
            if (dcResult.effect) {
                materialPool = creatureMaterialPools["RadScorpion"];
                loot.push_back({ getRandomMaterial(materialPool), 1 });
            }
        }   
        if (effectCount == 1) {
            loot.push_back({ "Rare Material", 1 });
        }
        else if (effectCount >= 2) {
            loot.push_back({ "RadScorpion Egg", 1 });
        }
        break;
    case 10: // Stingwing
        loot.push_back({ "Stingwing Meat", 1 });
        dcResult = rollCombatDice();
        if (dcResult.result > 0) {
            materialPool = creatureMaterialPools["Stingwing"];
            loot.push_back({ getRandomMaterial(materialPool), dcResult.result });
        }
        break;
    default:
        cout << "Invalid animal choice.\n";
        break;
    }

    return loot;
}

vector<Item> generateLootForMutatedHumanoids(int humanoidChoice, int luck) {
    vector<Item> loot;
    for (int i = 0; i < 2; ++i) {
        CombatDiceResult dcResult = rollCombatDice();
        loot.push_back({ "Junk", dcResult.result });
    }
    return loot;
}

vector<Item> generateLootForRobots(int robotChoice, int luck) {
    vector<Item> loot;
    CombatDiceResult dcResult;

    auto dropMaterial = [&](const string& rarityPool) {
        int luckBasedRoll = uniform_int_distribution<>(1, 100)(gen);
        if (luckBasedRoll <= luck) {
            loot.push_back({ getRandomMaterial(robotMaterialPools["Rare"]), 1 });
        }
        else {
            dcResult = rollCombatDice();
            if (dcResult.result > 0) {
                loot.push_back({ getRandomMaterial(robotMaterialPools[rarityPool]), dcResult.result });
            }
        }
        };

    switch (robotChoice) {
    case 1: // Assaultron
        for (int i = 0; i < 3; ++i) {
            dcResult = rollCombatDice();
            loot.push_back({ "Fusion Cells", dcResult.result });
            if (dcResult.effect) {
                dropMaterial("Uncommon");
            }
        }
        cout << "Note: You can gain +1 DC Fusion Cells per AP Spent\n";
        break;
    case 2: // Eyebot
        dropMaterial("Common");
        break;
    case 3: // Mister Handy
        for (int i = 0; i < 2; ++i) {
            dropMaterial("Common");
        }
        loot.push_back({ "Flamer Fuel", static_cast<int>(applyDeviation(luck * 4)) });
        cout << "Note: You can gain +1 DC common material per AP Spent\n";
        break;
    case 4: // Mister Gutsy
        for (int i = 0; i < 2; ++i) {
            dropMaterial("Common");
        }
        loot.push_back({ "10mm Ammunition", static_cast<int>(applyDeviation(luck * 4)) });
        cout << "Note: You can gain +1 DC common material per AP Spent\n";
        break;
    case 5: // Protectron
        for (int i = 0; i < 2; ++i) {
            dropMaterial("Common");
        }
        cout << "Note: You can gain +1 DC common material per AP Spent\n";
        break;
    case 6: // Sentry Bot
        if (uniform_int_distribution<>(1, 2)(gen) == 1) {
            loot.push_back({ "Fusion Core Charge", uniform_int_distribution<>(1, 10)(gen) });
        }
        for (int i = 0; i < 2; ++i) {
            dropMaterial("Common");
        }
        cout << "Note: You can gain +1 DC common material per AP Spent\n";
        break;
    default:
        cout << "Invalid robot choice.\n";
        break;
    }

    return loot;
}

vector<Item> generateLootForSuperMutants(int mutantChoice, int luck) {
    vector<Item> loot;
    CombatDiceResult dcResult;

    switch (mutantChoice) {
    case 1: // Super Mutant
        if (uniform_int_distribution<>(1, 100)(gen) <= 90) {
            loot.push_back({ "Pipe Bolt-Action Rifle", 1 });
        }
        if (uniform_int_distribution<>(1, 100)(gen) <= 90) {
            loot.push_back({ "Board", 1 });
        }
        loot.push_back({ "Bone", static_cast<int>(applyDeviation(luck * 0.5)) });
        loot.push_back({ "Caps", static_cast<int>(applyDeviation(luck * 2)) });
        break;
    case 2: // Super Mutant Behemoth
        loot.push_back({ "Caps", static_cast<int>(applyDeviation(luck * 6)) });
        for (int i = 0; i < 4; ++i) {
            dcResult = rollCombatDice();
            loot.push_back({ "Junk", dcResult.result });
        }
        if (luck <= 4) {
            int roll = uniform_int_distribution<>(1, 100)(gen);
            if (roll <= 40) {
                loot.push_back({ "Board", 1 });
            }
            else if (roll <= 65) {
                loot.push_back({ "Pipe Bolt-Action Rifle", 1 });
            }
            else if (roll <= 80) {
                loot.push_back({ "Fire Hydrant Bat", 1 });
            }
            else if (roll <= 90) {
                loot.push_back({ "Missile Launcher", 1 });
            }
            // 10% chance for none
        }
        else if (luck <= 6) {
            int roll = uniform_int_distribution<>(1, 100)(gen);
            if (roll <= 20) {
                loot.push_back({ "Board", 1 });
            }
            else if (roll <= 50) {
                loot.push_back({ "Pipe Bolt-Action Rifle", 1 });
            }
            else if (roll <= 75) {
                loot.push_back({ "Fire Hydrant Bat", 1 });
            }
            else if (roll <= 100) {
                loot.push_back({ "Missile Launcher", 1 });
            }
        }
        else if (luck <= 8) {
            int roll = uniform_int_distribution<>(1, 100)(gen);
            if (roll <= 10) {
                loot.push_back({ "Board", 1 });
            }
            else if (roll <= 20) {
                loot.push_back({ "Pipe Bolt-Action Rifle", 1 });
            }
            else if (roll <= 60) {
                loot.push_back({ "Fire Hydrant Bat", 1 });
            }
            else if (roll <= 95) {
                loot.push_back({ "Missile Launcher", 1 });
            }
            else {
                loot.push_back({ "Fire Hydrant Bat", 1 });
                loot.push_back({ "Missile Launcher", 1 });
            }
        }
        else {
            int roll = uniform_int_distribution<>(1, 100)(gen);
            if (roll <= 50) {
                loot.push_back({ "Fire Hydrant Bat", 1 });
            }
            else if (roll <= 90) {
                loot.push_back({ "Missile Launcher", 1 });
            }
            else {
                loot.push_back({ "Fire Hydrant Bat", 1 });
                loot.push_back({ "Missile Launcher", 1 });
            }
        }
        cout << "GM must decide your ammunition in the weapon just dropped.\n";
        break;
    default:
        cout << "Invalid mutant choice.\n";
        break;
    }

    return loot;
}

vector<Item> generateLootForSynths(int synthChoice, int luck) {
    vector<Item> loot;
    uniform_int_distribution<> fusionCellDist(0, 6 * luck);
    uniform_int_distribution<> percentDist(1, 100);
    CombatDiceResult dcResult;

    auto dropMaterial = [&](const string& rarityPool, int quantity) {
        for (int i = 0; i < quantity; ++i) {
            loot.push_back({ getRandomMaterial(synthMaterialPools[rarityPool]), 1 });
        }
        };

    switch (synthChoice) {
    case 1: // Synth
        loot.push_back({ "Institute Laser Gun (Photon Exciter, Improved Barrel)", 1 });
        loot.push_back({ "Shock Baton", 1 });
        loot.push_back({ "Fusion Cells", fusionCellDist(gen) });
        dropMaterial("Common", 2);
        dropMaterial("Uncommon", 1);
        if (percentDist(gen) <= 50) {
            dropMaterial("Rare", 1);
        }
        if (percentDist(gen) <= 10) {
            loot.push_back({ "Synth Helmet", 1 });
        }
        break;
    case 2: // Synth Strider
        loot.push_back({ "Institute Laser Gun (Boosted Photon Agitator, Long Barrel, Short Scope)", 1 });
        loot.push_back({ "Shock Baton", 1 });
        loot.push_back({ "Fusion Cells", fusionCellDist(gen) });
        loot.push_back({ "Sturdy Synth Helmet", 1 });
        loot.push_back({ "Sturdy Synth Chest piece", 1 });
        loot.push_back({ "Sturdy Synth Leg", 1 });
        loot.push_back({ "Sturdy Synth Arm", 1 });
        dropMaterial("Common", 3);
        dropMaterial("Uncommon", 2);
        dropMaterial("Rare", 1);
        break;
    case 3: // Synth Trooper
        loot.push_back({ "Institute Laser Gun (Boosted Photon Agitator, Long Barrel, Short Scope)", 1 });
        loot.push_back({ "Shock Baton", 1 });
        loot.push_back({ "Fusion Cells", fusionCellDist(gen) });
        loot.push_back({ "Sturdy Synth Helmet", 1 });
        loot.push_back({ "Sturdy Synth Chest piece", 1 });
        loot.push_back({ "Sturdy Synth Leg", 2 });
        loot.push_back({ "Sturdy Synth Arm", 2 });
        dropMaterial("Common", 3);
        dropMaterial("Uncommon", 3);
        dropMaterial("Rare", 2);
        break;
    default:
        cout << "Invalid synth choice.\n";
        break;
    }

    return loot;
}

vector<Item> generateLootForTurrets(int turretChoice, int luck) {
    vector<Item> loot;
    uniform_int_distribution<> fusionCellDist(0, 6 * luck);
    CombatDiceResult dcResult;

    auto dropMaterial = [&](const string& rarityPool, int quantity) {
        for (int i = 0; i < quantity; ++i) {
            loot.push_back({ getRandomMaterial(turretMaterialPools[rarityPool]), 1 });
        }
        };

    switch (turretChoice) {
    case 1: // Machine Gun Turret MK I
        loot.push_back({ "5.56mm rounds", fusionCellDist(gen) });
        dcResult = rollCombatDice();
        loot.push_back({ getRandomMaterial(turretMaterialPools["Common"]), dcResult.result });
        for (int i = 0; i < 2; ++i) {
            dcResult = rollCombatDice();
            loot.push_back({ getRandomMaterial(turretMaterialPools["Uncommon"]), dcResult.result });
        }
        break;
    case 2: // Machine Gun Turret MK III
        loot.push_back({ "5.56mm rounds", fusionCellDist(gen) });
        dcResult = rollCombatDice();
        loot.push_back({ getRandomMaterial(turretMaterialPools["Common"]), dcResult.result });
        for (int i = 0; i < 2; ++i) {
            dcResult = rollCombatDice();
            loot.push_back({ getRandomMaterial(turretMaterialPools["Uncommon"]), dcResult.result });
        }
        if (dcResult.effect) {
            loot.push_back({ getRandomMaterial(turretMaterialPools["Rare"]), 1 });
        }
        break;
    case 3: // Machine Gun Turret MK IV
        loot.push_back({ "5.56mm rounds", fusionCellDist(gen) });
        for (int i = 0; i < 2; ++i) {
            dcResult = rollCombatDice();
            loot.push_back({ getRandomMaterial(turretMaterialPools["Common"]), dcResult.result });
        }
        for (int i = 0; i < 3; ++i) {
            dcResult = rollCombatDice();
            loot.push_back({ getRandomMaterial(turretMaterialPools["Uncommon"]), dcResult.result });
        }
        if (dcResult.effect) {
            loot.push_back({ getRandomMaterial(turretMaterialPools["Rare"]), 1 });
        }
        break;
    default:
        cout << "Invalid turret choice.\n";
        break;
    }

    return loot;
}

vector<Item> generateLootForBrotherhood(int brotherhoodChoice, int luck) {
    vector<Item> loot;
    uniform_int_distribution<> fusionCellDist;
    uniform_int_distribution<> pieceCountDist;
    uniform_int_distribution<> percentDist(1, 100);
    vector<string> armorPieces = { "Helmet", "Right Arm", "Left Arm", "Torso", "Left Leg", "Right Leg" };
    vector<string> droppedPieces;
    int maxPieces;
    string randomPiece;

    auto dropArmorPieces = [&](int maxPieces, const string& prefix) {
        for (int i = 0; i < maxPieces; ++i) {
            do {
                randomPiece = prefix + " " + armorPieces[uniform_int_distribution<>(0, armorPieces.size() - 1)(gen)];
            } while (find(droppedPieces.begin(), droppedPieces.end(), randomPiece) != droppedPieces.end());
            droppedPieces.push_back(randomPiece);
            loot.push_back({ randomPiece, 1 });
        }
        };

    switch (brotherhoodChoice) {
    case 1: // Knight
        loot.push_back({ "Brotherhood of Steel Uniform", 1 });
        if (luck <= 4) {
            maxPieces = 1;
        }
        else if (luck <= 6) {
            maxPieces = uniform_int_distribution<>(2, 3)(gen);
        }
        else if (luck <= 8) {
            maxPieces = uniform_int_distribution<>(3, 4)(gen);
        }
        else if (luck == 9) {
            maxPieces = uniform_int_distribution<>(4, 5)(gen);
        }
        else {
            maxPieces = 6; // Full set
        }
        dropArmorPieces(maxPieces, "Combat Armor");
        loot.push_back({ "Long Laser Rifle", 1 });
        fusionCellDist = uniform_int_distribution<>(0, 6 * luck);
        loot.push_back({ "Fusion Cells", fusionCellDist(gen) });
        loot.push_back({ "Brotherhood of Steel Holotags", 1 });
        break;
    case 2: // Paladin
        loot.push_back({ "Power Armor Frame", 1 });
        if (luck <= 4) {
            maxPieces = 1;
        }
        else if (luck <= 6) {
            maxPieces = uniform_int_distribution<>(2, 3)(gen);
        }
        else if (luck <= 8) {
            maxPieces = uniform_int_distribution<>(3, 4)(gen);
        }
        else if (luck == 9) {
            maxPieces = uniform_int_distribution<>(4, 5)(gen);
        }
        else {
            maxPieces = 6; // Full set
        }
        dropArmorPieces(maxPieces, "T-60 Power Armor");
        loot.push_back({ "Improved Long Laser Rifle", 1 });
        fusionCellDist = uniform_int_distribution<>(0, 8 * luck);
        loot.push_back({ "Fusion Cells", fusionCellDist(gen) });
        loot.push_back({ "Brotherhood of Steel Holotags", 1 });
        break;
    case 3: // Initiate
        loot.push_back({ "Brotherhood fatigues", 1 });
        loot.push_back({ "Brotherhood hood", 1 });
        loot.push_back({ "Combat knife", 1 });
        if (uniform_int_distribution<>(1, 2)(gen) == 1) {
            loot.push_back({ "Laser pistol", 1 });
            fusionCellDist = uniform_int_distribution<>(0, 4 * luck);
            loot.push_back({ "Fusion Cells", fusionCellDist(gen) });
        }
        else {
            loot.push_back({ "10mm pistol", 1 });
            fusionCellDist = uniform_int_distribution<>(0, 6 * luck);
            loot.push_back({ "10mm rounds", fusionCellDist(gen) });
        }
        loot.push_back({ "Brotherhood holotags", 1 });
        break;
    default:
        cout << "Invalid Brotherhood of Steel choice.\n";
        break;
    }

    return loot;
}

vector<Item> generateLootForRaiders(int raiderChoice, int luck) {
    vector<Item> loot;
    uniform_int_distribution<> capsDist;
    CombatDiceResult dcResult;

    switch (raiderChoice) {
    case 1: // Raider
        loot.push_back({ "Road Leathers", 1 });
        loot.push_back({ "Pipe Gun", 1 });
        loot.push_back({ "Tire Iron", 1 });
        capsDist = uniform_int_distribution<>(0, 2 * luck);
        loot.push_back({ "Caps", capsDist(gen) });
        break;
    case 2: // Raider Boss
        loot.push_back({ "Heavy Raider Chest Piece", 1 });
        if (luck < 6) {
            loot.push_back({ "Sturdy Raider Leg", 1 });
            loot.push_back({ "Heavy Raider Arm", 1 });
        }
        else {
            loot.push_back({ "Sturdy Raider Leg", 2 });
            loot.push_back({ "Heavy Raider Arm", 2 });
        }
        dcResult = rollCombatDice();
        loot.push_back({ "Frag Grenades", dcResult.result });
        loot.push_back({ "Hunting Rifle", 1 });
        capsDist = uniform_int_distribution<>(0, 5 * luck);
        loot.push_back({ "Caps", capsDist(gen) });
        break;
    case 3: // Raider Psycho
        loot.push_back({ "Road Leathers", 1 });
        loot.push_back({ "Leather Chest Piece", 1 });
        loot.push_back({ "Double-Barreled Shotgun", 1 });
        dcResult = rollCombatDice();
        loot.push_back({ "Shells", luck * dcResult.result });
        loot.push_back({ "Machete", 1 });
        capsDist = uniform_int_distribution<>(0, 3 * luck);
        loot.push_back({ "Caps", capsDist(gen) });
        break;
    case 4: // Raider Veteran
        loot.push_back({ "Spike Armor", 1 });
        loot.push_back({ "Combat Rifle", 1 });
        loot.push_back({ ".45 Ammunition", static_cast<int>(applyDeviation(luck * 2)) });
        loot.push_back({ "Machete", 1 });
        capsDist = uniform_int_distribution<>(0, 4 * luck);
        loot.push_back({ "Caps", capsDist(gen) });
        break;
    default:
        cout << "Invalid raider choice.\n";
        break;
    }

    return loot;
}

vector<Item> generateLootForWastelanders(int wastelanderChoice, int luck) {
    vector<Item> loot;
    uniform_int_distribution<> capsDist;
    uniform_int_distribution<> fusionCellDist;
    uniform_int_distribution<> ammoDist;
    CombatDiceResult dcResult;

    switch (wastelanderChoice) {
    case 1: // Gunner
        loot.push_back({ "Combat Armor Chest Piece", 1 });
        if (luck < 6) {
            loot.push_back({ "Leather Leg", 1 });
            loot.push_back({ "Leather Arm", 1 });
        }
        else {
            loot.push_back({ "Leather Leg", 2 });
            loot.push_back({ "Leather Arm", 2 });
        }
        if (uniform_int_distribution<>(1, 2)(gen) == 1) {
            loot.push_back({ "Combat Rifle", 1 });
            ammoDist = uniform_int_distribution<>(0, 6 * luck);
            loot.push_back({ "10mm rounds", ammoDist(gen) });
        }
        else {
            loot.push_back({ "Laser Gun", 1 });
            fusionCellDist = uniform_int_distribution<>(0, 4 * luck);
            loot.push_back({ "Fusion Cells", fusionCellDist(gen) });
        }
        capsDist = uniform_int_distribution<>(0, 4 * luck);
        loot.push_back({ "Caps", capsDist(gen) });
        break;
    case 2: // Mercenary
        loot.push_back({ "Combat Armor Chest Piece", 1 });
        if (luck < 7) {
            loot.push_back({ "Combat Armor Leg", 1 });
            loot.push_back({ "Combat Armor Arm", 1 });
        }
        else {
            loot.push_back({ "Combat Armor Leg", 2 });
            loot.push_back({ "Combat Armor Arm", 2 });
        }
        loot.push_back({ "Combat Rifle", 1 });
        loot.push_back({ ".45 Ammunition", static_cast<int>(applyDeviation(luck * 2)) });
        capsDist = uniform_int_distribution<>(0, 4 * luck);
        loot.push_back({ "Caps", capsDist(gen) });
        break;
    case 3: // Trader / Caravan Merchant
        loot.push_back({ "Drifter Outfit", 1 });
        loot.push_back({ "10mm Auto Pistol", 1 });
        dcResult = rollCombatDice();
        loot.push_back({ "10mm Ammunition", luck * dcResult.result });
        capsDist = uniform_int_distribution<>(30, 12 * luck);
        loot.push_back({ "Caps", capsDist(gen) });
        break;
    case 4: // Vault Dweller
        loot.push_back({ "Vault Jumpsuit", 1 });
        loot.push_back({ "10mm pistol", 1 });
        ammoDist = uniform_int_distribution<>(0, 6 * luck);
        loot.push_back({ "10mm rounds", ammoDist(gen) });
        capsDist = uniform_int_distribution<>(0, 4 * luck);
        loot.push_back({ "Caps", capsDist(gen) });
        break;
    case 5: // Wastelander
        loot.push_back({ "Road Leathers", 1 });
        loot.push_back({ "Double-Barreled Shotgun", 1 });
        ammoDist = uniform_int_distribution<>(0, 2 * luck);
        loot.push_back({ "Shells", ammoDist(gen) });
        capsDist = uniform_int_distribution<>(0, 2 * luck);
        loot.push_back({ "Caps", capsDist(gen) });
        break;
    default:
        cout << "Invalid wastelander choice.\n";
        break;
    }

    return loot;
}


void displayLoot(const vector<Item>& loot) {
    // Create a map to combine item quantities
    map<string, int> combinedLoot;

    for (const auto& item : loot) {
        combinedLoot[item.name] += item.quantity;
    }

    // Display the combined loot
    for (const auto& entry : combinedLoot) {
        cout << entry.first << ": " << entry.second << endl;
    }
}

int main() {
    int inputNumber;
    int multiplier;
    string choice, searchType, placeCategory;
    char userDecision;

    cout << "Welcome to the Fallout 2d20 Loot Generator 0.2a!\n ";

    do {
        cout << "Please Choose what you're about to loot!\n ";
        do {
            cout << "Choose '1: Place' or '2: Body': ";
            cin >> choice;
            if (choice != "1" && choice != "2") {
                cout << "Invalid choice, please enter '1' for Place or '2' for Body.\n";
                clearCin();  // Clears any error flags that might be set if a non-integer is input
            }
        } while (choice != "1" && choice != "2");

        cout << "Enter the Luck Attribute of the Character: ";
        while (!(cin >> inputNumber)) {
            clearCin(); // Clear input buffer if input is invalid
            cout << "Invalid input for luck attribute. Please enter a valid number.\n";
        }

        if (choice == "1") {
            cout << "Select the category of the place:\n";
            cout << "1: Military\n";
            cout << "2: Residential\n";
            cout << "3: Commercial\n";
            cout << "4: Industry\n";
            cout << "5: Medical\n";
            cout << "6: Agriculture\n";
            cout << "Enter your choice: ";
            cin >> placeCategory;

            int placeSize;
            cout << "Select the size of the place:\n";
            cout << "1: Tiny\n";
            cout << "2: Small\n";
            cout << "3: Average\n";
            cout << "4: Large\n";
            cout << "Enter your choice: ";
            cin >> placeSize;

            // Define multipliers based on place type
            int placeSizeMultiplier;
            if (placeSize == 1) {
                placeSizeMultiplier = 1;
            }
            else if (placeSize == 2) {
                placeSizeMultiplier = 3;
            }
            else if (placeSize == 3) {
                placeSizeMultiplier = 5;
            }
            else if (placeSize == 4) {
                placeSizeMultiplier = 10;
            }
            else {
                cout << "Invalid place size choice, defaulting to Tiny.\n";
                placeSizeMultiplier = 1;
            }

            cout << "Select the type of search:\n";
            cout << "1: Untouched (5 Minutes of search)\n";
            cout << "2: Partly Searched (30 Minutes of Search)\n";
            cout << "3: Mostly Searched (1 Hours of Search)\n";
            cout << "4: Heavily Searched (4 Hours of Search)\n";
            cout << "Enter your search type: ";
            cin >> searchType;

            // Define multipliers based on search type
            if (searchType == "1") {
                multiplier = 1;
            }
            else if (searchType == "2") {
                multiplier = 3;
            }
            else if (searchType == "3") {
                multiplier = 5;
            }
            else if (searchType == "4") {
                multiplier = 10;
            }
            else {
                cout << "Invalid search type choice, defaulting to 1x multiplier.\n";
                multiplier = 1;
            }

            do {
                pair<vector<Item>, int> lootResult = generateLootForPlace(inputNumber, multiplier * placeSizeMultiplier, placeCategory);
                vector<Item> loot = lootResult.first;
                int totalLootValue = lootResult.second;

                displayLoot(loot);
                cout << "Total Loot Value: " << totalLootValue << endl;

                cout << "Press 'r' to regenerate, 'n' to start new, or 'e' to exit: ";
                cin >> userDecision;


                if (userDecision == 'e' || userDecision == 'E') {
                    return 0;  // Exit the program
                }
            } while (userDecision == 'r' || userDecision == 'R');
        }
        else if (choice == "2") {
            int bodyChoice;
            cout << "Select a body type:\n";
            cout << "1: Animals and Insects\n";
            cout << "2: Mutated Humanoids\n";
            cout << "3: Robots\n";
            cout << "4: Super Mutants\n";
            cout << "5: Synths\n";
            cout << "6: Turrets\n";
            cout << "7: Human\n";
            cout << "Enter your choice: ";
            cin >> bodyChoice;

            if (bodyChoice == 1) {
                int animalChoice;
                cout << "Select an animal:\n";
                cout << "1: Bloodbug\n";
                cout << "2: Bloatfly\n";
                cout << "3: Brahmin\n";
                cout << "4: Deathclaw\n";
                cout << "5: Mirelurk\n";
                cout << "6: Mirelurk Queen\n";
                cout << "7: Mole Rat\n";
                cout << "8: RadRoach\n";
                cout << "9: RadScorpion\n";
                cout << "10: Stingwing\n";
                cout << "Enter your choice: ";
                cin >> animalChoice;

                do {
                    vector<Item> loot = generateLootForAnimal(animalChoice, inputNumber);
                    displayLoot(loot);

                    cout << "Press 'r' to regenerate, 'n' to start new, or 'e' to exit: ";
                    cin >> userDecision;

                    if (userDecision == 'e' || userDecision == 'E') {
                        return 0;  // Exit the program
                    }
                } while (userDecision == 'r' || userDecision == 'R');

                continue; // Skip the remaining loop to go back to regeneration or start new
            }
            else if (bodyChoice == 2) {
                int humanoidChoice;
                cout << "Select a mutated humanoid:\n";
                cout << "1: Ghoul\n";
                cout << "2: The Glowing One\n";
                cout << "Enter your choice: ";
                cin >> humanoidChoice;

                do {
                    vector<Item> loot = generateLootForMutatedHumanoids(humanoidChoice, inputNumber);
                    displayLoot(loot);

                    cout << "Press 'r' to regenerate, 'n' to start new, or 'e' to exit: ";
                    cin >> userDecision;

                    if (userDecision == 'e' || userDecision == 'E') {
                        return 0;  // Exit the program
                    }
                } while (userDecision == 'r' || userDecision == 'R');

                continue; // Skip the remaining loop to go back to regeneration or start new
            }
            else if (bodyChoice == 3) {
                int robotChoice;
                cout << "Select a robot:\n";
                cout << "1: Assaultron\n";
                cout << "2: Eyebot\n";
                cout << "3: Mister Handy\n";
                cout << "4: Mister Gutsy\n";
                cout << "5: Protectron\n";
                cout << "6: Sentry Bot\n";
                cout << "Enter your choice: ";
                cin >> robotChoice;

                do {
                    vector<Item> loot = generateLootForRobots(robotChoice, inputNumber);
                    displayLoot(loot);

                    cout << "Press 'r' to regenerate, 'n' to start new, or 'e' to exit: ";
                    cin >> userDecision;

                    if (userDecision == 'e' || userDecision == 'E') {
                        return 0;  // Exit the program
                    }
                } while (userDecision == 'r' || userDecision == 'R');

                continue; // Skip the remaining loop to go back to regeneration or start new
            }
            else if (bodyChoice == 4) {
                int mutantChoice;
                cout << "Select a super mutant:\n";
                cout << "1: Super Mutant\n";
                cout << "2: Super Mutant Behemoth\n";
                cout << "Enter your choice: ";
                cin >> mutantChoice;

                do {
                    vector<Item> loot = generateLootForSuperMutants(mutantChoice, inputNumber);
                    displayLoot(loot);

                    cout << "Press 'r' to regenerate, 'n' to start new, or 'e' to exit: ";
                    cin >> userDecision;

                    if (userDecision == 'e' || userDecision == 'E') {
                        return 0;  // Exit the program
                    }
                } while (userDecision == 'r' || userDecision == 'R');

                continue; // Skip the remaining loop to go back to regeneration or start new
            }
            else if (bodyChoice == 5) {
                int synthChoice;
                cout << "Select a synth:\n";
                cout << "1: Synth\n";
                cout << "2: Synth Strider\n";
                cout << "3: Synth Trooper\n";
                cout << "Enter your choice: ";
                cin >> synthChoice;

                do {
                    vector<Item> loot = generateLootForSynths(synthChoice, inputNumber);
                    displayLoot(loot);

                    cout << "Press 'r' to regenerate, 'n' to start new, or 'e' to exit: ";
                    cin >> userDecision;

                    if (userDecision == 'e' || userDecision == 'E') {
                        return 0;  // Exit the program
                    }
                } while (userDecision == 'r' || userDecision == 'R');

                continue; // Skip the remaining loop to go back to regeneration or start new
            }
            else if (bodyChoice == 6) {
                int turretChoice;
                cout << "Select a turret:\n";
                cout << "1: Machine Gun Turret MK I\n";
                cout << "2: Machine Gun Turret MK III\n";
                cout << "3: Machine Gun Turret MK IV\n";
                cout << "Enter your choice: ";
                cin >> turretChoice;

                do {
                    vector<Item> loot = generateLootForTurrets(turretChoice, inputNumber);
                    displayLoot(loot);

                    cout << "Press 'r' to regenerate, 'n' to start new, or 'e' to exit: ";
                    cin >> userDecision;

                    if (userDecision == 'e' || userDecision == 'E') {
                        return 0;  // Exit the program
                    }
                } while (userDecision == 'r' || userDecision == 'R');

                continue; // Skip the remaining loop to go back to regeneration or start new
            }
            else if (bodyChoice == 7) {
                int humanChoice;
                cout << "Select a human faction:\n";
                cout << "1: Brotherhood of Steel\n";
                cout << "2: Raiders\n";
                cout << "3: Wastelanders\n";
                cout << "Enter your choice: ";
                cin >> humanChoice;

                if (humanChoice == 1) {
                    int brotherhoodChoice;
                    cout << "Select a Brotherhood of Steel member:\n";
                    cout << "1: Knight\n";
                    cout << "2: Paladin\n";
                    cout << "3: Initiate\n";
                    cout << "Enter your choice: ";
                    cin >> brotherhoodChoice;

                    do {
                        vector<Item> loot = generateLootForBrotherhood(brotherhoodChoice, inputNumber);
                        displayLoot(loot);

                        cout << "Press 'r' to regenerate, 'n' to start new, or 'e' to exit: ";
                        cin >> userDecision;

                        if (userDecision == 'e' || userDecision == 'E') {
                            return 0;  // Exit the program
                        }
                    } while (userDecision == 'r' || userDecision == 'R');

                    continue; // Skip the remaining loop to go back to regeneration or start new
                }
                else if (humanChoice == 2) {
                    int raiderChoice;
                    cout << "Select a Raider type:\n";
                    cout << "1: Raider\n";
                    cout << "2: Raider Boss\n";
                    cout << "3: Raider Psycho\n";
                    cout << "4: Raider Veteran\n";
                    cout << "Enter your choice: ";
                    cin >> raiderChoice;

                    do {
                        vector<Item> loot = generateLootForRaiders(raiderChoice, inputNumber);
                        displayLoot(loot);

                        cout << "Press 'r' to regenerate, 'n' to start new, or 'e' to exit: ";
                        cin >> userDecision;

                        if (userDecision == 'e' || userDecision == 'E') {
                            return 0;  // Exit the program
                        }
                    } while (userDecision == 'r' || userDecision == 'R');

                    continue; // Skip the remaining loop to go back to regeneration or start new
                }
                else if (humanChoice == 3) {
                    int wastelanderChoice;
                    cout << "Select a Wastelander type:\n";
                    cout << "1: Gunner\n";
                    cout << "2: Mercenary\n";
                    cout << "3: Trader / Caravan Merchant\n";
                    cout << "4: Vault Dweller\n";
                    cout << "5: Wastelander\n";
                    cout << "Enter your choice: ";
                    cin >> wastelanderChoice;

                    do {
                        vector<Item> loot = generateLootForWastelanders(wastelanderChoice, inputNumber);
                        displayLoot(loot);

                        cout << "Press 'r' to regenerate, 'n' to start new, or 'e' to exit: ";
                        cin >> userDecision;

                        if (userDecision == 'e' || userDecision == 'E') {
                            return 0;  // Exit the program
                        }
                    } while (userDecision == 'r' || userDecision == 'R');

                    continue; // Skip the remaining loop to go back to regeneration or start new
                }

            }

        }

    } while (userDecision == 'n' || userDecision == 'N');

    return 0;
}