// -- enumerator definitions -- //

// rotation type
struct direct_t {
    enum types {
        North = 0, South, West, East
    };
};
typedef direct_t::types Direction;

// border type
struct border_t {
    enum types {
        None = 0, Out, In, Full
    };
};
typedef border_t::types Border;

// tunnel type
struct tunnel_t {
    enum types {
        None = 0, Horizontal, Vertical, Both
    };
};
typedef tunnel_t::types Tunnel;

// light type
struct light_t {
    enum types {
        Black = 0, Red, Yellow, Green
    };
};
typedef light_t::types tlc;

// tile type
struct tile_t {
    enum types {
        None = 0, Parking, Shop, Fuel
    };
};
typedef tile_t::types Service;

// room switching
struct room_t {
    enum types {
        Game, Menu, Editor
    };
};
typedef room_t::types Room;

// editor mode
struct edit_t {
    enum types {
        View = 0, Tiles, Sides, Corners, Marks, Borders, Tunnels, Parking, Lights
    };
};
typedef edit_t::types Edit;

// emulation speed
struct speed_t {
    enum types {
        Pause = 0, Normal, Fast
    };
};
typedef speed_t::types Speed;
