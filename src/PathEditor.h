#pragma once
#include "Path.h"


typedef struct {
    void handle_event(sf::Event& event, sf::RenderWindow& window);
    Path* current_editing_path;
} SplineEditor, PathEditor;
