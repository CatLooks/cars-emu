// -- world buffer updater -- //

// reset edges
void resetEdges(World& world) {
    for (int y = 0; y < world.height(); y++) {
        for (int x = 0; x < world.width(); x++) {
            Tile* tile = world.ftile(Vector2i(x, y));
            if (tile == null)
                continue;

            // proceed if tile is not void
            if (tile->tile == TILE_VOID)
                continue;

            // check each neighbor
            for (int nbi = 0; nbi < 8; nbi++) {
                Tile* nb = world.ntile(Vector2i(x, y), nbi);
                if (nb) { // prevent crashes
                    if (nb->tile == TILE_VOID) {
                        // enable outside border
                        nb->buf.edges |= 1 << neightborOpposite[nbi];
                        continue;
                    };
                };
            };
        };
    };
};

// update edge buffer
void bufferEdges(World& world) {
    for (int y = 0; y < world.height(); y++) {
        for (int x = 0; x < world.width(); x++) {
            Tile* tile = world.ftile(Vector2i(x, y));
            if (tile == null)
                continue;
            tile->buf.edges = 0;
        };
    };
};

// update type vectors
void bufferService(World& world, vector<Vector2i>& v, Service s) {
    v.clear();

    for (int y = 0; y < world.height(); y++) {
        for (int x = 0; x < world.width(); x++) {
            Tile* tile = world.ftile(Vector2i(x, y));
            if (tile == null)
                continue;

            if (tile->service == s)
                v.push_back(Vector2i(x, y));
        };
    };
};
