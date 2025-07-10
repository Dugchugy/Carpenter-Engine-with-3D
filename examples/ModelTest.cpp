/**
 * @file SoundTest.cpp
 * @brief Your basic spinning cube test 
 * @author: Roberto Selles
 */

#include <Game.hpp>
#include <UI/UILabel.hpp>
#include <Graphics/Shapes.hpp>
#include <Assets/Model.hpp>

using namespace Engine;

class ExampleScene : public Scene {
  private:
  float time = 0;
  UI::UILabel label;
  Assets::Model stlTest;
  Assets::Model objTest;
  public:
  ExampleScene() : 
      Scene("ExampleScene"),
      label("Label", "Model Test") {
    AddChild(&label);
    stlTest = Assets::loadStlModel( "Assets/test-monkey.stl" );
    objTest = Assets::loadObjModel( "Assets/test-monkey.obj" );
  }

  void Draw() override {
    Scene::Draw();

    stlTest.draw( Game::getInstance().GetRenderer(), 
                  { 1, 0, 10 },
                  { 90, time, 0 },
                  { 1, 1, 1 } );
    objTest.draw( Game::getInstance().GetRenderer(), 
                  { -1, 0, 10 },
                  { 0, time, 0 },
                  { 1, 1, 1 } );
  }

  void Update(float dt) override {
    time += 2 * dt;
  }
};

Game& game{Game::getInstance(new ExampleScene())};
