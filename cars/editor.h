// -- editor manager -- //

// select tile
void selectTile() {

};

// select coordinate
void selectCoord(World& world, bool flag) {
    // get tile
    Tile* tile = world.ftile(cursor);
    if (tile == null)
        return;

    // if no selected tile, exit
    if (selTile == -1)
        return;

    switch (mode) {
        case Edit::Tiles: {
            // replace texture
            tile->tile = flag ? selTile : 0;
            //bufferEdges(world);
        };
        break;

        case Edit::Sides: {
            // replace texture
            tile->sides = flag ? selTile : 0;
        };
        break;

        case Edit::Corners: {
            // replace texture
            tile->corners = flag ? selTile : 0;
        };
        break;

        case Edit::Marks: {
            // replace texture
            tile->marks = flag ? selTile : 0;
        };
        break;

        case Edit::Borders: {
            // get new attributes
            int dir;
            if (selTile < 16)
                dir = static_cast<Direction>(selTile >> 2);
            else
                dir = 4;

            Border nbi = Border::None;
            if (flag)
                nbi = static_cast<Border>(selTile & 3);

            // make changes
            if (dir == 4) {
                tile->borders[0] = nbi;
                tile->borders[1] = nbi;
                tile->borders[2] = nbi;
                tile->borders[3] = nbi;
            } else {
                tile->borders[dir] = nbi;
            };
        };
        break;

        case Edit::Tunnels: {
            // replace tunnel
            tile->tunnel = static_cast<Tunnel>(flag ? selTile : 0);
        };
        break;

        case Edit::Parking: {
            // (re)set parking
            tile->service = static_cast<Service>(flag ? selTile : 0);
        };
        break;

        // prevent warnings
        default:
        break;
    };
};
