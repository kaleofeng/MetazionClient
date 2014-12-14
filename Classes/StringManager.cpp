#include "StringManager.h"

#include <string>
#include <sstream>

#include <platform/CCFileUtils.h>

#include "Xml/rapidxml.hpp"
#include "Xml/rapidxml_utils.hpp"

USING_NS_CC;

StringManager::StringManager() {}

StringManager::~StringManager() {}

StringManager& StringManager::Instance() {
    static StringManager s_stringManager;
    return s_stringManager;
}

void StringManager::Initialize() {
    LoadAllString();
}

void StringManager::Finalize() {
    m_stringMap.clear();
}

const std::string& StringManager::GetString(int id) {
    return m_stringMap[id];
}

void StringManager::LoadAllString() {
    rapidxml::file<> file("String/UI.xml");
    auto data = file.data();

    rapidxml::xml_document<> doc;
    doc.parse<0>(data);
    auto docName = doc.name();

    rapidxml::xml_node<>* root = doc.first_node();
    auto rootName = root->name();

    for (auto node = root->first_node("String"); node != NULL
        ; node = node->next_sibling()) {
        auto idAttr = node->first_attribute("id");
        auto contentAttr = node->first_attribute("content");
        auto szId = idAttr->value();
        auto szContent = contentAttr->value();

        auto id = atoi(szId);
        m_stringMap[id] = szContent;
    }
}