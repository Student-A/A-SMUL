#ifndef A_COMPONENT_HPP
#define A_COMPONENT_HPP

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

namespace A
{
  class MainComponent;
  class SubComponent;
  class IdComponent;

  class Component
  {
  public:
    Component(std::string name);

    Component();

    ~Component();

    Component &getComponent(int index);

    Tag &getTag(int index);

    Tag &getTag(std::string name);

    void addTag(Tag tag);

    void addMainComponent(std::shared_ptr<MainComponent> mainComponent);

    void addSubComponent(std::shared_ptr<SubComponent> subComponent);

    void addIdComponent(std::shared_ptr<IdComponent> idComponent);
    
  protected:
    
    std::string _name{""};
    
    std::unordered_map<std::string, std::vector<std::unique_ptr<MainComponent>>> _mainComponents;p
    std::unordered_map<std::string, std::vector<std::unique_ptr<SubComponent>>> _subComponents;
    std::unordered_map<std::string, std::vector<std::unique_ptr<IdComponent>>> _idComponents;

    std::unordered_map<std::string, std::vector<Tag>> _tags;
    
    std::vector<std::unique_ptr<Component>> _orderedComponents;
    std::vector<Tag> _orderedTags;
    //std::vector<std::shared_ptr<MarkupElement>> _orderedElements;
  };
}

#endif // A_COMPONENT_HPP
