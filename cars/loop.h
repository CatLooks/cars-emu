// -- main loop functions -- //

// variables
Clock timer;
Event evt;

// startup code
void onStart() {
    onCreate();
};

// render debug text
void onDebug(int pos, int qc, float fps) {
    char text[101];

    // debug info
    sprintf(text, "= Debug =");
    renderText(text, 8, pos, colorTextSpecial);
    pos += 19;

    sprintf(text, "Camera: %d %d", camtile.x, camtile.y);
    renderText(text, 8, pos);
    pos += 19;

    sprintf(text, "Quads: %d", qc);
    renderText(text, 8, pos);
    pos += 19;

    sprintf(text, "FPS: %.1f", fps);
    renderText(text, 8, pos);
};

// game event handler
void onEventGame() {
    // program quit
    if (evt.type == Event::Closed) {
        app.close();
        return;
    };
    // key pressed
    if (evt.type == Event::KeyPressed) {
        // toggle debug mode
        if (evt.key.code == Keys::buttonDebug && evt.key.control)
            debugMode = !debugMode;

        switch (evt.key.code) {
            // go to world center
            case Keys::buttonHome:
            world.centerCamera();
            break;

            // switch window size
            case Keys::buttonFull:
            windowFull = !windowFull;
            app.close();
            updateWindow(windowFull);
            break;

            // switch to editor
            case Keys::buttonSwitch:
            room = Room::Editor;
            select = Vector2i(-1, 0);
            break;

            // update cars & lights
            case Keys::buttonTick:
            onTick();
            break;

            // spawn car
            case Keys::buttonSpawn:
            spawnCar(select.x, select.y);
            break;

            case Keyboard::H:
            folCar = &cars[0];
            break;

            // prevent warnings
            default:
            break;
        };
        return;
    };
    // mouse button pressed
    if (evt.type == Event::MouseButtonPressed) {
        // check mouse location
        if (evt.mouseButton.y < 64) {
            if (evt.mouseButton.x >= ws.x - 64 * 1) {
                // open pause menu
                room = Room::Menu;
                return;
            };
            if (evt.mouseButton.x >= ws.x - 64 * 2) {
                // cycle simulation speed
                speed = static_cast<Speed>((speed + 1) % 3);
                return;
            };
            if (evt.mouseButton.x >= ws.x - 64 * 3) {
                // reset simulation
                onReset();
                return;
            };
            if (evt.mouseButton.x >= ws.x - 64 * 4) {
                // spawn car
                spawnCar(select.x, select.y);
                select = Vector2i(-1, 0);
                return;
            };
        };

        if (evt.mouseButton.button == Mouse::Left) {
            // select a tile
            if (select == cursor)
                select = Vector2i(-1, 0);
            else
                select = cursor;
            return;
        } else if (evt.mouseButton.button == Mouse::Right) {
            // follow car
            if (folCar)
                folCar = null;
            else
                folCar = selCar;
        };
    };
    // mouse wheel scrolled
    if (evt.type == Event::MouseWheelMoved) {
        // switch zoom ID
        zoomID += evt.mouseWheel.delta;
        if (zoomID < 0)
            zoomID = 0;
        if (zoomID >= zoomVars)
            zoomID = zoomVars - 1;

        // update tile size
        float buf = wts;
        wts = zoomValues[zoomID];

        // pad camera
        camera.x *= wts / buf;
        camera.y *= wts / buf;
        return;
    };
};

// update game
void onUpdateGame(float fps) {
    Vector2i mouse = Mouse::getPosition(app);

    // update following car
    folCar = selCar;

    // update camera tile
    camtile.x = floor(float(camera.x - ws.x / 2) / wts);
    camtile.y = floor(float(camera.y - ws.y / 2) / wts);

    // update cursor
    cursor.x = floor(float(mouse.x + camera.x - ws.x / 2) / wts);
    cursor.y = floor(float(mouse.y + camera.y - ws.y / 2) / wts);

    // update cars & lights
    switch (speed) {
        // 1x time
        case Speed::Normal:
        onTick();
        break;
        // 2x time
        case Speed::Fast:
        onTick();
        onTick();
        break;
        // 0x time
        default:
        break;
    };

    // update booleans
    showTunnels = Keyboard::isKeyPressed(Keys::buttonTunnel);
    showPipes = Keyboard::isKeyPressed(Keys::buttonPipe);

    // update tile animation timer
    if (animTimer-- == 0) {
        animFrame = (animFrame + 1) % animationFrames;
        animTimer = animationSpeed;
    };

    // only if has focus
    if (app.hasFocus()) {
        // update movement
        // check keys
        if (Keyboard::isKeyPressed(Keys::buttonUp)) {
            camera.y -= moveValues[zoomID];
        };
        if (Keyboard::isKeyPressed(Keys::buttonDown)) {
            camera.y += moveValues[zoomID];
        };
        if (Keyboard::isKeyPressed(Keys::buttonLeft)) {
            camera.x -= moveValues[zoomID];
        };
        if (Keyboard::isKeyPressed(Keys::buttonRight)) {
            camera.x += moveValues[zoomID];
        };
        // check mouse
        if (false) {
            if (IntRect(0, 0, steerDist, rs.y).contains(mouse)) {
                camera.x -= moveValues[zoomID];
            };
            if (IntRect(0, 0, rs.x, steerDist).contains(mouse)) {
                camera.y -= moveValues[zoomID];
            };
            if (IntRect(rs.x - steerDist, 0, steerDist, rs.y).contains(mouse)) {
                camera.x += moveValues[zoomID];
            };
            if (IntRect(0, rs.y - steerDist, rs.x, steerDist).contains(mouse)) {
                camera.y += moveValues[zoomID];
            };
        };
    };
};

// render game
void onRenderGame(float fps) {
    // clear screen
    app.clear(colorVoid);

    // render world
    int qc = renderWorld(world, cars, lights);

    // render text
    viewDefault();
    char text[101];
    int pos = 8;

    // render game label
    renderText("= Game =", 8, pos, colorTextSpecial);
    pos += 19;

    // render light count
    sprintf(text, "Light Count: %d\n", (int)lights.size());
    renderText(text, 8, pos);
    pos += 19;

    // render car count
    sprintf(text, "Car Count: %d\n", (int)cars.size());
    renderText(text, 8, pos);
    pos += 29;

    // render info label
    Tile* tile = world.ftile(cursor);
    renderText("= Cursor =", 8, pos, colorTextSpecial);
    pos += 19;

    // render cursor position
    sprintf(text, "Position: %d %d", cursor.x, cursor.y);
    renderText(text, 8, pos);
    pos += 19;

    // render cursor occupation
    if (tile) {
        if (tile->buf.occ) {
            renderText("Occupied", 8, pos, colorTextInvalid);
            pos += 19;
        };
    };
    pos += 10;

    if (selCar) {
        // render car label
        renderText("= Car Info =", 8, pos, colorTextSpecial);
        pos += 19;

        // get car data
        Vector3i s = selCar->getSpeed();
        Vector2i b = selCar->getServices();
        Vector2i p = selCar->getCoords();
        int w = selCar->getWait();

        // render car info
        sprintf(text, "Max Speed: %d", s.x);
        renderText(text, 8, pos);
        pos += 19;

        sprintf(text, "Road Slowdown: %.1f%%", 100 - s.y / 2.55);
        renderText(text, 8, pos);
        pos += 19;

        sprintf(text, "Actual Speed: %d", s.z);
        renderText(text, 8, pos);
        pos += 24;

        sprintf(text, "Waiting Time: %d", w);
        renderText(text, 8, pos);
        pos += 24;

        sprintf(text, "Trips to Parking: %d", b.x);
        renderText(text, 8, pos);
        pos += 19;

        sprintf(text, "Trips to Gas Station: %d", b.y);
        renderText(text, 8, pos);
        pos += 24;

        sprintf(text, "Position: %.2f, %.2f", p.x / 64.0, p.y / 64.0);
        renderText(text, 8, pos);
        pos += 29;
    };

    // render debug text
    if (debugMode)
        onDebug(pos, qc, fps);
};

// editor event handler
void onEventEdit() {
    // program quit
    if (evt.type == Event::Closed) {
        app.close();
        return;
    };
    if (evt.type == Event::KeyPressed) {
        // toggle debug mode
        if (evt.key.code == Keys::buttonDebug && evt.key.control)
            debugMode = !debugMode;

        switch (evt.key.code) {
            // go to world center
            case Keys::buttonHome:
            camera.x = world.width() * (wts / 2);
            camera.y = world.height() * (wts / 2);
            break;

            // switch window size
            case Keys::buttonFull:
            windowFull = !windowFull;
            app.close();
            updateWindow(windowFull);
            break;

            // switch to editor
            case Keys::buttonSwitch:
            room = Room::Game;
            bufferService(world, Dests::parks, Service::Parking);
            bufferService(world, Dests::shops, Service::Shop);
            bufferService(world, Dests::fuels, Service::Fuel);
            break;

            // switch editor mode
            case Keys::buttonModeTile:
            mode = Edit::Tiles;
            selTile = -1;
            break;
            case Keys::buttonModeSide:
            mode = Edit::Sides;
            selTile = -1;
            break;
            case Keys::buttonModeCorner:
            mode = Edit::Corners;
            selTile = -1;
            break;
            case Keys::buttonModeMark:
            mode = Edit::Marks;
            selTile = -1;
            break;
            case Keys::buttonModeAccess:
            mode = Edit::Borders;
            selTile = -1;
            break;
            case Keys::buttonModeTunnel:
            mode = Edit::Tunnels;
            selTile = -1;
            break;
            case Keys::buttonModePark:
            mode = Edit::Parking;
            selTile = -1;
            break;
            case Keys::buttonModeLight:
            mode = Edit::Lights;
            select = Vector2i(-1, 0);
            break;

            // prevent warnings
            default:
            break;
        };
        return;
    };
    // mouse clicked
    if (evt.type == Event::MouseButtonPressed) {
        if (evt.mouseButton.button == Mouse::Left) {
            // menu
            if (evt.mouseButton.y < 64) {
                if (evt.mouseButton.x >= ws.x - 64 * 1) {
                    // open menu
                    room = Room::Menu;
                    return;
                } else if (evt.mouseButton.x >= ws.x - 64 * 2) {
                    // cycle simulation speed
                    speed = static_cast<Speed>((speed + 1) % 3);
                    return;
                } else if (evt.mouseButton.x >= ws.x - 64 * 3) {
                    // cycle edit modes
                    mode = static_cast<Edit>((mode + 1) % 9);
                    if (mode == Edit::View)
                        mode = Edit::Tiles;
                };

                return;
            };
            if (evt.mouseButton.x >= ws.x - palleteDist) {
                Vector2i mv(evt.mouseButton.x, evt.mouseButton.y);

                // pallete
                if (mode == Edit::Lights) {
                    // light editor
                    if (select == Vector2i(-1, 0)) {
                        if (IntRect(ws.x - 240, 180, 224, 40).contains(mv)) {
                            createLight();
                        };
                    } else {
                        // click buttons
                        if (IntRect(ws.x - 240, 216, 32, 32).contains(mv)) {
                            selLight->setBorder(lightState, 0);

                        } else if (IntRect(ws.x - 192, 216, 32, 32).contains(mv)) {
                            selLight->setBorder(lightState, 1);

                        } else if (IntRect(ws.x - 144, 216, 32, 32).contains(mv)) {
                            lightState = !lightState;

                        } else if (IntRect(ws.x - 96, 216, 32, 32).contains(mv)) {
                            selLight->setBorder(lightState, 2);

                        } else if (IntRect(ws.x - 48, 216, 32, 32).contains(mv)) {
                            selLight->setBorder(lightState, 3);

                        } else if (IntRect(ws.x - 64, 256, 48, 48).contains(mv)) {
                            selLight->toggleLightColor(lightState);

                        } else if (IntRect(ws.x - 240, 420, 224, 40).contains(mv)) {
                            selLight->toggleVisibility();

                        } else if (IntRect(ws.x - 240, 470, 40, 40).contains(mv)) {
                            selLight->toggleCorner(8);

                        } else if (IntRect(ws.x - 179, 470, 40, 40).contains(mv)) {
                            selLight->toggleCorner(4);

                        } else if (IntRect(ws.x - 117, 470, 40, 40).contains(mv)) {
                            selLight->toggleCorner(2);

                        } else if (IntRect(ws.x - 56, 470, 40, 40).contains(mv)) {
                            selLight->toggleCorner(1);

                        } else if (IntRect(ws.x - 240, 520, 224, 40).contains(mv)) {
                            deleteLight();
                        };
                    };
                } else if (mode == Edit::View) {
                    if (IntRect(ws.x - 155, 82, 57, 28).contains(mv)) {
                        //dataEnter = true;
                    };

                } else {
                    // tilesets
                    int t = (((evt.mouseButton.x - ws.x + palleteDist) >> 6) & 3) | (evt.mouseButton.y / 64 - 2) << 2;
                    if (t == selTile)
                        selTile = -1;
                    else
                        selTile = t;
                    if (selTile >= palleteTileCount[mode] || selTile < -1)
                        selTile = -1;
                    selectTile();
                };
            } else {
                // world
                if (mode == Edit::Lights) {
                    if (select == cursor) {
                        select = Vector2i(-1, 0);
                        selLight = null;
                    } else select = cursor;

                    // validate selected tile
                    bool match = false;
                    for (Light& light : lights) {
                        if (light.getCoords() == select) {
                            selLight = &light;
                            match = true;
                            break;
                        };
                    };

                    // deselect if invalid
                    if (match == false) {
                        select = Vector2i(-1, 0);
                        selLight = null;
                    };
                };
            };
        };
        return;
    };
    // mouse wheel scrolled
    if (evt.type == Event::MouseWheelMoved) {
        if (evt.mouseWheel.x < ws.x - palleteDist) {
            // switch zoom ID
            zoomID += evt.mouseWheel.delta;
            if (zoomID < 0)
                zoomID = 0;
            if (zoomID >= zoomVars)
                zoomID = zoomVars - 1;

            // update tile size
            float buf = wts;
            wts = zoomValues[zoomID];

            // pad camera
            camera.x *= wts / buf;
            camera.y *= wts / buf;
            return;
        } else {
            if (mode == Edit::Lights && selLight) {
                Vector2i mv(evt.mouseWheel.x, evt.mouseWheel.y);

                // roll light
                if (IntRect(ws.x - 240, 264, 72, 36).contains(mv)) {
                    selLight->rollTicks(lightState, evt.mouseWheel.delta, true);
                } else if (IntRect(ws.x - 168, 264, 72, 36).contains(mv)) {
                    selLight->rollTicks(lightState, evt.mouseWheel.delta, false);
                }

                // roll initial pause
                if (IntRect(ws.x - 145, 318, 67, 36).contains(mv)) {
                    selLight->rollInitialTicks(lightState, evt.mouseWheel.delta, true);
                } else if (IntRect(ws.x - 78, 318, 67, 36).contains(mv)) {
                    selLight->rollInitialTicks(lightState, evt.mouseWheel.delta, false);
                }

                // roll coords
                if (IntRect(ws.x - 170, 370, 80, 40).contains(mv)) {
                    selLight->rollCoords(evt.mouseWheel.delta, false);
                    select = selLight->getCoords();
                } else if (IntRect(ws.x - 90, 370, 74, 40).contains(mv)) {
                    selLight->rollCoords(evt.mouseWheel.delta, true);
                    select = selLight->getCoords();
                };
            };
        };
    };
};

// update editor
void onUpdateEdit(float fps) {
    Vector2i mouse = Mouse::getPosition(app);

    // update camera tile
    camtile.x = floor(float(camera.x - ws.x / 2) / wts);
    camtile.y = floor(float(camera.y - ws.y / 2) / wts);

    // update cursor
    cursor.x = floor(float(mouse.x + camera.x - ws.x / 2) / wts);
    cursor.y = floor(float(mouse.y + camera.y - ws.y / 2) / wts);

    // update tile animation timer
    if (animTimer-- == 0) {
        animFrame = (animFrame + 1) % animationFrames;
        animTimer = animationSpeed;
    };

    // only if has focus
    if (app.hasFocus()) {
        // update movement
        // check keys
        if (Keyboard::isKeyPressed(Keys::buttonUp)) {
            camera.y -= moveValues[zoomID];
        };
        if (Keyboard::isKeyPressed(Keys::buttonDown)) {
            camera.y += moveValues[zoomID];
        };
        if (Keyboard::isKeyPressed(Keys::buttonLeft)) {
            camera.x -= moveValues[zoomID];
        };
        if (Keyboard::isKeyPressed(Keys::buttonRight)) {
            camera.x += moveValues[zoomID];
        };
        // check mouse
        if (false) {
            if (IntRect(0, 0, steerDist, rs.y).contains(mouse)) {
                camera.x -= moveValues[zoomID];
            };
            if (IntRect(0, 0, rs.x, steerDist).contains(mouse)) {
                camera.y -= moveValues[zoomID];
            };
            if (IntRect(rs.x - steerDist, 0, steerDist, rs.y).contains(mouse)) {
                camera.x += moveValues[zoomID];
            };
            if (IntRect(0, rs.y - steerDist, rs.x, steerDist).contains(mouse)) {
                camera.y += moveValues[zoomID];
            };
        };

        // draw data
        if (mouse.x < (ws.x - palleteDist)) {
            if (Mouse::isButtonPressed(Mouse::Left))
                selectCoord(world, true);
            if (Mouse::isButtonPressed(Mouse::Right))
                selectCoord(world, false);
        };
    };
};

// render editor
void onRenderEdit(float fps) {
    // clear screen
    app.clear(colorVoid);

    // render editor
    int qc = renderEditor(world, lights);

    // render text
    viewDefault();
    char text[101];
    int pos = 8;

    sprintf(text, "= Edit =");
    renderText(text, 8, pos, colorTextSpecial);
    pos += 19;

    sprintf(text, "Cursor: %d %d", cursor.x, cursor.y);
    renderText(text, 8, pos);
    pos += 19;

    if (select == Vector2i(-1, 0))
        sprintf(text, "Select: Null");
    else
        sprintf(text, "Select: %d %d", select.x, select.y);
    renderText(text, 8, pos);
    pos += 19;

    sprintf(text, "Mode: %s", modeName[mode]);
    renderText(text, 8, pos);
    pos += 29;

    //sprintf(text, "= Edit =", cursor.x, cursor.y);
    //renderText(text, 8, pos, colorTextSpecial);
    //pos += 19;

    // render debug text
    if (debugMode)
        onDebug(pos, qc, fps);
};

// handle menu events
void onEventMenu() {
    // close program
    if (evt.type == Event::Closed) {
        app.close();
        return;
    };
    // keypress
    if (evt.type == Event::KeyPressed) {
        // toggle debug mode
        if (evt.key.code == Keys::buttonDebug && evt.key.control) {
            debugMode = !debugMode;
            return;
        };

        switch (evt.key.code) {
            // switch window size
            case Keys::buttonFull:
            windowFull = !windowFull;
            app.close();
            updateWindow(windowFull);
            break;

            // prevent warnings
            default:
            break;
        };

        return;
    };
    // mouse click
    if (evt.type == Event::MouseButtonPressed) {
        Vector2i mv(evt.mouseButton.x, evt.mouseButton.y);

        // switch button
        if (IntRect(ws.x - 64, 0, 64, 64).contains(mv)) {
            room = Room::Game;
        };
        // close button
        if (IntRect(ws.x - 64, ws.y - 64, 64, 64).contains(mv)) {
            app.close();
            return;
        };

        // slot buttons
        for (int t = 0; t < 10; t++) {
            int x = (ws.x * (t + 1)) / 11;
            if (IntRect(x - 32, 96, 64, 64).contains(mv)) {
                if (menuSelect == t)
                    menuSelect = -1;
                else
                    menuSelect = t;
                return;
            };
        };

        if (menuSelect >= 0 && menuSelect < 10) {
            if (menuMaps[menuSelect].valid) {
                if (IntRect(ws.x * 9 / 11 - 32, 188, ws.x / 11 + 64, 48).contains(mv)) {
                    onLoad();
                } else if (IntRect(ws.x * 9 / 11 - 32, 238, ws.x / 11 + 64, 48).contains(mv)) {
                    onSave();
                } else if (IntRect(ws.x * 9 / 11 - 32, 288, ws.x / 11 + 64, 48).contains(mv)) {
                    onDelete();
                };
            } else {
                if (IntRect(ws.x * 9 / 11 - 32, 188, ws.x / 11 + 64, 48).contains(mv)) {
                    onSave();
                } else if (IntRect(ws.x * 9 / 11 - 32, 238, ws.x / 11 + 64, 48).contains(mv)) {
                    onNew();
                };
            };
            return;
        };

        // deselect
        menuSelect = -1;
    };
};

// update menu
void onUpdateMenu(float fps) {
    // update menu offset
    menuOffset = (menuOffset + 1) & 63;
};

// render menu
void onRenderMenu(float fps) {
    // clear screen
    app.clear(colorVoid);

    // render menu
    int qc = renderMenu();

    // render text
    viewDefault();
    int pos = 8;

    // render debug text
    if (debugMode)
        onDebug(pos, qc, fps);
};
