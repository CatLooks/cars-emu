// -- world manager -- //

// world object
class World {
    public:
    // constructor
    World() {

    };
    // destructor
    ~World() {
        if (data)
            delete[] data;
    };

    // load map
    void load(Map map) {
        if (map.valid == false)
            return;

        // copy data
        size = map.size;
        data = map.data;
        name = map.name;
    };
    // dump map
    void dump(Map& m) {
        m.size = Vector2i(size.x, size.y);
        m.data = data;
        m.name = name;
        m.valid = true;
    };

    // validate coordinate
    bool invalid(Vector2i pos) {
        return pos.x < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y;
    };

    // access tiles
    Tile* ftile(Vector2i pos) {
        if (invalid(pos))
            return null;
        return data + (pos.y * size.y + pos.x);
    };
    TileBuf* fbuf(Vector2i pos) {
        if (invalid(pos))
            return null;
        return &(data + (pos.y * size.y + pos.x))->buf;
    };
    Tile* ntile(Vector2i pos, int nbi) {
        return ftile(pos + neighborList[nbi]);
    };
    TileBuf* nbuf(Vector2i pos, int nbi) {
        return fbuf(pos + neighborList[nbi]);
    };

    // variable access
    int width()  { return size.x; };
    int height() { return size.y; };

    // center camera
    void centerCamera() {
        camera.x = size.x * (wts / 2);
        camera.y = size.y * (wts / 2);
    };

    // try to move to tile
    /**
     *  0 - can't move
     *  1 - can move
     *  2 - move in tunnel
     *  3 - move out of tunnel
    **/
    bt tmt(class Car* cp, int dx, int dy, Direction dir, bool inTunnel) {
        if (inTunnel) {
            // check ground tile
            Tile* root = ftile(Vector2i(dx, dy));
            if (root == null)
                return 0;

            // check next tile
            Tile* next = ntile(Vector2i(dx, dy), dir);
            if (next == null)
                return 0;

            // check if next tile can be tunnel exit
            if (next->tunnel == Tunnel::None) {
                // check if can move from tile
                if (root->tunnel != tunnelTransition[dir])
                    return 0;

                // check if can move to next tile
                //if (next->borders[neightborOpposite[dir]] == Border::In || next->borders[neightborOpposite[dir]] == Border::Full) {
                if (next->borders[neightborOpposite[dir]] & BORDER_IN) {
                    // check if was already on tile
                    if (next->buf.pf == cp)
                        return 0;
                    next->buf.pf = cp;
                    return 3;
                };

                // can't move
                return 0;
            };

            // check if next tunnel is accessible
            if (next->tunnel == tunnelTransition[dir] || next->tunnel == Tunnel::Both) {
                // check if was already on tile
                // if (next->buf.pf == cp)
                //     return 0;
                // next->buf.pf = cp;
                return 1;
            };

            // can't move
            return 0;
        } else {
            // check if can move from tile
            Tile* root = ftile(Vector2i(dx, dy));
            if (root == null)
                return 0;
            if (root->borders[dir] == Border::None || root->borders[dir] == Border::In)
                return 0;

            // check next tile
            Tile* next = ntile(Vector2i(dx, dy), dir);
            if (next == null)
                return 0;

            // check next tile entrance
            //if (next->borders[neightborOpposite[dir]] == Border::In || next->borders[neightborOpposite[dir]] == Border::Full) {
            if (next->borders[neightborOpposite[dir]] & BORDER_IN) {
                // check if was already on tile
                if (next->buf.pf == cp)
                    return 0;
                next->buf.pf = cp;
                return 1;
            };

            // check if next tile is a tunnel
            if (next->tunnel == tunnelTransition[dir] || next->tunnel == Tunnel::Both) {
                // check if was already on tile
                // if (next->buf.pf == cp)
                //     return 0;
                // next->buf.pf = cp;
                return 2;
            };

            // can't move
            return 0;
        };
    };

    // reset pathfind buffer
    void resetPF() {
        for (int y = 0; y < size.y; y++) {
            for (int x = 0; x < size.x; x++) {
                Tile* tile = ftile(Vector2i(x, y));
                if (tile == null)
                    continue;
                tile->buf.pf = null;
            };
        };
    };
    // reset car buffer
    void resetCD() {
        for (int y = 0; y < size.y; y++) {
            for (int x = 0; x < size.x; x++) {
                Tile* tile = ftile(Vector2i(x, y));
                if (tile == null)
                    continue;
                tile->buf.cd.cp = null;
                tile->buf.cd.dx = 0;
                tile->buf.cd.dy = 0;
            };
        };
    };

    // get name
    st getName() {
        return (st)name;
    };

    // get size
    Vector2i getSize() {
        return size;
    };

    private:
    Tile* data;
    Vector2i size;
    char* name;
};

// get tile current border
Border gtb(Tile* tile, Direction dir) {
    return static_cast<Border>(tile->borders[dir] & tile->buf.borders[dir]);
};
