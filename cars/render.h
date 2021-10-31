// -- render manager -- //

// render layers
const bool LAYER_LOW  = true;
const bool LAYER_HIGH = false;

// render text
void renderText(st str, int x, int y, Color color = colorTextDefault, int charSize = 16, bool centered = false) {
    vtext.setCharacterSize(charSize);
    vtext.setString(str);
    if (centered)
        vtext.setPosition(x - vtext.getLocalBounds().width / 2, y);
    else
        vtext.setPosition(x, y);
    vtext.setFillColor(color);
    app.draw(vtext);
};

// render tile
int renderTile(VertexArray& arr, Tile* tile, int rx, int ry, bool rl) {
    if (tile == null)
        return 0;

    // check if elevated (if on low level)
    if (rl) {
        if (tile->tunnel != Tunnel::None)
            return 1; // add to elevation level
    };

    // get position
    IntRect pos(rx, ry, wts, wts);

    // render texture
    /*if (tile->tile == TILE_VOID) {
        // render edges
        bt edges = tile->buf.edges;
        renderSquare(arr, pos, texRoadCorners[edges >> 4], colorEdge);
        renderSquare(arr, pos, texRoadBorders[edges & 15], colorEdge);

    } else */if (tile->tile == TILE_ROAD) {
        renderSquare(arr, pos, texRoad);
        renderSquare(arr, pos, texRoadMarks[tile->marks]);
        renderSquare(arr, pos, texRoadCorners[tile->corners]);
        renderSquare(arr, pos, texRoadBorders[tile->sides]);

    } else if (tile->tile == TILE_DIRT) {
        renderSquare(arr, pos, texDirt);
        renderSquare(arr, pos, texRoadCorners[tile->corners], colorDirt);
        renderSquare(arr, pos, texRoadBorders[tile->sides], colorDirt);

    } else if (tile->tile == TILE_PARK) {
        renderSquare(arr, pos, texPark);

    } else if (tile->tile == TILE_GRASS) {
        renderSquare(arr, pos, texGrass[(animFrame + rx / 64 + ry / 64) % 4]);

    } else if (tile->tile == TILE_WATER) {
        renderSquare(arr, pos, texWater[animFrame]);

    } else if (tile->tile == TILE_FLOOR) {
        renderSquare(arr, pos, texBuilding[0]);
        renderSquare(arr, pos, texRoadCorners[tile->corners], colorPave);
        renderSquare(arr, pos, texRoadBorders[tile->sides], colorPave);

    } else if (tile->tile == TILE_BRICK) {
        renderSquare(arr, pos, texBrick);
        renderSquare(arr, pos, texRoadCorners[tile->corners], colorPave);
        renderSquare(arr, pos, texRoadBorders[tile->sides], colorPave);

    };

    // successful render
    return isBuilding[tile->tile] ? 2 : 0;
};
Tunnel renderTile(VertexArray& arr, Tile* tile, int rx, int ry) {
    if (tile == null)
        return Tunnel::None;

    // get position
    IntRect pos(rx, ry, wts, wts);

    // render texture
    /*if (tile->tile == TILE_VOID) {
        // render edges
        bt edges = tile->buf.edges;
        renderSquare(arr, pos, texRoadCorners[edges >> 4], colorEdge);
        renderSquare(arr, pos, texRoadBorders[edges & 15], colorEdge);

    } else */if (tile->tile == TILE_ROAD) {
        renderSquare(arr, pos, texRoad);
        renderSquare(arr, pos, texRoadMarks[tile->marks]);
        renderSquare(arr, pos, texRoadCorners[tile->corners]);
        renderSquare(arr, pos, texRoadBorders[tile->sides]);

    } else if (tile->tile == TILE_DIRT) {
        renderSquare(arr, pos, texDirt);
        renderSquare(arr, pos, texRoadCorners[tile->corners], colorDirt);
        renderSquare(arr, pos, texRoadBorders[tile->sides], colorDirt);

    } else if (tile->tile == TILE_PARK) {
        renderSquare(arr, pos, texPark);

    } else if (tile->tile == TILE_GRASS) {
        renderSquare(arr, pos, texGrass[animFrame]);

    } else if (tile->tile == TILE_WATER) {
        renderSquare(arr, pos, texWater[animFrame]);

    } else if (tile->tile == TILE_FLOOR) {
        renderSquare(arr, pos, texBuilding[0]);
        renderSquare(arr, pos, texRoadCorners[tile->corners], colorPave);
        renderSquare(arr, pos, texRoadBorders[tile->sides], colorPave);

    } else if (tile->tile == TILE_CEIL) {
        renderSquare(arr, pos, texBuilding[1]);
        renderSquare(arr, pos, texRoadCorners[tile->corners], colorPave);
        renderSquare(arr, pos, texRoadBorders[tile->sides], colorPave);

    } else if (tile->tile == TILE_BRICK) {
        renderSquare(arr, pos, texBrick);
        renderSquare(arr, pos, texRoadCorners[tile->corners], colorPave);
        renderSquare(arr, pos, texRoadBorders[tile->sides], colorPave);

    };

    // return tunnel
    return tile->tunnel;
};

// render world
int renderWorld(World& world, vector<Car> cars, vector<Light> lights) {
    // new vertex array
    VertexArray arr(Quads);

    // apply camera
    viewCamera(camera);

    // structure <Tile + Position>
    struct tp {
        Tile* tile;
        int x, y;

        // constructor
        tp(Vector2i p, Tile* t) {
            tile = t;
            x = p.x;
            y = p.y;
        };
    };

    // foreground tiles
    vector<Car*> above;
    vector<tp> builds;
    vector<tp> hills;

    // render rug
    renderSquare(arr, IntRect(0, 0, world.width() * wts, world.height() * wts), colorRug);

    // render ground tiles
    for (int y = camtile.y - 1; y < camtile.y + ws.y / wts + 2; y++) {
        for (int x = camtile.x - 1; x < camtile.x + (ws.x) / wts + 2; x++) {
            Tile* tile = world.ftile(Vector2i(x, y));
            Vector2i pos(x * wts, y * wts);

            int i = renderTile(arr, tile, pos.x, pos.y, LAYER_LOW);
            switch (i) {
                case 1: {
                    if (showTunnels) {
                        if (tile->tunnel)
                            renderSquare(arr, IntRect(pos.x, pos.y, wts, wts), texTunnel[tile->tunnel - 1]);
                    } else {
                        hills.push_back(tp(pos, tile));
                    };
                };
                break;
                case 2: {
                    if (showTunnels) {
                        renderTile(arr, tile, pos.x, pos.y);
                    } else {
                        builds.push_back(tp(pos, tile));
                    };
                };
                break;
                default:
                break;
            };
        };
    };

    // render underground cars
    for (Car& car : cars) {
        if (car.isUnderground()) {
            if (car.render(arr, Mouse::getPosition(app) + Vector2i(camera.x - ws.x / 2, camera.y - ws.y / 2)))
                selCar = &car;
        } else above.push_back(&car);
    };

    // render hill tiles
    for (tp& u : hills) {
        renderTile(arr, u.tile, u.x, u.y, LAYER_HIGH);
    };

    // render ground cars
    selCar = null;
    for (Car* car : above) {
        if (car->render(arr, Mouse::getPosition(app) + Vector2i(camera.x - ws.x / 2, camera.y - ws.y / 2))) {
            selCar = car;
        };
    };

    // render buildings
    for (tp& u : builds) {
        renderTile(arr, u.tile, u.x, u.y);
    };

    // render pipes
    if (showPipes) {
        for (int y = camtile.y - 1; y < camtile.y + ws.y / wts + 2; y++) {
            for (int x = camtile.x - 1; x < camtile.x + (ws.x) / wts + 2; x++) {
                Tile* tile = world.ftile(Vector2i(x, y));
                if (tile == null)
                    continue;

                Vector2i pos(x * wts, y * wts);
                IntRect rect(pos.x, pos.y, wts, wts);
                Border b[4] {gtb(tile, Direction::North), gtb(tile, Direction::South), gtb(tile, Direction::West), gtb(tile, Direction::East)};

                if (b[0])
                    renderSquare(arr, rect, texBorder[b[0]], Direction::North);
                if (b[1])
                    renderSquare(arr, rect, texBorder[b[1]], Direction::South);
                if (b[2])
                    renderSquare(arr, rect, texBorder[b[2]], Direction::West);
                if (b[3])
                    renderSquare(arr, rect, texBorder[b[3]], Direction::East);
            };
        };
    };

    // render lights
    for (Light& light : lights) {
        light.render(arr);
    };

    // render cursor
    if (select != Vector2i(-1, 0))
        renderSquare(arr, IntRect(select.x * wts, select.y * wts, wts, wts), texCursor, colorDark);

    // display world
    app.draw(arr, &vtex);
    int vc = arr.getVertexCount();
    arr.clear();

    // render menu
    viewDefault();
    renderSquare(arr, IntRect(ws.x - 64 * 4, 0, 64, 64), texCarIcon);
    renderSquare(arr, IntRect(ws.x - 64 * 3, 0, 64, 64), texReset);
    renderSquare(arr, IntRect(ws.x - 64 * 2, 0, 64, 64), texSpeed[speed]);
    renderSquare(arr, IntRect(ws.x - 64 * 1, 0, 64, 64), texMenu);

    // display menu
    app.draw(arr, &vtex);

    // return quad count
    return (arr.getVertexCount() + vc) / 4;
};

// render editor
int renderEditor(World& world, vector<Light> lights) {
    // new vertex array
    VertexArray arr(Quads);

    // apply camera
    viewCamera(camera);

    // render rug
    renderSquare(arr, IntRect(0, 0, world.getSize().x * wts, world.getSize().y * wts), colorRug);

    // render tiles
    Direction dir = static_cast<Direction>(selTile >> 2);
    for (int y = camtile.y - 1; y < camtile.y + ws.y / wts + 2; y++) {
        for (int x = camtile.x - 1; x < camtile.x + (ws.x - palleteDist) / wts + 2; x++) {
            Tile* tile = world.ftile(Vector2i(x, y));
            if (tile == null)
                continue;

            Vector2i pos(x * wts, y * wts);
            IntRect rect(pos.x, pos.y, wts, wts);

            // render tile
            Tunnel t = renderTile(arr, tile, pos.x, pos.y);
            if (t) {
                renderSquare(arr, rect, texTunnel[t - 1], mode == Edit::Tunnels ? colorTextDefault : colorHalf);
            };
            if (tile->service)
                renderSquare(arr, rect, texService[tile->service - 1], mode == Edit::Parking ? colorTextDefault : colorHalf);

            // render borders
            switch (mode) {
                case Edit::Borders: {
                    if (darkDetails) {
                        if (tile->borders[0])
                            renderSquare(arr, rect, texBorder[tile->borders[0]], Direction::North, colorVoid);
                        if (tile->borders[1])
                            renderSquare(arr, rect, texBorder[tile->borders[1]], Direction::South, colorVoid);
                        if (tile->borders[2])
                            renderSquare(arr, rect, texBorder[tile->borders[2]], Direction::West, colorVoid);
                        if (tile->borders[3])
                            renderSquare(arr, rect, texBorder[tile->borders[3]], Direction::East, colorVoid);
                    } else if (selTile >= 16 || selTile == -1) {
                        if (tile->borders[0])
                            renderSquare(arr, rect, texBorder[tile->borders[0]], Direction::North);
                        if (tile->borders[1])
                            renderSquare(arr, rect, texBorder[tile->borders[1]], Direction::South);
                        if (tile->borders[2])
                            renderSquare(arr, rect, texBorder[tile->borders[2]], Direction::West);
                        if (tile->borders[3])
                            renderSquare(arr, rect, texBorder[tile->borders[3]], Direction::East);
                    } else {
                        if (tile->borders[0])
                            renderSquare(arr, rect, texBorder[tile->borders[0]], Direction::North, (dir == Direction::North) ? colorTextDefault : colorVoid);
                        if (tile->borders[1])
                            renderSquare(arr, rect, texBorder[tile->borders[1]], Direction::South, (dir == Direction::South) ? colorTextDefault : colorVoid);
                        if (tile->borders[2])
                            renderSquare(arr, rect, texBorder[tile->borders[2]], Direction::West,  (dir == Direction::West)  ? colorTextDefault : colorVoid);
                        if (tile->borders[3])
                            renderSquare(arr, rect, texBorder[tile->borders[3]], Direction::East,  (dir == Direction::East)  ? colorTextDefault : colorVoid);
                    };
                };
                break;
                case Edit::Parking: {
                    if (tile->service)
                        renderSquare(arr, rect, texService[tile->service - 1]);
                };
                break;
                case Edit::Lights: {
                    if (tile->borders[0])
                        renderSquare(arr, rect, texBorder[tile->borders[0]], Direction::North, colorBack);
                    if (tile->borders[1])
                        renderSquare(arr, rect, texBorder[tile->borders[1]], Direction::South, colorBack);
                    if (tile->borders[2])
                        renderSquare(arr, rect, texBorder[tile->borders[2]], Direction::West, colorBack);
                    if (tile->borders[3])
                        renderSquare(arr, rect, texBorder[tile->borders[3]], Direction::East, colorBack);
                };

                // prevent warnings
                default:
                break;
            };
        };
    };

    // render lights
    if (mode == Edit::Lights) {
        for (Light& light : lights) {
            light.renderEdit(arr);
        };
    } else {
        for (Light& light : lights) {
            light.render(arr);
        };
    };

    // render cursor
    renderSquare(arr, IntRect(cursor.x * wts, cursor.y * wts, wts, wts), texCursor, colorDark);

    // display editor
    app.draw(arr, &vtex);
    int vc = arr.getVertexCount();

    // render pallete BG
    viewDefault();
    arr.clear();
    renderSquare(arr, IntRect(ws.x - palleteDist - palleteBorderWidth, 0, palleteBorderWidth + palleteDist, ws.y), palleteBorderColor);
    renderSquare(arr, IntRect(ws.x - palleteDist, 64, palleteDist, ws.y), palleteColor);

    // render pallete tiles
    switch (mode) {
        case Edit::Tiles: {
            renderSquare(arr, IntRect(ws.x - 64 * 3, 128, 64, 64), texRoad);
            renderSquare(arr, IntRect(ws.x - 64 * 2, 128, 64, 64), texPark);
            renderSquare(arr, IntRect(ws.x - 64 * 1, 128, 64, 64), texGrass[animFrame]);
            renderSquare(arr, IntRect(ws.x - 64 * 4, 192, 64, 64), texDirt);
            renderSquare(arr, IntRect(ws.x - 64 * 3, 192, 64, 64), texBuilding[0]);
            renderSquare(arr, IntRect(ws.x - 64 * 2, 192, 64, 64), texBuilding[0]);
            renderSquare(arr, IntRect(ws.x - 64 * 1, 192, 64, 64), texWater[animFrame]);
            renderSquare(arr, IntRect(ws.x - 64 * 4, 256, 64, 64), texBrick);

            for (int y = 0; y < 2; y++) {
                for (int x = 0; x < 4; x++) {
                    renderSquare(arr, IntRect(ws.x - 64 * (4 - x), y * 64 + 128, 64, 64), texPallete, (y * 4 + x == selTile) ? palleteSelected : palleteBorderColor);
                };
            };
            for (int x = 0; x < 1; x++) {
                renderSquare(arr, IntRect(ws.x - 64 * (4 - x), 256, 64, 64), texPallete, (2 * 4 + x == selTile) ? palleteSelected : palleteBorderColor);
            };
        };
        break;
        case Edit::Sides: {
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    renderSquare(arr, IntRect(ws.x - 64 * (4 - x), y * 64 + 128, 64, 64), texRoadBorders[y * 4 + x]);
                    renderSquare(arr, IntRect(ws.x - 64 * (4 - x), y * 64 + 128, 64, 64), texPallete, (y * 4 + x == selTile) ? palleteSelected : palleteBorderColor);
                };
            };
        };
        break;
        case Edit::Corners: {
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    renderSquare(arr, IntRect(ws.x - 64 * (4 - x), y * 64 + 128, 64, 64), texRoadCorners[y * 4 + x]);
                    renderSquare(arr, IntRect(ws.x - 64 * (4 - x), y * 64 + 128, 64, 64), texPallete, (y * 4 + x == selTile) ? palleteSelected : palleteBorderColor);
                };
            };
        };
        break;
        case Edit::Marks: {
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    renderSquare(arr, IntRect(ws.x - 64 * (4 - x), y * 64 + 128, 64, 64), texRoadMarks[y * 4 + x]);
                    renderSquare(arr, IntRect(ws.x - 64 * (4 - x), y * 64 + 128, 64, 64), texPallete, (y * 4 + x == selTile) ? palleteSelected : palleteBorderColor);
                };
            };
            for (int x = 0; x < 3; x++) {
                renderSquare(arr, IntRect(ws.x - 64 * (4 - x), 4 * 64 + 128, 64, 64), texRoadMarks[4 * 4 + x]);
                renderSquare(arr, IntRect(ws.x - 64 * (4 - x), 4 * 64 + 128, 64, 64), texPallete, (4 * 4 + x == selTile) ? palleteSelected : palleteBorderColor);
            };
        };
        break;
        case Edit::Borders: {
            for (int x = 0; x < 4; x++) {
                for (int y = 0; y < 4; y++) {
                    renderSquare(arr, IntRect(ws.x - 64 * (4 - x), y * 64 + 128, 64, 64), texBorder[x], static_cast<Direction>(y));
                    renderSquare(arr, IntRect(ws.x - 64 * (4 - x), 4 * 64 + 128, 64, 64), texBorder[x], static_cast<Direction>(y));
                    renderSquare(arr, IntRect(ws.x - 64 * (4 - x), y * 64 + 128, 64, 64), texPallete, (y * 4 + x == selTile) ? palleteSelected : palleteBorderColor);
                };
                renderSquare(arr, IntRect(ws.x - 64 * (4 - x), 4 * 64 + 128, 64, 64), texPallete, (4 * 4 + x == selTile) ? palleteSelected : palleteBorderColor);
            };
        };
        break;
        case Edit::Tunnels: {
            renderSquare(arr, IntRect(ws.x - 64 * 3, 128, 64, 64), texTunnel[0]);
            renderSquare(arr, IntRect(ws.x - 64 * 2, 128, 64, 64), texTunnel[1]);
            renderSquare(arr, IntRect(ws.x - 64 * 1, 128, 64, 64), texTunnel[2]);
            for (int x = 0; x < 4; x++) {
                renderSquare(arr, IntRect(ws.x - 64 * (4 - x), 128, 64, 64), texPallete, (x == selTile) ? palleteSelected : palleteBorderColor);
            };
        };
        break;
        case Edit::Parking: {
            renderSquare(arr, IntRect(ws.x - 64 * 3, 128, 64, 64), texService[0]);
            renderSquare(arr, IntRect(ws.x - 64 * 2, 128, 64, 64), texService[1]);
            renderSquare(arr, IntRect(ws.x - 64 * 1, 128, 64, 64), texService[2]);
            for (int x = 0; x < 4; x++) {
                renderSquare(arr, IntRect(ws.x - 64 * (4 - x), 128, 64, 64), texPallete, (x == selTile) ? palleteSelected : palleteBorderColor);
            };
        };
        break;
        case Edit::Lights: {
            if (select == Vector2i(-1, 0)) {
                // render new button
                renderSquare(arr, IntRect(ws.x - 240, 180, 224, 40), colorTextValid);
            } else {
                // get tile
                Tile* tile = world.ftile(selLight->getCoords());
                IntRect rect(ws.x - 240, 144, 64, 64);

                // render tile borders
                if (tile) {
                    if (tile->borders[0])
                        renderSquare(arr, rect, texBorder[tile->borders[0]], Direction::North);
                    if (tile->borders[1])
                        renderSquare(arr, rect, texBorder[tile->borders[1]], Direction::South);
                    if (tile->borders[2])
                        renderSquare(arr, rect, texBorder[tile->borders[2]], Direction::West);
                    if (tile->borders[3])
                        renderSquare(arr, rect, texBorder[tile->borders[3]], Direction::East);
                };
                renderSquare(arr, rect, texPallete, palleteBorderColor);

                // get light borders
                rect.left = ws.x - 160;
                Border borders[4] {
                    selLight->getBorder(lightState, 0),
                    selLight->getBorder(lightState, 1),
                    selLight->getBorder(lightState, 2),
                    selLight->getBorder(lightState, 3)
                };

                // render light borders
                if (borders[0])
                    renderSquare(arr, rect, texBorder[borders[0]], Direction::North);
                if (borders[1])
                    renderSquare(arr, rect, texBorder[borders[1]], Direction::South);
                if (borders[2])
                    renderSquare(arr, rect, texBorder[borders[2]], Direction::West);
                if (borders[3])
                    renderSquare(arr, rect, texBorder[borders[3]], Direction::East);
                renderSquare(arr, rect, texPallete, palleteSelected);

                // calculate resulting borders
                rect.left = ws.x - 80;
                Border rbs[4] {
                    static_cast<Border>(tile->borders[0] & borders[0]),
                    static_cast<Border>(tile->borders[1] & borders[1]),
                    static_cast<Border>(tile->borders[2] & borders[2]),
                    static_cast<Border>(tile->borders[3] & borders[3])
                };

                // render resulting borders
                if (rbs[0])
                    renderSquare(arr, rect, texBorder[rbs[0]], Direction::North);
                if (rbs[1])
                    renderSquare(arr, rect, texBorder[rbs[1]], Direction::South);
                if (rbs[2])
                    renderSquare(arr, rect, texBorder[rbs[2]], Direction::West);
                if (rbs[3])
                    renderSquare(arr, rect, texBorder[rbs[3]], Direction::East);
                renderSquare(arr, rect, texPallete, palleteBorderColor);

                // render buttons
                renderSquare(arr, IntRect(ws.x - 240, 216, 32, 32), texDirection[0]);
                renderSquare(arr, IntRect(ws.x - 192, 216, 32, 32), texDirection[1]);
                renderSquare(arr, IntRect(ws.x - 144, 216, 32, 32), texSpeed[2]);
                renderSquare(arr, IntRect(ws.x - 96, 216, 32, 32), texDirection[2]);
                renderSquare(arr, IntRect(ws.x - 48, 216, 32, 32), texDirection[3]);

                // render text BGs
                renderSquare(arr, IntRect(ws.x - palleteDist, 310, palleteDist, 5), palleteBorderColor);

                renderSquare(arr, IntRect(ws.x - 240, 260, 224, 40), palleteBorderColor);
                renderSquare(arr, IntRect(ws.x - 240, 320, 224, 40), palleteBorderColor);
                renderSquare(arr, IntRect(ws.x - 240, 370, 224, 40), palleteBorderColor);
                renderSquare(arr, IntRect(ws.x - 240, 420, 224, 40), palleteBorderColor);
                renderSquare(arr, IntRect(ws.x - 240, 520, 224, 40), colorTextInvalid);

                // render light
                renderSquare(arr, IntRect(ws.x - 64, 256, 48, 48), texLED[selLight->getLightColor(lightState)]);

                // render corner buttons
                renderSquare(arr, IntRect(ws.x - 240, 470, 40, 40), palleteBorderColor);
                renderSquare(arr, IntRect(ws.x - 179, 470, 40, 40), palleteBorderColor);
                renderSquare(arr, IntRect(ws.x - 117, 470, 40, 40), palleteBorderColor);
                renderSquare(arr, IntRect(ws.x - 56, 470, 40, 40),  palleteBorderColor);
                renderSquare(arr, IntRect(ws.x - 236, 474, 32, 32), selLight->getCorner(8) ? colorTextValid : colorTextInvalid);
                renderSquare(arr, IntRect(ws.x - 175, 474, 32, 32), selLight->getCorner(4) ? colorTextValid : colorTextInvalid);
                renderSquare(arr, IntRect(ws.x - 113, 474, 32, 32), selLight->getCorner(2) ? colorTextValid : colorTextInvalid);
                renderSquare(arr, IntRect(ws.x - 52, 474, 32, 32),  selLight->getCorner(1) ? colorTextValid : colorTextInvalid);
            };
        };
        break;

        // prevent warnings
        default:
        break;
    };

    // render menu
    renderSquare(arr, IntRect(ws.x - 64 * 3, 0, 64, 64), texMode[mode]);
    renderSquare(arr, IntRect(ws.x - 64 * 2, 0, 64, 64), texSpeed[speed]);
    renderSquare(arr, IntRect(ws.x - 64 * 1, 0, 64, 64), texMenu);

    // display pallete
    app.draw(arr, &vtex);

    // display pallete text
    renderText(modeName[mode], ws.x - palleteDist / 2, 12 + 64, colorTextSpecial, 32, true);
    if (mode == Edit::Lights) {
        if (select == Vector2i(-1, 0)) {
            renderText(lightWarning, ws.x - palleteDist / 2, 12 + 128, colorTextMarked, 16, true);

            // render delete button
            renderText(lightNew, ws.x - 128, 184, colorTextDefault, 24, true);
        } else {
            renderText(stateName[lightState], ws.x - palleteDist / 2, 12 + 104, colorTextMarked, 20, true);

            // render state ticks
            char text[20];
            wt ticks = selLight->getLightTicks(lightState);
            sprintf(text, "%d'", ticks / 60);
            renderText(text, ws.x - 204, 265, colorTextDefault, 24, true);
            sprintf(text, "%d\"", ticks % 60);
            renderText(text, ws.x - 132, 265, colorTextDefault, 24, true);

            // render initial ticks
            ticks = selLight->getInitialTicks();
            sprintf(text, "Pause:");
            renderText(text, ws.x - 224, 324, colorTextDefault, 24);
            sprintf(text, "%d'", ticks / 60);
            renderText(text, ws.x - 108, 324, colorTextDefault, 24, true);
            sprintf(text, "%d\"", ticks % 60);
            renderText(text, ws.x - 50, 324, colorTextDefault, 24, true);

            // render position
            Vector2i pos = selLight->getCoords();
            sprintf(text, "Pos:");
            renderText(text, ws.x - 224, 374, colorTextDefault, 24);
            sprintf(text, "%d", pos.x);
            renderText(text, ws.x - 128, 374, colorTextDefault, 24, true);
            sprintf(text, "%d", pos.y);
            renderText(text, ws.x - 56, 374, colorTextDefault, 24, true);

            // render visibility
            sprintf(text, "Visible: %s", booleanName[selLight->visible()]);
            renderText(text, ws.x - 224, 424, colorTextDefault, 24);

            // render delete button
            renderText(lightDelete, ws.x - 128, 524, colorTextDefault, 24, true);
        };
    } else if (mode == Edit::View) {
        char text[65];

        // render name
        sprintf(text, "Name: %s", world.getName());
        renderText(text, ws.x - 240, 128, colorTextDefault, 24);

        // render size
        sprintf(text, "Size: %dx%d", world.getSize().x, world.getSize().y);
        renderText(text, ws.x - 240, 160, colorTextDefault, 24);

    };

    // return quad count
    return (arr.getVertexCount() + vc) / 4;
};
