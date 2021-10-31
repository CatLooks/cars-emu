// -- initialize data -- //

Speed speed = Speed::Normal;
Edit mode = Edit::Tiles;
Room room = Room::Menu;

Vector2i camtile;
Vector2i cursor;
Vector2i camera;

class Car* selCar = null;
class Car* folCar = null;

Vector2i select(-1, 0);
class Light* selLight;
bool lightState = 0;

int selTile = -1;
int zoomID = 4;
int wts = 64;

bool showTunnels = false;
bool showPipes   = false;
bool debugMode   = false;
bool darkDetails = false;
bool windowFull  = false;

bool menuMode = false;
bt menuSelect = -1;
bt menuOffset = 0;
bt menuTimer = 0;
Map menuMaps[10];

bt animTimer = 0;
bt animFrame = 0;

// render square
void renderSquare(VertexArray& array, IntRect position, Color color = Color::White) {
    array.append(Vertex(Vector2f(position.left,                  position.top),                   color, texColor));
    array.append(Vertex(Vector2f(position.left + position.width, position.top),                   color, texColor));
    array.append(Vertex(Vector2f(position.left + position.width, position.top + position.height), color, texColor));
    array.append(Vertex(Vector2f(position.left,                  position.top + position.height), color, texColor));
};
void renderSquare(VertexArray& array, IntRect position, IntRect texture, Color color = Color::White) {
    array.append(Vertex(Vector2f(position.left,                  position.top),                   color, Vector2f(texture.left,                 texture.top)));
    array.append(Vertex(Vector2f(position.left + position.width, position.top),                   color, Vector2f(texture.left + texture.width, texture.top)));
    array.append(Vertex(Vector2f(position.left + position.width, position.top + position.height), color, Vector2f(texture.left + texture.width, texture.top + texture.height)));
    array.append(Vertex(Vector2f(position.left,                  position.top + position.height), color, Vector2f(texture.left,                 texture.top + texture.height)));
};
void renderSquare(VertexArray& array, IntRect position, IntRect texture, Direction direction, Color color = Color::White) {
    if (direction == Direction::North) {
        array.append(Vertex(Vector2f(position.left,                  position.top),                   color, Vector2f(texture.left,                 texture.top)));
        array.append(Vertex(Vector2f(position.left + position.width, position.top),                   color, Vector2f(texture.left + texture.width, texture.top)));
        array.append(Vertex(Vector2f(position.left + position.width, position.top + position.height), color, Vector2f(texture.left + texture.width, texture.top + texture.height)));
        array.append(Vertex(Vector2f(position.left,                  position.top + position.height), color, Vector2f(texture.left,                 texture.top + texture.height)));
    } else if (direction == Direction::East) {
        array.append(Vertex(Vector2f(position.left,                  position.top),                   color, Vector2f(texture.left,                 texture.top + texture.height)));
        array.append(Vertex(Vector2f(position.left + position.width, position.top),                   color, Vector2f(texture.left,                 texture.top)));
        array.append(Vertex(Vector2f(position.left + position.width, position.top + position.height), color, Vector2f(texture.left + texture.width, texture.top)));
        array.append(Vertex(Vector2f(position.left,                  position.top + position.height), color, Vector2f(texture.left + texture.width, texture.top + texture.height)));
    } else if (direction == Direction::South) {
        array.append(Vertex(Vector2f(position.left,                  position.top),                   color, Vector2f(texture.left + texture.width, texture.top + texture.height)));
        array.append(Vertex(Vector2f(position.left + position.width, position.top),                   color, Vector2f(texture.left,                 texture.top + texture.height)));
        array.append(Vertex(Vector2f(position.left + position.width, position.top + position.height), color, Vector2f(texture.left,                 texture.top)));
        array.append(Vertex(Vector2f(position.left,                  position.top + position.height), color, Vector2f(texture.left + texture.width, texture.top)));
    } else {
        array.append(Vertex(Vector2f(position.left,                  position.top),                   color, Vector2f(texture.left + texture.width, texture.top)));
        array.append(Vertex(Vector2f(position.left + position.width, position.top),                   color, Vector2f(texture.left + texture.width, texture.top + texture.height)));
        array.append(Vertex(Vector2f(position.left + position.width, position.top + position.height), color, Vector2f(texture.left,                 texture.top + texture.height)));
        array.append(Vertex(Vector2f(position.left,                  position.top + position.height), color, Vector2f(texture.left,                 texture.top)));
    };
};
