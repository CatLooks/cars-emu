// -- project data -- //

// control data
const int steerDist = 32;
const int zoomVars = 8;
const int zoomValues[zoomVars] {
    16, 24, 32, 48, 64, 96, 128, 256
};
const int moveValues[zoomVars] {
    16, 18, 20, 21, 22, 22, 17, 12
};

// road speed
const bt tileCount = 9;
const bool isBuilding[tileCount] {
    0, 0, 0, 0, 0, 0, 1, 0, 0
};
const bt roadType[tileCount] {
    0, 0, 1, 1, 1, 0, 2, 2, 2
};
const bt roadSpeed[] {
    255, 160, 240
};

// static tiles
const bt TILE_VOID  = 0;
const bt TILE_ROAD  = 1;
const bt TILE_PARK  = 2;
const bt TILE_GRASS = 3;
const bt TILE_DIRT  = 4;
const bt TILE_FLOOR = 5;
const bt TILE_CEIL  = 6;
const bt TILE_WATER = 7;
const bt TILE_BRICK = 8;

const IntRect texService[3] {
    IntRect(208, 64, 16, 16),
    IntRect(224, 64, 16, 16),
    IntRect(240, 64, 16, 16)
};
const IntRect texRoad  = IntRect(8  * 16, 16 * 0, 16, 16);
const IntRect texPark  = IntRect(9  * 16, 16 * 0, 16, 16);
const IntRect texDirt  = IntRect(10 * 16, 16 * 0, 16, 16);
const IntRect texBrick = IntRect(10 * 16, 16 * 1, 16, 16);
const IntRect texBuilding[] {
    IntRect(16 * 8,  16 * 1, 16, 16),
    IntRect(16 * 9,  16 * 1, 16, 16),
};

const IntRect texTunnel[3] {
    IntRect(128, 48, 16, 16),
    IntRect(144, 48, 16, 16),
    IntRect(128, 64, 16, 16)
};
const IntRect texBorder[4] {
    IntRect(160, 48, 16, 16),
    IntRect(176, 48, 16, 16),
    IntRect(192, 48, 16, 16),
    IntRect(208, 48, 16, 16)
};

// tile pallete
const Color palleteBorderColor = Color(0x202128FF);
const Color palleteColor       = Color(0x383A44FF);
const Color palleteSelected    = Color(0xFFF040FF);
const int palleteBorderWidth = 4;
const int palleteDist = 256;
const int palleteTileCount[9] {
    0, tileCount, 16, 16, 19, 20, 4, 4, 0
};

// neighbor tiles data
const int neightborOpposite[8] {
    1, 0, 3, 2, 7, 6, 5, 4
};
const Vector2i neighborList[8] {
    Vector2i( 0, -1),
    Vector2i( 0,  1),
    Vector2i(-1,  0),
    Vector2i( 1,  0),
    Vector2i(-1, -1),
    Vector2i( 1, -1),
    Vector2i(-1,  1),
    Vector2i( 1,  1)
};

// misc
const Direction relativeDirection[4][4] {
    {Direction::North, Direction::South, Direction::West, Direction::East},
    {Direction::South, Direction::North, Direction::East, Direction::West},
    {Direction::West, Direction::East, Direction::South, Direction::North},
    {Direction::East, Direction::West, Direction::North, Direction::South}
};
const ActType directionToAction[4] {
    Action::MoveNorth,
    Action::MoveSouth,
    Action::MoveWest,
    Action::MoveEast
};
const IntRect texLightTile = IntRect(128, 96, 16, 16);
const IntRect texPallete = IntRect(240, 48, 16, 16);
const IntRect texCursor = IntRect(224, 48, 16, 16);

// directions
const IntRect texDirection[4] {
    IntRect(144, 96, 16, 16),
    IntRect(160, 96, 16, 16),
    IntRect(176, 96, 16, 16),
    IntRect(192, 96, 16, 16)
};

// car data
const int maxPathfindIterations = 16384;
const int turnSlowdown = 4;

const int BORDER_IN  = 2;
const int BORDER_OUT = 1;

// menu tiles
const IntRect texMenuBG = IntRect(128, 112, 16, 16);
const IntRect texMenu  = IntRect(192, 64, 16, 16);
const IntRect texReset = IntRect(240, 96, 16, 16);
const IntRect texSpeed[3] {
    IntRect(144, 64, 16, 16),
    IntRect(160, 64, 16, 16),
    IntRect(176, 64, 16, 16)
};
const IntRect texMode[9] {
    IntRect(0, 0, 0, 0),
    IntRect(128, 80, 16, 16),
    IntRect(144, 80, 16, 16),
    IntRect(176, 80, 16, 16),
    IntRect(160, 80, 16, 16),
    IntRect(192, 80, 16, 16),
    IntRect(208, 80, 16, 16),
    IntRect(224, 80, 16, 16),
    IntRect(240, 80, 16, 16)
};

// tunnel data
const Tunnel tunnelTransition[4] {
    Tunnel::Vertical,
    Tunnel::Vertical,
    Tunnel::Horizontal,
    Tunnel::Horizontal
};

// colors
const Vector2f texColor = Vector2f(24, 69);
const Color colorVoid = Color(0x101114FF);
const Color colorEdge = Color(0x606270FF);
const Color colorHalf = Color(0xFFFFFF3F);
const Color colorDark = Color(0xFFFFFF9F);
const Color colorBack = Color(0x909090FF);
const Color colorRug  = Color(0x181922FF);
const Color colorPave = Color(0x404250FF);
const Color colorDirt = Color(0x30A266FF);

// text
const Color colorLine        = Color(0x000000FF);
const Color colorTextDefault = Color(0xF0F0F0FF);
const Color colorTextSpecial = Color(0xF000F0FF);
const Color colorTextMarked  = Color(0xFFF040FF);
const Color colorTextInvalid = Color(0xFF2040FF);
const Color colorTextValid   = Color(0x20E040FF);
const int textOutline = 2;

// tile animations
const int animationFrames = 4;
const int animationSpeed = 12;
const IntRect texGrass[animationFrames] {
    IntRect(12 * 16, 0, 16, 16),
    IntRect(13 * 16, 0, 16, 16),
    IntRect(14 * 16, 0, 16, 16),
    IntRect(15 * 16, 0, 16, 16)
};
const IntRect texWater[animationFrames] {
    IntRect(12 * 16, 16, 16, 16),
    IntRect(13 * 16, 16, 16, 16),
    IntRect(14 * 16, 16, 16, 16),
    IntRect(15 * 16, 16, 16, 16)
};

// road corners
const bool shouldRenderCorners[16] {
    1, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 1, 0, 0, 0, 0, 0
};
const IntRect texRoadCorners[16] {
    IntRect(0 * 16, 2 * 16, 16, 16),
    IntRect(1 * 16, 2 * 16, 16, 16),
    IntRect(2 * 16, 2 * 16, 16, 16),
    IntRect(3 * 16, 2 * 16, 16, 16),
    IntRect(4 * 16, 2 * 16, 16, 16),
    IntRect(5 * 16, 2 * 16, 16, 16),
    IntRect(6 * 16, 2 * 16, 16, 16),
    IntRect(7 * 16, 2 * 16, 16, 16),
    IntRect(0 * 16, 3 * 16, 16, 16),
    IntRect(1 * 16, 3 * 16, 16, 16),
    IntRect(2 * 16, 3 * 16, 16, 16),
    IntRect(3 * 16, 3 * 16, 16, 16),
    IntRect(4 * 16, 3 * 16, 16, 16),
    IntRect(5 * 16, 3 * 16, 16, 16),
    IntRect(6 * 16, 3 * 16, 16, 16),
    IntRect(7 * 16, 3 * 16, 16, 16)
};
// road borders
const IntRect texRoadBorders[16] {
    IntRect(0 * 16, 0 * 16, 16, 16),
    IntRect(1 * 16, 0 * 16, 16, 16),
    IntRect(2 * 16, 0 * 16, 16, 16),
    IntRect(3 * 16, 0 * 16, 16, 16),
    IntRect(4 * 16, 0 * 16, 16, 16),
    IntRect(5 * 16, 0 * 16, 16, 16),
    IntRect(6 * 16, 0 * 16, 16, 16),
    IntRect(7 * 16, 0 * 16, 16, 16),
    IntRect(0 * 16, 1 * 16, 16, 16),
    IntRect(1 * 16, 1 * 16, 16, 16),
    IntRect(2 * 16, 1 * 16, 16, 16),
    IntRect(3 * 16, 1 * 16, 16, 16),
    IntRect(4 * 16, 1 * 16, 16, 16),
    IntRect(5 * 16, 1 * 16, 16, 16),
    IntRect(6 * 16, 1 * 16, 16, 16),
    IntRect(7 * 16, 1 * 16, 16, 16)
};
// road marks
const IntRect texRoadMarks[32] {
    IntRect(0 * 16, 4 * 16, 16, 16),
    IntRect(1 * 16, 4 * 16, 16, 16),
    IntRect(2 * 16, 4 * 16, 16, 16),
    IntRect(3 * 16, 4 * 16, 16, 16),
    IntRect(4 * 16, 4 * 16, 16, 16),
    IntRect(5 * 16, 4 * 16, 16, 16),
    IntRect(6 * 16, 4 * 16, 16, 16),
    IntRect(7 * 16, 4 * 16, 16, 16),
    IntRect(0 * 16, 5 * 16, 16, 16),
    IntRect(1 * 16, 5 * 16, 16, 16),
    IntRect(2 * 16, 5 * 16, 16, 16),
    IntRect(3 * 16, 5 * 16, 16, 16),
    IntRect(4 * 16, 5 * 16, 16, 16),
    IntRect(5 * 16, 5 * 16, 16, 16),
    IntRect(6 * 16, 5 * 16, 16, 16),
    IntRect(7 * 16, 5 * 16, 16, 16),
    IntRect(0 * 16, 6 * 16, 16, 16),
    IntRect(1 * 16, 6 * 16, 16, 16),
    IntRect(2 * 16, 6 * 16, 16, 16),
    IntRect(3 * 16, 6 * 16, 16, 16),
    IntRect(4 * 16, 6 * 16, 16, 16),
    IntRect(5 * 16, 6 * 16, 16, 16),
    IntRect(6 * 16, 6 * 16, 16, 16),
    IntRect(7 * 16, 6 * 16, 16, 16),
    IntRect(0 * 16, 7 * 16, 16, 16),
    IntRect(1 * 16, 7 * 16, 16, 16),
    IntRect(2 * 16, 7 * 16, 16, 16),
    IntRect(3 * 16, 7 * 16, 16, 16),
    IntRect(4 * 16, 7 * 16, 16, 16),
    IntRect(5 * 16, 7 * 16, 16, 16),
    IntRect(6 * 16, 7 * 16, 16, 16),
    IntRect(7 * 16, 7 * 16, 16, 16),
};

// car textures
const int carSpawnRate = 193;
const int carCount = 4;
const int carSpeed[carCount] {
    6, 5, 8, 3
};

const IntRect texCar[carCount] {
    IntRect(8  * 16, 9  * 16, 16, 16),
    IntRect(9  * 16, 9  * 16, 16, 16),
    IntRect(10 * 16, 9  * 16, 16, 16),
    IntRect(11 * 16, 9  * 16, 16, 16),
    /*IntRect(12 * 16, 9  * 16, 16, 16),
    IntRect(13 * 16, 9  * 16, 16, 16),
    IntRect(14 * 16, 9  * 16, 16, 16),
    IntRect(15 * 16, 9  * 16, 16, 16),
    IntRect(8  * 16, 10 * 16, 16, 16),
    IntRect(9  * 16, 10 * 16, 16, 16),
    IntRect(10 * 16, 10 * 16, 16, 16),
    IntRect(11 * 16, 10 * 16, 16, 16),
    IntRect(12 * 16, 10 * 16, 16, 16),
    IntRect(13 * 16, 10 * 16, 16, 16),
    IntRect(14 * 16, 10 * 16, 16, 16),
    IntRect(15 * 16, 10 * 16, 16, 16)*/
};
const IntRect texCarIcon = IntRect(192, 128, 16, 16);

// traffic lights
const int lightYellowTicksLeft = 24;
const Vector2i texLightOffset[4] {
    Vector2i(-2, -12),
    Vector2i(15, -12),
    Vector2i(-2, 4),
    Vector2i(15, 4)
};
const IntRect texLight[4] {
    IntRect(176, 0, 4, 16),
    IntRect(180, 0, 4, 16),
    IntRect(184, 0, 4, 16),
    IntRect(188, 0, 4, 16)
};
const IntRect texLED[2] {
    IntRect(208, 96, 16, 16),
    IntRect(224, 96, 16, 16)
};

// debug info
const Color debugColor[4] {
    Color(0x00000000),
    Color(0x00FFFFC0),
    Color(0xFF0000C0),
    Color(0xFF00FFC0)
};
const IntRect debugSide[4] {
    IntRect(128, 32, 16, 16),
    IntRect(144, 32, 16, 16),
    IntRect(160, 32, 16, 16),
    IntRect(176, 32, 16, 16),
};

// menu buttons
const IntRect texButtonSelect = IntRect(176, 128, 16, 16);
const IntRect texMenuButton[10] {
    IntRect(144, 112, 16, 16),
    IntRect(160, 112, 16, 16),
    IntRect(176, 112, 16, 16),
    IntRect(192, 112, 16, 16),
    IntRect(208, 112, 16, 16),
    IntRect(224, 112, 16, 16),
    IntRect(240, 112, 16, 16),
    IntRect(128, 128, 16, 16),
    IntRect(144, 128, 16, 16),
    IntRect(160, 128, 16, 16)
};
