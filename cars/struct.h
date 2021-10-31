// -- structure definitions -- //

// tile buffer structure
struct TileBuf {
    Border borders[4];
    class Car* pf;
    bool occ;
    bt edges;

    struct {
        class Car* cp;
        Direction d;
        int dx;
        int dy;
    } cd;

    // constructor
    TileBuf() {
        reset();
    };

    // reset to std values
    void reset() {
        borders[0] = Border::Full;
        borders[1] = Border::Full;
        borders[2] = Border::Full;
        borders[3] = Border::Full;
        edges = 0x0;
        pf = null;
        cd.cp = null;
        occ = false;
    };
};

// tile structure
struct Tile {
    Border borders[4];
    Tunnel tunnel;
    Service service;
    bt corners;
    bt sides;
    bt marks;
    bt tile;
    TileBuf buf;

    // setters
    void setA(bt v) {
        borders[0] = static_cast<Border>(v >> 6);
        borders[1] = static_cast<Border>(v >> 4 & 3);
        borders[2] = static_cast<Border>(v >> 2 & 3);
        borders[3] = static_cast<Border>(v & 3);
    };
    void setB(bt v) {
        tile = v;
    };
    void setC(bt v) {
        tunnel = static_cast<Tunnel>(v >> 5 & 3);
        marks = v & 31;
    };
    void setD(bt v) {
        corners = v & 15;
        sides = v >> 4;
    };
    void setE(bt v) {
        service = static_cast<Service>(v & 3);
    };
};

// car specs structure
struct CarSpecs {
    int speed;
    int look;

    // constructors
    CarSpecs(int look) {
        this->look = look;
    };
    CarSpecs(int speed, int look) {
        this->speed = speed;
        this->look = look;
    };
};

// car action structure
struct Action {
    enum ActionType {
        Wait, MoveNorth, MoveSouth, MoveWest, MoveEast, EnterTunnel, ExitTunnel, Idle
    };
    ActionType id;
    int param;

    // constructor
    Action(ActionType action, int parameter = 0) {
        param = parameter;
        id = action;
    };
};
typedef Action::ActionType ActType;

// light frame structure
struct LightFrame {
    Border borders[4];
    wt ticks;
    bool state;

    // constructor
    LightFrame() {};
    LightFrame(bt b, wt ticks, bool state) {
        this->ticks = ticks;
        this->state = state;
        borders[0] = static_cast<Border>(b >> 6);
        borders[1] = static_cast<Border>(b >> 4 & 3);
        borders[2] = static_cast<Border>(b >> 2 & 3);
        borders[3] = static_cast<Border>(b & 3);
    };
};

// light template
struct LightTemplate {
    bool s1, s2;
    bool v;
    bt b1, b2;
    wt t1, t2;
    wt it;
    wt x;
    wt y;
    bt c;

    // constructor
    LightTemplate() {};
    LightTemplate(bt vc, bt vb, bt va, bt v9, bt v8, bt v7, bt v6, bt v5, bt v4, bt v3, bt v2, bt v1, bt v0) {
        x = v0 | v1 << 8;
        y = v2 | v3 << 8;
        v = v4 & 16;
        c = v4 & 15;
        b1 = v5;
        t1 = v6 | (v7 & 0x7F) << 8;
        s1 = v7 >> 7;
        b2 = v8;
        t2 = v9 | (va & 0x7F) << 8;
        s2 = va >> 7;
        it = vb | vc << 8;
    };
};

// map structure
struct Map {
    bool valid = false;
    Vector2i size;
    Tile* data;

    char* name;
    int ns;

    vector<LightTemplate> lights;
    int lcount;
};
