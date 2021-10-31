// -- game logic -- //

// variables
vector<Light> lights;
vector<Car> cars;
World world;

namespace Dests {
    vector<Vector2i> parks;
    vector<Vector2i> shops;
    vector<Vector2i> fuels;
};

// spawn random-spec car
void spawnCar(int x, int y) {
    // validate position
    if (x == -1)
        return;

    int look = rint(carCount);
    int speed = carSpeed[look] + rint(-1, 2);
    int dir = rint(4);

    cars.push_back(Car(CarSpecs(speed, look), x, y, static_cast<Direction>(dir)));
};

// spawn cars on parkings
void spawnParkedCars() {
    for (Vector2i& pos : Dests::parks) {
        int chance = rand() & 0xFF;
        if (chance < carSpawnRate) {
            spawnCar(pos.x, pos.y);
        };
    };
};

// create function
void onCreate() {
    // load all maps
    char text[33];
    for (int t = 0; t < 10; t++) {
        sprintf(text, fileMap, t);
        menuMaps[t] = loadMap(text);
    };
};

// new function
void onNew() {
    Map& m = menuMaps[menuSelect];
    m.size = Vector2i(256, 256);

    // free resources
    if (m.data)
        delete[] m.data;
    if (m.name)
        delete[] m.name;

    // create empty space
    m.data = new Tile[m.size.x * m.size.y];
    for (int y = 0; y < m.size.y; y++) {
        for (int x = 0; x < m.size.x; x++) {
            m.data[y * m.size.x + x].tile = 0;
        };
    };

    m.data[126 * 256 + 126].tile = 2;
    m.data[126 * 256 + 127].tile = 2;
    m.data[126 * 256 + 128].tile = 2;
    m.data[126 * 256 + 129].tile = 2;
    m.data[127 * 256 + 126].tile = 2;
    m.data[127 * 256 + 127].tile = 1;
    m.data[127 * 256 + 128].tile = 1;
    m.data[127 * 256 + 129].tile = 2;
    m.data[128 * 256 + 126].tile = 2;
    m.data[128 * 256 + 127].tile = 1;
    m.data[128 * 256 + 128].tile = 1;
    m.data[128 * 256 + 129].tile = 2;
    m.data[129 * 256 + 126].tile = 2;
    m.data[129 * 256 + 127].tile = 2;
    m.data[129 * 256 + 128].tile = 2;
    m.data[129 * 256 + 129].tile = 2;

    m.name = new char[1];
    m.name[0] = 0;
    m.ns = 0;
    m.lights.clear();
    m.lcount = 0;
    m.valid = true;

    // create a name
    m.name = new char[7];
    sprintf(m.name, "Map %d", menuSelect + 1);
    m.ns = 6;

    // load map to world
    world.load(m);

    // update visual details
    world.centerCamera();
    //bufferEdges(world);

    // update types
    bufferService(world, Dests::parks, Service::Parking);
    bufferService(world, Dests::shops, Service::Shop);
    bufferService(world, Dests::fuels, Service::Fuel);

    // spawn cars
    /*spawnParkedCars();
    for (Car& car : cars) {
        car.trip(world, Dests::parks, Dests::shops, Dests::fuels);
    };*/
};

// delete function
void onDelete() {
    // disable map
    Map& m = menuMaps[menuSelect];
    m.valid = false;

    // free resources
    if (m.data)
        delete[] m.data;
    if (m.name)
        delete[] m.name;

    // delete file
    char text[33];
    sprintf(text, fileMap, menuSelect);
    remove(text);
};

// load function
void onLoad() {
    // load map to world
    Map& m = menuMaps[menuSelect];
    world.load(m);

    // load lights
    lights.clear();
    for (int i = 0; i < m.lcount; i++) {
        lights.push_back(createFromTemplate(m.lights[i]));
        lights.back().buffer(world);
    };

    // update visual details
    world.centerCamera();
    //bufferEdges(world);

    // update types
    bufferService(world, Dests::parks, Service::Parking);
    bufferService(world, Dests::shops, Service::Shop);
    bufferService(world, Dests::fuels, Service::Fuel);

    // spawn cars
    spawnParkedCars();
    for (Car& car : cars) {
        car.trip(world, Dests::parks, Dests::shops, Dests::fuels);
    };

    //cars.push_back(Car(CarSpecs(4, 2), 139, 126, Direction::West));

    // switch to game room
    room = Room::Game;
};

// save function
void onSave() {
    // check for file validity
    if (!menuMaps[menuSelect].valid)
        return;

    // save tile data
    Map& m = menuMaps[menuSelect];
    world.dump(m);

    // save lights
    m.lcount = lights.size();
    for (Light& light : lights) {
        m.lights.push_back(light.createTemplate());
    };

    // save to file
    char text[33];
    sprintf(text, fileMap, menuSelect);
    saveMap(text, m);
};

// reset function
void onReset() {
    for (Light& light : lights) {
        light.reset();
    };
};

// tick function
void onTick() {
    // update lights
    for (Light& light : lights) {
        light.tick(world);
    };

    // reset car buffer
    world.resetCD();

    // update cars
    list<bool> ticks;
    list<Car*> seen;

    for (Car& car : cars) {
        car.occupy(world);

        if (car.out(ws))
            continue;

        seen.push_back(&car);
        ticks.push_back(car.tick(world));
        car.trip(world, Dests::parks, Dests::shops, Dests::fuels);
    };

    // move cars
    for (Car* car : seen) {
        if (ticks.front() && car->canMove(world)) {
            car->move(world);
        };
        ticks.pop_front();
    };
};

// create new light
void createLight() {
    Vector2i nc(camtile.x + ws.x / wts / 2, camtile.y + ws.y / wts / 2);
    lights.push_back(Light(LightFrame(0x00, 300, false), LightFrame(0xFF, 300, true), 0, nc.x, nc.y, 8, true));

    select = nc;
    selLight = &lights.back();
};

// delete light
void deleteLight() {
    for (int i = 0; i < (int)lights.size(); i++) {
        if (&lights[i] == selLight) {
            Tile* tile = world.ftile(selLight->getCoords());
            if (tile) {
                tile->buf.borders[0] = Border::Full;
                tile->buf.borders[1] = Border::Full;
                tile->buf.borders[2] = Border::Full;
                tile->buf.borders[3] = Border::Full;
            };
            lights.erase(lights.begin() + i);
            break;
        };
    };

    select = Vector2i(-1, 0);
    selLight = null;
};
