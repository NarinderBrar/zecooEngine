#include "UI.h"
UI::UI()
{
  
}

void UI::Update()
{

}

void UI::Render()
{
	ImGui::Begin( "Hello, world!" );
	ImGui::Text("Hi");

	if( ImGui::Button( "Button" ) )
	{
		v = true;
	}
	ImGui::ColorEdit3( "color", clr);
	ImGui::SliderFloat( "Y", &angle, 0.0, 5.0 );
	ImGui::End();
}

UI::~UI()
{

}