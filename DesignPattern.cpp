#include "main.h"

#include <memory>

namespace DesignPattern {

namespace FactoryMethod
{
  struct Leadership
  {
    virtual std::string hasIdea() = 0;
  };

  struct NoleMusk : Leadership { std::string hasIdea() override { return "Space Program"; } };
  struct LalaLama : Leadership { std::string hasIdea() override { return "Peace and Love"; } };

  // a complicated world
  struct World
  {
    void developCulture(std::string idea) { /* .... */ }
    void embraceNature(std::string idea)  { /* .... */ }
    void everyoneCounts(std::string idea) { /* ... */ }
    std::string futureBasedOn()
    {
      auto idea = withLeadership()->hasIdea();
      developCulture(idea);
      embraceNature(idea);
      everyoneCounts(idea);
      return idea;
    }

  protected:
    // this is our "factory method" to produce our leadership value
    virtual std::unique_ptr<Leadership> withLeadership() = 0;
  };

  // an identical world but with just different leadership
  struct Apocalypse : World
  {
    std::unique_ptr<Leadership> withLeadership() override
    {
      return std::make_unique<NoleMusk>();
    }
  };

  // another identical world also with different leadership
  struct Utopia : World
  {
    std::unique_ptr<Leadership> withLeadership() override
    {
      return std::make_unique<LalaLama>();
    }
  };
}

namespace AbstractFactory
{
  // While an abstract factory with abstract components are defined,
  // concrete factories and concrete components
  // can represent a product portfolio in a matrix form (orthogonal dimensions)
  struct ComputerFactory
  {
    struct Display   { virtual int price() = 0; };
    struct Keyboard  { virtual int price() = 0; };
    struct Mainboard { virtual int price() = 0; };

    virtual std::unique_ptr<Display>   buildDisplay() = 0;
    virtual std::unique_ptr<Keyboard>  buildKeyboard() = 0;
    virtual std::unique_ptr<Mainboard> buildMainboard() = 0;

    int price()
    {
      return buildDisplay()->price() +
             buildKeyboard()->price() +
             buildMainboard()->price();
    }
  };

  struct HighEndComputerManufacturer : ComputerFactory
  {
    struct FourKDisplay : ComputerFactory::Display
    { int price() override {return 3000;} };
    struct WirelessGamingKeyboard : ComputerFactory::Keyboard
    { int price() override {return 100;} };
    struct MultiCoreCPUDedicatedGPUMainboard : ComputerFactory::Mainboard
    { int price() override {return 1000;} };

    virtual std::unique_ptr<Display> buildDisplay()     { return std::make_unique<FourKDisplay>(); }
    virtual std::unique_ptr<Keyboard> buildKeyboard()   { return std::make_unique<WirelessGamingKeyboard>(); }
    virtual std::unique_ptr<Mainboard> buildMainboard() { return std::make_unique<MultiCoreCPUDedicatedGPUMainboard>(); }
  };

  struct SustainableComputerMaker : ComputerFactory
  {
    struct OfficeHDMonitor : ComputerFactory::Display
    { int price() override {return 100;} };
    struct OldGoodMechanicalKeyboard : ComputerFactory::Keyboard
    { int price() override {return 10;} };
    struct GoodEnoughMainboard : ComputerFactory::Mainboard
    { int price() override {return 300;} };

    virtual std::unique_ptr<Display> buildDisplay()     { return std::make_unique<OfficeHDMonitor>(); }
    virtual std::unique_ptr<Keyboard> buildKeyboard()   { return std::make_unique<OldGoodMechanicalKeyboard>(); }
    virtual std::unique_ptr<Mainboard> buildMainboard() { return std::make_unique<GoodEnoughMainboard>(); }
  };
}

namespace Builder
{
  struct RealHouse {};
  struct ToyHouse {};

  template<typename T>
  struct Builder
  {
    virtual void buildWall() = 0;
    virtual void buildRoof() = 0;
    virtual void buildWindow() = 0;
    virtual T deliverdResult() = 0;
  };

  struct ConstructionCompany : Builder<RealHouse>
  {
    void buildWall() override { /* build real wall */ }
    void buildRoof() override { /* build real roof */ }
    void buildWindow() override { /* mount real window */ }
    RealHouse deliverdResult() override
    {
      return RealHouse{};
    }
  };

  struct Kid : Builder<ToyHouse>
  {
    void buildWall() override { /* paper wall */ }
    void buildRoof() override { /* paper roof */ }
    void buildWindow() override { /* make some holes in the paper walls */ }
    ToyHouse deliverdResult() override
    {
      return ToyHouse();
    }
  };

  struct Director
  {
    // a director issue same commands regardless associated builders
    // the actual product depends on builders.
    template<typename B>
    void buildAHouseBy(B& builder)
    {
      builder.buildWall();
      builder.buildWindow();
      builder.buildRoof();
    }
  };
}

namespace Prototype
{
  // nothing more than clonable interface
  template<typename T>
  struct Thing
  {
    virtual T clone() = 0;
  };

  struct Alien : Thing<Alien>
  {
    Alien clone() override
    {
      return Alien(this->intelligence);
    }

    explicit Alien(int intelligence) : intelligence(intelligence) {}

    int intelligence;
  };
}

namespace Singleton
{
  // only one instance possible
  struct SantaClaus
  {
    static SantaClaus& hohoho()
    {
      static SantaClaus he;
      return he;
    }
  private:
    SantaClaus() = default;
    ~SantaClaus() = default;
    SantaClaus(const SantaClaus&) = delete;
    SantaClaus& operator=(const SantaClaus&) = delete;
  };

  struct Kid
  {
    SantaClaus& makeChrismasWishToWhom() { return SantaClaus::hohoho(); }
  };
}

namespace Adapter
{
  // TODO:
}

namespace Bridge
{
  // Decouple different dimentions into separate class hierarchy with abstract base one.
  // Then various concrete implementation can be composed to fufill desired purpose.

  // any person
  struct Role
  {
    // common buisness act of a role
    virtual void takeRisk() = 0;
    virtual void decide() = 0;
    virtual void discuss() = 0;
  };

  // any company
  struct Company
  {
    Company(Role& ceo) : ceo(ceo) {}

    // common company operations
    virtual void makeDecision() = 0;
    virtual void developStrategy() = 0;

  protected:
    Role& ceo;
  };

  // an actual company
  struct StartUpCompany : Company
  {
    StartUpCompany(Role& ceo) : Company(ceo) {}

    void makeDecision() override
    {
      /* The CEO makes quick decision. Sometimes it could be risky one.  */
      ceo.takeRisk();
      ceo.decide();
    }
    void developStrategy() override
    {
      /* The CEO identifies market niche and targets the profitable but manageable use cases */
      ceo.takeRisk();
    }
  };

  // another actual company
  struct LargeCompany : Company
  {
    LargeCompany(Role& ceo) : Company(ceo) {}

    void makeDecision() override
    {
      /* The CEO makes decision after intensive discussion with several key persons.  */
      ceo.discuss();
      ceo.decide();
    }
    void developStrategy() override
    {
      /* more discussion */
      ceo.discuss();
    }
  };

  // an actual role
  struct RecurrentFounder : Role
  {
    void takeRisk() override { /* no risk no fun */ }
    void decide() override   { /* prompt decision */ }
    void discuss() override  { /* */ }
  };

  // another actual role
  struct TraditionalManager : Role
  {
    void takeRisk() override { /* take minimal risk */ }
    void decide() override   { /* deliberate decision */ }
    void discuss() override  {}
  };
}

namespace Composite
{
  // TODO:
}

int main() {

  // FactoryMethod
  {
    using namespace FactoryMethod;
    // Our world is an extrme complicated and complex system
    // It is difficult to change every aspect in order to make a better one.
    // sometime we can just change a little bit of our value about the leadership....
    std::unique_ptr<World> world = std::make_unique<Apocalypse>();
    std::unique_ptr<World> anotherWorld = std::make_unique<Utopia>();
    CHECK("future", (world->futureBasedOn() !=  anotherWorld->futureBasedOn()));
  }

  // AbstractFactory
  {
    using namespace AbstractFactory;
    // you want to order a computer
    // which is either built by a high-end computer manufacturer
    HighEndComputerManufacturer fancy;
    // or by a company building cheap but sustainable elements
    SustainableComputerMaker sustainable;
    // they provide the same list of sub-components
    // but with different spec, capacity and price
    CHECK("price", (fancy.price() > sustainable.price()));
  }

  // Builder
  {
    using namespace Builder;
    // you as a client want to build a house
    Director you;
    // either by your kid who is just 4 years old
    Kid kid;
    you.buildAHouseBy(kid);
    // or by professioanl company
    ConstructionCompany professional;
    you.buildAHouseBy(professional);
    // even you have the identical instructions and wishes
    // you will get different thing as expected... hopfully :p
    ToyHouse toy = kid.deliverdResult();
    RealHouse house = professional.deliverdResult();
  }

  // Prototype
  {
    using namespace Prototype;
    // something complex which we have no knowlege about....
    Alien alien1{/* intelligence= */300};
    // and they can clone themselves
    auto alien2 = alien1.clone();
    // and clone another
    auto alien3 = alien2.clone();
    // and they are all indentical smart....
    CHECK_VALUE("clone", alien1.intelligence, alien2.intelligence);
    CHECK_VALUE("clone", alien3.intelligence, alien2.intelligence);
  }

  // Singleton
  {
    using namespace Singleton;
    // every boy
    Kid boy;
    // and every girl
    Kid girl;
    // makes a wish list to his or her Santa..
    SantaClaus& hisSanta = boy.makeChrismasWishToWhom();
    SantaClaus& herSanta = girl.makeChrismasWishToWhom();
    // miraculously there is only one true santa who will bring gifts to every kid :)
    CHECK_VALUE("Hohoho", &hisSanta, &herSanta);
  }

  // Bridge
  {
    using namespace Bridge;
    // there are different types of CEO
    TraditionalManager oldGoodManager;
    RecurrentFounder youngFounder;
    // there are different types of companies
    // how about swap the role and see how they work...
    StartUpCompany startup(oldGoodManager);
    LargeCompany conventionalFirm(youngFounder);
  }
  return 0;
}
}