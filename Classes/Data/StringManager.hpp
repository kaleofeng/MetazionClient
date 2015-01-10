#ifndef _CLIENT_STRINGMANAGER_HPP_
#define _CLIENT_STRINGMANAGER_HPP_

#include <map>
#include <string>

class StringManager {
    using StringMap_t = std::map<int, std::string>;

public:
    StringManager();

    ~StringManager();

    static StringManager& Instance();

public:
    void Initialize();

    void Finalize();

    const std::string& GetString(int id);

private:
    void LoadAllString();

private:
    StringMap_t m_stringMap;
};

#endif // _CLIENT_STRINGMANAGER_HPP_
