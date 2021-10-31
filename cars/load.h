// -- load manager -- //

// variables
Texture vtex;
Font vfont;
Text vtext;

// load function
bool loadFiles() {
    // load font
    if (!vfont.loadFromFile(fileFont)) {
        return true;
    };
    vtext.setFont(vfont);
    vtext.setOutlineColor(colorLine);
    vtext.setOutlineThickness(textOutline);

    // load images
    if (!vtex.loadFromFile(fileTiles)) {
        return true;
    };

    // success
    return false;
};

// load map file
Map loadMap(st filename) {
    FILE* fp = fopen(filename, "rb");
    long fs;

    // check if open failed
    if (fp == null)
        return Map();

    // get file size
    fseek(fp, 0, 2);
    fs = ftell(fp);
    fseek(fp, 0, 0);

    // get header
    if (fs < 16)
        return Map();
    bt buffer[16];
    for (int i = 0; i < 16; i++)
        buffer[i] = fgetc(fp);

    // get map size
    Map map;
    map.size.x = buffer[0] | buffer[1] << 8;
    map.size.y = buffer[2] | buffer[3] << 8;
    int s = map.size.x * map.size.y;

    // get lights count
    map.lcount = buffer[4] | buffer[5] << 8 | buffer[6] << 16;

    // check size
    if (fs < map.lcount * 13 + s * 5 + 16)
        return Map();

    // fetch map data
    map.data = new Tile[s];

    if (map.data == null)
        return Map();

    for (int i = 0; i < s; i++)
        map.data[i].setA(fgetc(fp));
    for (int i = 0; i < s; i++)
        map.data[i].setB(fgetc(fp));
    for (int i = 0; i < s; i++)
        map.data[i].setC(fgetc(fp));
    for (int i = 0; i < s; i++)
        map.data[i].setD(fgetc(fp));
    for (int i = 0; i < s; i++)
        map.data[i].setE(fgetc(fp));

    // fetch lights
    for (int i = 0; i < map.lcount; i++) {
        map.lights.push_back(LightTemplate(fgetc(fp), fgetc(fp), fgetc(fp), fgetc(fp), fgetc(fp), fgetc(fp), fgetc(fp), fgetc(fp), fgetc(fp), fgetc(fp), fgetc(fp), fgetc(fp), fgetc(fp)));
    };

    // fetch name
    int ns = fs - (map.lcount * 13 + s * 5 + 16);
    map.name = new char[ns + 1];
    for (int i = 0; i < ns; i++) {
        map.name[i] = fgetc(fp);
    };

    map.name[ns] = 0;
    map.ns = ns;

    // success
    fclose(fp);
    map.valid = true;
    return map;
};

// save map file
bool saveMap(st filename, Map& map) {
    FILE* fp = fopen(filename, "wb");

    // check if open failed
    if (fp == null)
        return false;

    // write header
    // write map size
    fputc(map.size.x & 0xFF, fp);
    fputc(map.size.x >> 8 & 0xFF, fp);
    fputc(map.size.y & 0xFF, fp);
    fputc(map.size.y >> 8 & 0xFF, fp);
    // write lights count
    fputc(map.lcount & 0xFF, fp);
    fputc(map.lcount >> 8 & 0xFF, fp);
    fputc(map.lcount >> 16 & 0xFF, fp);
    for (int i = 0; i < 9; i++)
        fputc(0x00, fp);

    // write section A
    for (int y = 0; y < map.size.y; y++) {
        for (int x = 0; x < map.size.x; x++) {
            Tile* tile = map.data + y * map.size.x + x;
            bt a = tile->borders[0] << 6 | tile->borders[1] << 4 | tile->borders[2] << 2 | tile->borders[3];
            fputc(a, fp);
        };
    };
    // write section B
    for (int y = 0; y < map.size.y; y++) {
        for (int x = 0; x < map.size.x; x++) {
            Tile* tile = map.data + y * map.size.x + x;
            bt b = tile->tile;
            fputc(b, fp);
        };
    };
    // write section C
    for (int y = 0; y < map.size.y; y++) {
        for (int x = 0; x < map.size.x; x++) {
            Tile* tile = map.data + y * map.size.x + x;
            bt c = tile->marks | tile->tunnel << 5;
            fputc(c, fp);
        };
    };
    // write section D
    for (int y = 0; y < map.size.y; y++) {
        for (int x = 0; x < map.size.x; x++) {
            Tile* tile = map.data + y * map.size.x + x;
            bt d = tile->corners | tile->sides << 4;
            fputc(d, fp);
        };
    };
    // write section E
    for (int y = 0; y < map.size.y; y++) {
        for (int x = 0; x < map.size.x; x++) {
            Tile* tile = map.data + y * map.size.x + x;
            bt e = tile->service;
            fputc(e, fp);
        };
    };

    // write lights
    for (int i = 0; i < map.lcount; i++) {
        fputc(map.lights[i].x & 0xFF, fp);
        fputc(map.lights[i].x >> 8, fp);
        fputc(map.lights[i].y & 0xFF, fp);
        fputc(map.lights[i].y >> 8, fp);
        fputc(map.lights[i].c | map.lights[i].v << 4, fp);
        fputc(map.lights[i].b1, fp);
        fputc(map.lights[i].t1 & 0xFF, fp);
        fputc((map.lights[i].t1 >> 8) | (map.lights[i].s1 << 7), fp);
        fputc(map.lights[i].b2, fp);
        fputc(map.lights[i].t2 & 0xFF, fp);
        fputc((map.lights[i].t2 >> 8) | (map.lights[i].s2 << 7), fp);
        fputc(map.lights[i].it & 0xFF, fp);
        fputc(map.lights[i].it >> 8, fp);
    };

    // write name
    for (int i = 0; i < map.ns; i++) {
        fputc(map.name[i], fp);
    };

    // success
    fclose(fp);
    return true;
};
