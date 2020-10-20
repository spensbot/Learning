#include <iostream>

#include <string>
#include <vector>
#include <numeric>

/**
 A comparable app version
 Requires input string of format "1.11.111" with any depth
 depth = number of sub-versions -> e.g. depth of 1.1.1 = 3;
 */
class AppVersion {
public:
    /**
      maxDepth represents the maximum number of sub-versions to parse
      e.g. depth of 1.1 = 2  |  depth of 1.1.1.1 = 4
      */
    AppVersion(const std::string& fromString, int maxDepth) {
        parsed = parseString(fromString, maxDepth);
    }

    int depth() const {
        return parsed.size();
    }
    
    bool operator > (const AppVersion& other) {
        Comparison comparison = compareWith(other);
        return comparison == Comparison::greater;
    }

    bool operator == (const AppVersion& other) {
        Comparison comparison = compareWith(other);
        return comparison == Comparison::equal;
    }

    bool operator != (const AppVersion& other) {
        Comparison comparison = compareWith(other);
        return comparison != Comparison::equal;
    }

    bool operator < (const AppVersion& other) {
        Comparison comparison = compareWith(other);
        return comparison == Comparison::less;
    }

    bool operator >= (const AppVersion& other) {
        Comparison comparison = compareWith(other);
        return comparison == Comparison::greater
            || comparison == Comparison::equal;
    }

    bool operator <= (const AppVersion& other) {
        Comparison comparison = compareWith(other);
        return comparison == Comparison::less
            || comparison == Comparison::equal;
    }

    std::string toString() {
        std::string str = std::reduce(parsed.begin(), parsed.end(), std::string(""), [](std::string accumulator, int value){
            return accumulator + std::to_string(value) + ".";
        });
        str.pop_back();
        return str;
    }
    
private:
    std::vector<int> parsed;

    enum class Comparison {
        less,
        greater,
        equal
    };

    Comparison compareWith(const AppVersion& other) {
        if (depth() != other.depth()){
            // Assert(false) comparing versions with varying depth
            // Version results beyond 
        }
        int minDepth = std::min(depth(), other.depth());

        for (int i=0 ; i<minDepth ; i++) {
            if (parsed[i] > other.parsed[i]) {
                return Comparison::greater;
            }
            if (parsed[i] < other.parsed[i]) {
                return Comparison::less;
            }
        }
        return Comparison::equal;
    } 

    int stoiSafe(std::string s) {
        try {
            return std::stoi(s);
        } catch(...) {
            // ASSERT(false); //Invalid version format
            return 0;
        }
    }

    std::vector<int> parseString(const std::string& fromString, int maxDepth) {
        std::vector<int> parsedInts;
        std::string subVersion = "";
        
        for (auto char_ : fromString) {
            if (char_ != '.') {
                subVersion += char_;
            } else { // '.'
                parsedInts.push_back(stoiSafe(subVersion));
                subVersion = "";
                if (parsedInts.size() == maxDepth)
                    break;
            }
        }
        if (parsedInts.size() < maxDepth) {
            parsedInts.push_back(stoiSafe(subVersion));
        }
        
        return parsedInts;
    }
};



// operator <= uses <, >, and == under the hood
void lessOrEqualTest(AppVersion left, AppVersion right, bool expected) {
    std::cout << left.toString() << " <= " << right.toString() << ": ";
    bool result = left <= right;
    std::cout << (result ? "true" : "false") << (result == expected ? "" : " - FAILED!!!!") << std::endl;
}

int main() {
    lessOrEqualTest(AppVersion("1.1.1.999", 3), AppVersion("1.1.1", 3), true);
    lessOrEqualTest(AppVersion("2.1", 3), AppVersion("1.2", 3), false);
    lessOrEqualTest(AppVersion("1.1", 3), AppVersion("2.1", 3), true);
    lessOrEqualTest(AppVersion("1.1.1.2", 4), AppVersion("1.1", 2), true);
    lessOrEqualTest(AppVersion("1.1", 2), AppVersion("1.1.1.1", 4), true);
    lessOrEqualTest(AppVersion("110.111.112", 3), AppVersion("110.111.111", 3), false);
    lessOrEqualTest(AppVersion("100.111.112", 3), AppVersion("110.111.111", 3), true);
    lessOrEqualTest(AppVersion("", 3), AppVersion("1", 3), true);
}