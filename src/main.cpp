#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <functional>
#include "Core.h" // custom classes and functions (like Input, Button, etc.)
#include "Signal.h"
#include "SignalRenderer.h"
#include "Components.h"
#include "Components/ComponentRenderer.h"
#include "JSON/FileManager.h"

#define SAMPLERATE 48000

#define DATADIR string("../../../data/")

using namespace std;

int main()
{
	string title = "Sound Generator";

	Window::Create(sf::VideoMode(1280, 720), title);

	// Load font
	sf::Font font;
	if (!font.loadFromFile(DATADIR + "font/consola.ttf"))
	{
		printf("Error when loading font");
	}

	sf::Color lightGrey(200, 200, 200);
	sf::Color darkGrey(80, 80, 80);

	UIStyle style;
	style.setFont(font, 16);
	style.mapStyle(UI_NORMAL, lightGrey, sf::Color::Black, lightGrey, 1.0f);
	style.mapStyle(UI_HOVERED, lightGrey, sf::Color(50, 50, 50), lightGrey, 3.0f);
	style.mapStyle(UI_CLICKED, sf::Color::Black, lightGrey, lightGrey, 1.0f);
	style.mapStyle(UI_DISABLED, darkGrey, sf::Color::Black, darkGrey, 1.0f);
	style.mapStyle(UI_FOCUSED, lightGrey, sf::Color::Blue, lightGrey, 1.0f);


	// Button Example
	Button button(10, -10, 180, 30, style);
	button.setText("Generate");
	button.setMargins(10, 10, 10, 10);
	button.setAnchorMin(sf::Vector2f(0.0f, 1.0f));
	button.setAnchorMax(sf::Vector2f(0.0f, 1.0f));
	button.setPivot(sf::Vector2f(0.0f, 1.0f));


	Label labelDuration(0, 0, 100, 30, style);
	labelDuration.setText("Duration");
	labelDuration.setAlignement(TextAlign::ALIGN_RIGHT);


	string compList[10] =
	{
		"Sinusoidal",
		"Square",
		"Triangle",
		"Saw Tooth",
		"Random",
		"Add",
		"Multiply",
		"Repeat",
		"Delay",
		"ADSR"
	};

	VerticalLayout vLayout(0, 100, 200, 200);
	vLayout.setAnchorMin(sf::Vector2f(0, 0));
	vLayout.setAnchorMax(sf::Vector2f(0, 1));
	vLayout.setMargins(10, 10, 170, 10);
	vLayout.setSpacing(10);
	vLayout.setPaddings(10, 10, 0, 0);

	vector<Button> compBtns;
	compBtns.resize(10, Button(0, 0, 0, 30, style));
	for (int i = 0; i < compBtns.size(); i++)
	{
		compBtns[i].setText(compList[i]);
		vLayout.add(compBtns[i]);
	}

	HorizontalLayout toolLayout(10, 10, 0, 30);
	toolLayout.setAnchorMin(sf::Vector2f(0, 0));
	toolLayout.setAnchorMax(sf::Vector2f(1, 0));
	toolLayout.setMargins(10, 10, 10, 10);
	toolLayout.setSpacing(10);

	// View
	View view(0, 0, 0, 0, style);
	view.setAnchorMin(sf::Vector2f(0, 0));
	view.setAnchorMax(sf::Vector2f(1, 1));
	view.setMargins(200, 10, 170, 10);

	// Window layout
	Layout rootLayout(0, 0, Window::GetWidth(), Window::GetHeight());
	Window::SetLayout(rootLayout);
	rootLayout.add(button);
	rootLayout.add(vLayout);
	rootLayout.add(view);
	rootLayout.add(toolLayout);


	IconButton btnLoad(10, 10, 30, 30, style);
	btnLoad.setIconSize(32, 32);
	btnLoad.setIcon(DATADIR + "Images/icon_open.png");

	IconButton btnSave(10, 10, 30, 30, style);
	btnSave.setIconSize(32, 32);
	btnSave.setIcon(DATADIR + "Images/icon_save.png");

	IconButton btnStart(10, 10, 30, 30, style);
	btnStart.setIconSize(32, 32);
	btnStart.setIcon(DATADIR + "Images/icon_start.png");

	IconButton btnPlay(10, 10, 30, 30, style);
	btnPlay.setIconSize(32, 32);
	btnPlay.setIcon(DATADIR + "Images/icon_play.png");

	IconButton btnPause(10, 10, 30, 30, style);
	btnPause.setIconSize(32, 32);
	btnPause.setIcon(DATADIR + "Images/icon_pause.png");

	IconButton btnStop(10, 10, 30, 30, style);
	btnStop.setIconSize(32, 32);
	btnStop.setIcon(DATADIR + "Images/icon_stop.png");

	IconButton btnEnd(10, 10, 30, 30, style);
	btnEnd.setIconSize(32, 32);
	btnEnd.setIcon(DATADIR + "Images/icon_end.png");

	IconButton btnExport(10, 10, 30, 30, style);
	btnExport.setIconSize(32, 32);
	btnExport.setIcon(DATADIR + "Images/icon_export.png");

	Label labelVolume(0, 0, 100, 30, style);
	labelVolume.setText("Volume");
	labelVolume.setAlignement(TextAlign::ALIGN_RIGHT);

	// InputField Example
	InputField inputDuration(10, 90, 150, 30, style);
	inputDuration.setMaxLength(10);
	inputDuration.setFloat(1.0f);
	inputDuration.setPlaceholder("Test");

	InputField inputVolume(90, 50, 70, 30, style);
	inputVolume.setMaxLength(5);
	inputVolume.setFloat(20.0f);
	inputVolume.setPlaceholder("Volume");


	toolLayout.add(btnLoad);
	toolLayout.add(btnSave);
	toolLayout.add(btnExport);
	toolLayout.add(btnStart);
	toolLayout.add(btnPlay);
	toolLayout.add(btnPause);
	toolLayout.add(btnStop);
	toolLayout.add(btnEnd);
	toolLayout.add(labelVolume);
	toolLayout.add(inputVolume);
	toolLayout.add(labelDuration);
	toolLayout.add(inputDuration);


	// ============  Filename layout  ==============

	Label lblFilenameTitle(10, 10, 200, 30, style);
	lblFilenameTitle.setText("Filename");

	InputField inputFilename(10, 50, 200, 30, style);

	Button btnFilenameCancel(0, 0, 100, 30, style);
	btnFilenameCancel.setText("Cancel");

	Button btnFilenameApply(0, 0, 100, 30, style);
	btnFilenameApply.setAnchor(sf::Vector2f(1, 0));
	btnFilenameApply.setPivot(sf::Vector2f(1, 0));
	btnFilenameApply.setText("Apply");

	Layout layoutFilenameButtons(0, 0, 0, 30);
	layoutFilenameButtons.add(btnFilenameCancel);
	layoutFilenameButtons.add(btnFilenameApply);

	VerticalLayout vLayoutFilename(0, 0, 0, 150);
	vLayoutFilename.setAnchorMin(sf::Vector2f(0, 0.5f));
	vLayoutFilename.setAnchorMax(sf::Vector2f(1, 0.5f));
	vLayoutFilename.setPivot(sf::Vector2f(0.5f, 0.5f));
	vLayoutFilename.setMargins(100, 100, 0, 0);
	vLayoutFilename.setSpacing(10);
	vLayoutFilename.add(lblFilenameTitle);
	vLayoutFilename.add(inputFilename);
	vLayoutFilename.add(layoutFilenameButtons);


	Layout filenameLayout(0, 0, 0, 0);
	filenameLayout.add(vLayoutFilename);

	// callback called by the filename apply button
	//void(*callback)(string);
	function<void(string)> callback;
	vector<sf::Int16> samples;
	/*sf::VertexArray vertices;
	vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);*/

	Signal signal;
	SignalRenderer signalRenderer(10, 50, 100, 80, style);
	signalRenderer.setSignal(signal);
	signalRenderer.setAnchorMin(sf::Vector2f(0, 0));
	signalRenderer.setAnchorMax(sf::Vector2f(1, 0));
	signalRenderer.setMargins(10, 10, 10, 10);
	signalRenderer.setSampleOffset(0);
	signalRenderer.setSampleRange(10);
	rootLayout.add(signalRenderer);

	Scrollbar scrollbar(0, 140, 0, 20, style);
	scrollbar.setAnchorMin(sf::Vector2f(0, 0));
	scrollbar.setAnchorMax(sf::Vector2f(1, 0));
	scrollbar.setMargins(10, 10, 10, 10);
	scrollbar.setNbStep(10);
	scrollbar.setViewedStep(1);
	rootLayout.add(scrollbar);
	/*float fadein_first = 0.2;
	float fadein_last = 0.6f;
	float fadeout_first = 0.4f;
	float fadeout_last = 0.8f;*/

	sf::Vector2f prevMousePos;


	vector<Component*> components;
	vector<ComponentRenderer*> compRenderers;

	// Component renderers
	OutputComponent output;
	ComponentRenderer outputRenderer(-100, -200, 150, 100, style);
	outputRenderer.setComponent(&output);
	view.add(outputRenderer);


	bool playing = false;
	bool paused = false;
	float maxZoomIn = 0.1f;
	float maxZoomOut = 10.0f;

	Window::SetLayout(rootLayout);

	ComponentRenderer* selectedComp = nullptr;

	float currendViewedStep = 1.0f;

	// ///////////////////////////// APPLICATION LOOP
	while (Window::IsOpen())
	{
		prevMousePos = Input::GetMousePosition();
		Window::Update();

		// ///////////////////////// GAME LOGIC
		if (Input::GetKeyDown(sf::Keyboard::Escape))
		{
			Window::Close();
		}

		Window::GetWindow()->setTitle(title + " | FPS: " + floatToStr(Time::GetFps(), 0));

		if (button.click())
		{
			samples.clear();

			float duration = inputDuration.getFloat();

			sf::Uint64 totalSample = duration * SAMPLERATE;
			output.init();

			for (int i = 0; i < totalSample; i++)
			{
				float x = (float)i / SAMPLERATE;
				samples.push_back(0x7FFF * clamp(output.getOutput(x), -1.0f, 1.0f));
			}

			signal.setData(samples);
			cout << "Signal sample count: " << signal.getSampleCount() << endl;

			scrollbar.setNbStep(samples.size());
			scrollbar.setViewedStep(currendViewedStep * signal.getSampleCount());
		}

		signalRenderer.setSampleRange(scrollbar.getViewedStep());
		signalRenderer.setSampleOffset(scrollbar.getCurrentStep());


		for (int i = 0; i < compBtns.size(); i++)
		{
			if (compBtns[i].click())
			{
				components.push_back(ComponentFactory::CreateComponent(compList[i]));
				compRenderers.push_back(new ComponentRenderer(0, 0, 200, 0, style));
				compRenderers.back()->setComponent(components.back());
				view.add(*compRenderers.back());
			}
		}

		if (btnPlay.click() && (!playing || paused))
		{
			float volume = inputVolume.getFloat();
			signal.getSound()->setVolume(volume);
			signal.getSound()->play();
			playing = true;
			paused = false;
		}
		if(btnStop.click() && playing)
		{
			signal.getSound()->stop();
			playing = false;
		}
		if (btnPause.click() && (playing && !paused))
		{
			signal.getSound()->pause();
			paused = true;
		}

		if (Input::GetKeyDown(sf::Keyboard::Add))
		{
			currendViewedStep = clamp(currendViewedStep - 0.1f, 0.1f, 1.0f);
			scrollbar.setViewedStep(currendViewedStep * signal.getSampleCount());
		}
		if (Input::GetKeyDown(sf::Keyboard::Subtract))
		{
			currendViewedStep = clamp(currendViewedStep + 0.1f, 0.1f, 1.0f);
			scrollbar.setViewedStep(currendViewedStep * signal.getSampleCount());
		}

		if (btnSave.click())
		{
			Window::SetLayout(filenameLayout);
			lblFilenameTitle.setText("Save Project");
			callback = [&](string _filename) {
				FileManager::Save(_filename + ".json", outputRenderer, compRenderers);
			};
		}
		if (btnLoad.click())
		{
			Window::SetLayout(filenameLayout);
			lblFilenameTitle.setText("Load Project");
			callback = [&](string _filename) {
				FileManager::Load(_filename + ".json", view, outputRenderer, compRenderers, components);
			};
		}

		if (btnExport.click())
		{
			Window::SetLayout(filenameLayout);
			lblFilenameTitle.setText("Export Wav Sound");
			callback = [&](string _filename) {
				FileManager::ExportWav(_filename + ".wav", *signal.getSound()->getBuffer());
			};
		}

		if (btnFilenameApply.click())
		{
			string filename = inputFilename.getString();
			if (!filename.empty())
			{
				inputFilename.setString("");
				//(*callback)(filename);
				callback(filename);
				Window::SetLayout(rootLayout);
			}
		}
		if (btnFilenameCancel.click())
		{
			inputFilename.setString("");
			Window::SetLayout(rootLayout);
		}

		if (view.hovered(Input::GetMousePosition()))
		{
			float delta = Input::GetMouseWheelScroll();
			if (delta != 0)
			{
				view.setZoom(clamp(view.getZoom() * (1.0f - 0.05f * delta), maxZoomIn, maxZoomOut));
			}

			if (Input::GetMouseButton(sf::Mouse::Middle))
			{
				view.setCenter(view.getCenter() - view.mapScreenVectorToView(prevMousePos, Input::GetMousePosition()));
			}

			if (Input::GetMouseButtonDown(sf::Mouse::Right))
			{
				AbstractUI* hoveredUI = UIManager::GetFirstHoveredUI(Input::GetMousePosition());
				ComponentRenderer* hoveredComp = dynamic_cast<ComponentRenderer*>(hoveredUI);
				if (hoveredComp != selectedComp)
				{
					if (selectedComp != nullptr)
					{
						selectedComp->setFocused(false);
					}
					selectedComp = hoveredComp;
					if (selectedComp != nullptr)
					{
						selectedComp->setFocused(true);
					}
				}
				else if (selectedComp != nullptr)
				{
					selectedComp->setFocused(false);
					selectedComp = nullptr;
				}
			}
		}

		if (Input::GetKeyDown(sf::Keyboard::Delete))
		{
			if (selectedComp != nullptr)
			{
				vector<ComponentRenderer*>::iterator renderIt = find(compRenderers.begin(), compRenderers.end(), selectedComp);
				if (renderIt != compRenderers.end())
				{
					vector<Component*>::iterator it = find(components.begin(), components.end(), selectedComp->getComponent());
					if (it != components.end())
					{
						if (view.remove(**renderIt))
						{
							delete *renderIt;
							delete *it;
							compRenderers.erase(renderIt);
							components.erase(it);
							selectedComp = nullptr;
						}
					}
				}
			}
		}

		//if (Input::GetMouseButtonDown(sf::Mouse::Left))
		//{
		//	sf::Vector2f mousePos = Input::GetMousePosition();
		//	//cout << "Mouse position: (" << mousePos.x << "," << mousePos.y << ")" << endl;
		//}

		// ///////////////////////// START DRAW
		Window::Clear();


		Window::Draw(); // Draw active layout (with all its children)

		// ///////////////////////// END DRAW
		Window::Display();
	}

	for (int i = 0; i < components.size(); i++)
	{
		delete components[i];
	}

	for (int i = 0; i < compRenderers.size(); i++)
	{
		delete compRenderers[i];
	}

	return 0;
}