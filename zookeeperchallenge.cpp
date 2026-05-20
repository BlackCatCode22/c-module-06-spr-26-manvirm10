#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <iomanip>
#include <stdexcept>
#include <memory>
#include <utility>
#include <algorithm>

using namespace std;

class Animal {
protected:
    string id;
    string name;
    int age;
    string sex;
    string species;
    string color;
    int weight;
    string origin;
    string birthDate;
    string arrivalDate;

public:
    Animal(int age, string sex, string species, string color, int weight, string origin, string birthDate, string arrivalDate)
        : age(age), sex(std::move(sex)), species(std::move(species)), color(std::move(color)),
          weight(weight), origin(std::move(origin)), birthDate(std::move(birthDate)), arrivalDate(std::move(arrivalDate)) {}

    virtual ~Animal() = default;

    void setID(string newID) { id = std::move(newID); }
    void setName(string newName) { name = std::move(newName); }

    [[nodiscard]] string getSpecies() const { return species; }

    [[nodiscard]] string toString() const {
        stringstream ss;
        ss << id << "; " << name << "; birth date " << birthDate << "; "
           << color << " color; " << sex << "; " << weight << " pounds; from "
           << origin << "; arrived " << arrivalDate;
        return ss.str();
    }
};

class Hyena : public Animal {
public:
    Hyena(int age, string sex, string color, int weight, string origin, string birthDate, string arrivalDate)
        : Animal(age, std::move(sex), "hyena", std::move(color), weight, std::move(origin), std::move(birthDate), std::move(arrivalDate)) {}
};

class Lion : public Animal {
public:
    Lion(int age, string sex, string color, int weight, string origin, string birthDate, string arrivalDate)
        : Animal(age, std::move(sex), "lion", std::move(color), weight, std::move(origin), std::move(birthDate), std::move(arrivalDate)) {}
};

class Tiger : public Animal {
public:
    Tiger(int age, string sex, string color, int weight, string origin, string birthDate, string arrivalDate)
        : Animal(age, std::move(sex), "tiger", std::move(color), weight, std::move(origin), std::move(birthDate), std::move(arrivalDate)) {}
};

class Bear : public Animal {
public:
    Bear(int age, string sex, string color, int weight, string origin, string birthDate, string arrivalDate)
        : Animal(age, std::move(sex), "bear", std::move(color), weight, std::move(origin), std::move(birthDate), std::move(arrivalDate)) {}
};

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

string genBirthDay(int age, const string& season) {
    int birthYear = 2024 - age;
    string monthDay;

    if (season.find("spring") != string::npos) monthDay = "-03-15";
    else if (season.find("summer") != string::npos) monthDay = "-06-15";
    else if (season.find("fall") != string::npos) monthDay = "-09-15";
    else if (season.find("winter") != string::npos) monthDay = "-12-15";
    else monthDay = "-01-01";

    return to_string(birthYear) + monthDay;
}

map<string, int> speciesCounts;
string genUniqueID(const string& species) {
    speciesCounts[species]++;
    string prefix;

    if (species == "hyena") prefix = "Hy";
    else if (species == "lion") prefix = "Li";
    else if (species == "tiger") prefix = "Ti";
    else if (species == "bear") prefix = "Be";

    stringstream ss;
    ss << prefix << setfill('0') << setw(2) << speciesCounts[species];
    return ss.str();
}

int main() {
    map<string, queue<string>> animalNames;
    vector<unique_ptr<Animal>> zooPopulation;
    string arrivalDate = "2024-03-05";

    try {
        ifstream nameFile("animalNames.txt");
        if (!nameFile) throw runtime_error("Could not open animalNames.txt");

        string line, currentSpecies;
        while (getline(nameFile, line)) {
            line = trim(line);
            if (line.empty()) continue;

            string lowerLine = line;
            for(char &c : lowerLine) {
                c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
            }

            if (lowerLine.find("hyena") != string::npos) currentSpecies = "hyena";
            else if (lowerLine.find("lion") != string::npos) currentSpecies = "lion";
            else if (lowerLine.find("bear") != string::npos) currentSpecies = "bear";
            else if (lowerLine.find("tiger") != string::npos) currentSpecies = "tiger";
            else {
                vector<string> names = split(line, ',');
                for (const string& n : names) {
                    animalNames[currentSpecies].push(n);
                }
            }
        }
        nameFile.close();
    } catch (const exception& e) {
        cerr << "Exception loading names: " << e.what() << endl;
        return 1;
    }

    try {
        ifstream animalFile("arrivingAnimals.txt");
        if (!animalFile) throw runtime_error("Could not open arrivingAnimals.txt");

        string line;
        while (getline(animalFile, line)) {
            line = trim(line);
            if (line.empty()) continue;

            vector<string> tokens = split(line, ',');

            stringstream ss0(tokens[0]);
            string ageStr, yearStr, oldStr, sex, species;
            ss0 >> ageStr >> yearStr >> oldStr >> sex >> species;
            int age = stoi(ageStr);

            string birthDate = genBirthDay(age, tokens[1]);

            string color = tokens[2];
            size_t colorPos = color.find(" color");
            if (colorPos != string::npos) {
                color = color.substr(0, colorPos);
            }

            stringstream ss3(tokens[3]);
            string weightStr;
            ss3 >> weightStr;
            int weight = stoi(weightStr);

            string origin = tokens[4];
            for (size_t i = 5; i < tokens.size(); i++) {
                origin += ", " + tokens[i];
            }
            if (origin.find("from ") == 0) {
                origin = origin.substr(5);
            }

            unique_ptr<Animal> newAnimal = nullptr;
            if (species == "hyena") newAnimal = make_unique<Hyena>(age, sex, color, weight, origin, birthDate, arrivalDate);
            else if (species == "lion") newAnimal = make_unique<Lion>(age, sex, color, weight, origin, birthDate, arrivalDate);
            else if (species == "tiger") newAnimal = make_unique<Tiger>(age, sex, color, weight, origin, birthDate, arrivalDate);
            else if (species == "bear") newAnimal = make_unique<Bear>(age, sex, color, weight, origin, birthDate, arrivalDate);

            if (newAnimal) {
                newAnimal->setID(genUniqueID(species));

                if (!animalNames[species].empty()) {
                    newAnimal->setName(animalNames[species].front());
                    animalNames[species].pop();
                } else {
                    newAnimal->setName("Unnamed");
                }

                zooPopulation.push_back(std::move(newAnimal));
            }
        }
        animalFile.close();
    } catch (const exception& e) {
        cerr << "Exception processing animals: " << e.what() << endl;
        return 1;
    }

    try {
        ofstream report("zooPopulation.txt");
        if (!report) throw runtime_error("Could not create zooPopulation.txt");

        vector<string> habitats = {"hyena", "lion", "tiger", "bear"};

        for (const string& habitat : habitats) {
            string header = habitat;
            header[0] = static_cast<char>(toupper(static_cast<unsigned char>(header[0])));
            report << header << " Habitat:\n";

            for (const auto& a : zooPopulation) {
                if (a->getSpecies() == habitat) {
                    report << a->toString() << "\n";
                }
            }
            report << "\n";
        }
        report.close();
        cout << "Successfully generated zooPopulation.txt" << endl;

    } catch (const exception& e) {
        cerr << "Exception writing report: " << e.what() << endl;
        return 1;
    }

    return 0;
}