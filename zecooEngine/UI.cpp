#include "UI.h"
UI::UI()
{
  
}

void UI::Update()
{

}

static void HelpMarker( const char* desc )
{
    ImGui::TextDisabled( "(?)" );
    if( ImGui::IsItemHovered() )
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos( ImGui::GetFontSize() * 35.0f );
        ImGui::TextUnformatted( desc );
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void UI::ShowUserGuide()
{
    if( ImGui::CollapsingHeader( "Help" ) )
    {
        ImGui::Text( "ABOUT THIS DEMO:" );
        ImGui::BulletText( "Sections below are demonstrating many aspects of the library." );
        ImGui::BulletText( "The \"Examples\" menu above leads to more demo contents." );
        ImGui::BulletText( "The \"Tools\" menu above gives access to: About Box, Style Editor,\n"
                           "and Metrics/Debugger (general purpose Dear ImGui debugging tool)." );

        ImGui::Separator();
        

        ImGui::Text( "PROGRAMMER GUIDE:" );
        ImGui::BulletText( "See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!" );
        ImGui::BulletText( "See comments in imgui.cpp." );

        ImGui::BulletText( "See example applications in the examples/ folder." );
        ImGui::BulletText( "Read the FAQ at http://www.dearimgui.org/faq/" );
        ImGui::BulletText( "Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls." );
        ImGui::BulletText( "Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls." );
        ImGui::Separator();

    }
}

void UI::Render()
{
	ImGui::Begin( "Hello, world!" );

    if( ImGui::BeginMenu( "Examples" ) )
    {
        ImGui::MenuItem( "Main menu bar", NULL, &show_app_main_menu_bar );
        ImGui::MenuItem( "Console", NULL, &show_app_console );
        ImGui::MenuItem( "Log", NULL, &show_app_log );
        ImGui::MenuItem( "Simple layout", NULL, &show_app_layout );

        ImGui::BeginChild( "child", ImVec2( 0, 120 ), true );
        for( int i = 0; i < 10; i++ )
            ImGui::MenuItem( "Simple layout", NULL, &show_app_layout );
        ImGui::EndChild();

        ImGui::EndMenu();
    }

	if( ImGui::BeginMenu( "Colors" ) )
	{
		float sz = ImGui::GetTextLineHeight();
		for( int i = 0; i < ImGuiCol_COUNT; i++ )
		{
			const char* name = ImGui::GetStyleColorName( (ImGuiCol)i );
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled( p, ImVec2( p.x + sz, p.y + sz ), ImGui::GetColorU32( (ImGuiCol)i ) );
			ImGui::Dummy( ImVec2( sz, sz ) );
			ImGui::SameLine();
			ImGui::MenuItem( name );
		}
		ImGui::EndMenu();
	}

	ImGui::Checkbox( "MyCheckbox", &b );

    ImGui::InputFloat( "Input", &f, 0.1f );
    ImGui::SameLine(); HelpMarker( "Enable keyboard controls." );

	if( ImGui::MenuItem( "Quit", "Alt+F4" ) )
	{
		cout << "Quit the App" << endl;
	}

    if( ImGui::CollapsingHeader( "Window options" ) )
    {
        if( ImGui::BeginTable( "split", 4 ) )
        {
            ImGui::TableNextColumn(); ImGui::Checkbox( "No titlebar", &b );
            ImGui::TableNextColumn(); ImGui::Checkbox( "No scrollbar", &b );
            ImGui::TableNextColumn(); ImGui::Checkbox( "No menu", &b );
            ImGui::TableNextColumn(); ImGui::Checkbox( "No move", &b );
            ImGui::TableNextColumn(); ImGui::Checkbox( "No resize", &b );
            ImGui::TableNextColumn(); ImGui::Checkbox( "No collapse", &b );
            ImGui::TableNextColumn(); ImGui::Checkbox( "No close", &b );
            ImGui::TableNextColumn(); ImGui::Checkbox( "No nav", &b );
            ImGui::TableNextColumn(); ImGui::Checkbox( "No background", &b );
            ImGui::TableNextColumn(); ImGui::Checkbox( "No bring to front", &b );
            ImGui::TableNextColumn(); ImGui::Checkbox( "Unsaved document", &b );
            ImGui::EndTable();
        }
        k = 0;
    }
    else
    {
        ImGui::RadioButton( "radio c", &k, 2 );
    }

    static float f1 = 1.00f, f2 = 0.0067f;
    ImGui::DragFloat( "drag float", &f1, 0.005f );

	ImGui::End();
}

UI::~UI()
{

}