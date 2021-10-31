// -- core file -- //

// include libraries
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <list>
using namespace sf;

// include project
#include "project.h"

// program entry
int main() {
    // create window
    updateWindow(false);

    // load data
    if (loadFiles())
        return 1;

    // measure fps
    float fps = 0;

    // main loop
    onStart();
    while (app.isOpen()) {
        switch (room) {
            case Room::Game: {
                // handle events
                while (app.pollEvent(evt))
                    onEventGame();

                // handle game update
                onUpdateGame(fps);

                // handle game render
                onRenderGame(fps);
            };
            break;
            case Room::Editor: {
                // handle events
                while (app.pollEvent(evt))
                    onEventEdit();

                // handle game update
                onUpdateEdit(fps);

                // handle game render
                onRenderEdit(fps);
            };
            break;
            case Room::Menu: {
                // handle events
                while (app.pollEvent(evt))
                    onEventMenu();

                // handle game update
                onUpdateMenu(fps);

                // handle game render
                onRenderMenu(fps);
            };
        };

        // update frame
        timer.restart();
        app.display();
        fps = timer.restart().asMilliseconds();
        if (fps)
            fps = 1000 / fps;
    };

    // exit success
    return 0;
};
