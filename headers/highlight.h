#pragma once
#include "main.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

extern bool showCodePanel;
extern float codePanelWidth;

void drawCodeHighlightPanel(int indexSourceCode,  int currentStepIdx, bool isStepByStep, bool isFinished, std::vector<int> activeLines = std::vector<int> {});