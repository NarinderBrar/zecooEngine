#include "UI.h"
UI::UI()
{
  
}

void UI::Update()
{

}

void UI::Render()
{
    ImGui::Begin( "Triangle Position/Color" );
    ImGui::SliderFloat( "rotation", &rotation, 0, 2 * 3.14f );
    ImGui::SliderFloat( "position", &translation, -10.0, 10.0 );
    ImGui::ColorEdit3( "color", color );
    ImGui::Button( "Hello!" );
    ImGui::End();
}

UI::~UI()
{

}