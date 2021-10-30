// -- car manager -- //

// car class
class Car {
    public:
    // constructor
    Car(CarSpecs cs, int x, int y, Direction direction = Direction::North) {
        m_speed = cs.speed;
        m_look = cs.look;
        m_d = direction;
        m_u = false;
        m_px = x;
        m_py = y;
        m_dx = 0;
        m_dy = 0;
        m_ox = -1;
        m_oy = -1;
        m_road = 0;
        m_fuel = 0;
        m_rest = 0;
        m_accel = 0;
        m_ld = Vector2i(-1, 0);
        updateCoords();
    };

    // update real location
    void updateCoords() {
        m_x = m_px * 64 + m_dx;
        m_y = m_py * 64 + m_dy;
    };

    // warp to position
    void warp(int x, int y, Direction dir) {
        m_d = dir;
        m_px = x;
        m_py = y;
        updateCoords();
    };

    // create trip
    void trip(World& world, vector<Vector2i> parks, vector<Vector2i> shops, vector<Vector2i> fuels) {
        if (queue.empty()) {
            // reset occupation of tile
            Tile* gnd = world.ftile(Vector2i(m_ox, m_oy));
            if (gnd)
                gnd->buf.occ = false;
            m_ox = m_px;
            m_oy = m_py;

            // get desired service type
            Vector2i pos;
            bt type = 0;

            if (!shops.empty())
                type = 1;
            if (m_rest) {
                m_rest--;
            } else if (!parks.empty()) {
                m_rest = rint(4, 9);
                type = 2;
            };
            if (m_fuel) {
                m_fuel--;
            } else if (!fuels.empty()) {
                m_fuel = rint(6, 9);
                type = 3;
            };
            if (type == 0)
                return;

            // shuffle services
            std::random_shuffle(parks.begin(), parks.end());
            std::random_shuffle(fuels.begin(), fuels.end());
            std::random_shuffle(shops.begin(), shops.end());

            // get a random service position
            int id = 0;
            while (true) {
                if (type == 1) {
                    if (id == shops.size()) {
                        return;
                    };

                    pos = shops[id++];
                    //printf("Check: Shop %d (%d %d)\n", id, pos.x, pos.y);

                } else if (type == 2) {
                    if (id == parks.size())
                        return;

                    pos = parks[id++];
                    //printf("Check: Park %d (%d %d)\n", id, pos.x, pos.y);

                } else if (type == 3) {
                    if (id == fuels.size())
                        return;

                    pos = fuels[id++];
                    //printf("Check: Fuel %d (%d %d)\n", id, pos.x, pos.y);
                };

                // get tile
                Tile* tile = world.ftile(pos);
                if (tile == null) {
                    //printf("> Invalid tile.\n");
                    continue;
                };

                // check tile
                if (tile->buf.occ == false) {
                    // try pathfinding to tile
                    if (path(world, pos.x, pos.y)) {
                        tile->buf.occ = true;
                        //printf("> Path created successfully.\n");
                        break;
                    };// else printf("> Failed to create path.\n");
                };/* else {
                    printf("> Already occupied.\n");
                };*/
            };

            // add pause
            //printf("%d %d\n", pos.x, pos.y);
            queue.push_back(Action(Action::Wait, rint(240, 960)));
        };
    };

    // create path to position
    bool path(World& world, int x, int y) {
        // check if already on the destination
        if (m_px == x && m_py == y)
            return true;

        // validate tile
        Tile* tile = world.ftile(Vector2i(x, y));
        if (tile == null)
            return false;
        if ((tile->borders[0] == Border::None || tile->borders[0] == Border::Out) && (tile->borders[1] == Border::None || tile->borders[1] == Border::Out) &&
            (tile->borders[2] == Border::None || tile->borders[2] == Border::Out) && (tile->borders[3] == Border::None || tile->borders[3] == Border::Out))
            return false;

        // <Position + Direction + Tunnel + Parent>
        struct node_t {
            Direction dir;
            Vector2i pos;
            node_t* np;

            bool it = false;
            bt te = 0;

            // constructor
            node_t() {};
            node_t(int x, int y, Direction d, node_t* parent = null) {
                pos = Vector2i(x, y);
                np = parent;
                dir = d;
            };
        };

        // nodes list
        list<node_t> nodes;
        list<node_t*> stack;

        // get last destination
        //if (m_ld == Vector2i(-1, 0))
        //    m_ld = Vector2i(m_px, m_py);

        // root node
        node_t* root = new node_t(m_px, m_py, m_d);
        bool reach = false;

        // update last destination
        //m_ld = Vector2i(x, y);

        // dest node
        //printf("Destination: %d %d\n", x, y);
        node_t* dest;

        // finder loop
        int itercount = 0;

        // first iteration
        // check every neighbor
        for (int i = 0; i < 4; i++) {
            // prioritise neighbors
            Direction d;
            switch (i) {
                case 0:
                d = root->dir;
                break;
                case 1:
                d = relativeDirection[root->dir][2];
                break;
                case 2:
                d = relativeDirection[root->dir][3];
                break;
                case 3:
                d = relativeDirection[root->dir][1];   // may loop pathfind ???
                break;

                // prevent warnings
                default:
                break;
            };
            Vector2i nb = root->pos + neighborList[d];
            //printf(" - %d %d: ", nb.x, nb.y);

            // check if can move to neighbor
            bt res = world.tmt(this, root->pos.x, root->pos.y, d, root->it);
            //printf("%s\n", moveName[res]);

            // push state
            if (res) {
                nodes.push_back(node_t(nb.x, nb.y, d, root));
                nodes.back().it = root->it;
                stack.push_back(&nodes.back());
            };
            if (res == 2) {
                // enter tunnel
                nodes.back().te = 1;
                nodes.back().it = true;
            } else if (res == 3) {
                // exit tunnel
                nodes.back().te = 2;
                nodes.back().it = false;
            };

            // check if reached destination
            if (res && nb == Vector2i(x, y)) {
                dest = stack.back();
                reach = true;
                break;
            };
        };
        //system("pause > nul");

        while (!stack.empty() && itercount < maxPathfindIterations && !reach) {
            // next iteration stack
            list<node_t*> next;

            // pop stack node
            node_t* node = stack.front();
            stack.pop_front();
            //printf("Stack: %d %d\n", node->pos.x, node->pos.y);

            for (int i = 0; i < 3; i++) {
                // prioritise neighbors
                Direction d;
                switch (i) {
                    case 0:
                    d = node->dir;
                    break;
                    case 1:
                    d = relativeDirection[node->dir][2];
                    break;
                    case 2:
                    d = relativeDirection[node->dir][3];
                    break;
                    case 3:
                    d = relativeDirection[node->dir][1];   // may loop pathfind ???
                    break;

                    // prevent warnings
                    default:
                    break;
                };
                Vector2i nb = node->pos + neighborList[d];
                //printf(" - %d %d: ", nb.x, nb.y);

                // check if can move to neighbor
                bt res = world.tmt(this, node->pos.x, node->pos.y, d, node->it);
                //printf("%s\n", moveName[res]);

                // push state
                if (res) {
                    nodes.push_back(node_t(nb.x, nb.y, d, node));
                    nodes.back().it = node->it;
                    next.push_back(&nodes.back());
                };
                if (res == 2) {
                    // enter tunnel
                    nodes.back().te = 1;
                    nodes.back().it = true;
                } else if (res == 3) {
                    // exit tunnel
                    nodes.back().te = 2;
                    nodes.back().it = false;
                };

                // check if reached destination
                if (res && nb == Vector2i(x, y)) {
                    dest = next.back();
                    reach = true;
                    break;
                };
            };

            //system("pause > nul");

            // check if reached destination
            if (reach)
                break;

            // transmit next stack to original
            while (!next.empty()) {
                stack.push_back(next.front());
                next.pop_front();
            };

            // update iterations count
            itercount++;
        };
        //printf("Iterations: %d\n", itercount);

        // check if couldn't reach destination
        if (!reach) {
            return false;
        };

        // temp queue
        list<Action> nq;

        // create path
        while (dest->np) {
            // push actions
            switch (dest->te) {
                case 0:
                nq.push_front(Action(directionToAction[dest->dir], 1));
                break;
                case 1:
                nq.push_front(Action(directionToAction[dest->dir], 1));
                nq.push_front(Action(Action::EnterTunnel));
                break;
                case 2:
                nq.push_front(Action(Action::ExitTunnel));
                nq.push_front(Action(directionToAction[dest->dir], 1));
                break;
                default:
                break;
            };

            // backtrack node
            dest = dest->np;
        };

        // merge queues
        while (!nq.empty()) {
            queue.push_back(nq.front());
            nq.pop_front();
        };

        // reset pathfind buffer
        world.resetPF();

        // success
        return true;
    };

    // occupy tile
    void occupy(World& world) {
        // get tiles
        Tile* next = world.ntile(Vector2i(m_px, m_py), m_d);
        Tile* ground = world.ftile(Vector2i(m_px, m_py));

        // get road type
        m_road = 255;
        if (ground)
            m_road = (m_road + roadSpeed[roadType[ground->tile]]) / 2;
        if (next)
            m_road = (m_road + roadSpeed[roadType[next->tile]]) / 2;

        // check if on ground
        if (m_u)
            return;

        if (m_a) {
            // if active, occupy ground tile
            if (ground) {
                ground->buf.cd.cp = this;
                ground->buf.cd.dx = m_dx;
                ground->buf.cd.dy = m_dy;
                ground->buf.cd.d = m_d;
            };
            // and try occupying next tile
            if (next) {
                if (next->buf.cd.cp == null)
                    next->buf.cd.cp = this;
            };

        } else {
            // if passive, occupy ground tile
            if (ground)
                ground->buf.cd.cp = this;
        };
    };

    // check if can move
    bool canMove(World& world) {
        // check if underground
        if (m_u)
            return true;
        Tile* next = world.ntile(Vector2i(m_px, m_py), m_d);

        if (next) {
            // if tile is occupied by <this>
            if (next->buf.cd.cp == this || next->buf.cd.cp == null)
                return true;

            // if tile has free space
            if (next->buf.cd.d == m_d) {
                switch (m_d) {
                    case Direction::North: {
                        if (m_dy - speed() >= next->buf.cd.dy)
                            return true;
                        else
                            m_dy = next->buf.cd.dy;
                    };
                    break;

                    case Direction::South: {
                        if (m_dy + speed() <= next->buf.cd.dy)
                            return true;
                        else
                            m_dy = next->buf.cd.dy;
                    };
                    break;

                    case Direction::West: {
                        if (m_dx - speed() >= next->buf.cd.dx)
                            return true;
                        else
                            m_dx = next->buf.cd.dx;
                    };
                    break;

                    case Direction::East: {
                        if (m_dx + speed() <= next->buf.cd.dx)
                            return true;
                        else
                            m_dx = next->buf.cd.dx;
                    };
                    break;
                };
                updateCoords();
            };
        };
        return false;
    };

    // update direction & activity
    bool tick(World& world) {
        // reset activity
        m_a = false;

        // if no actions, exit
        if (queue.empty()) {
            return false;
        };

        // get current action
        Action& act = queue.front();

        // get next tile
        Tile* tile = null;
        if (act.id == Action::MoveNorth) {
            // switch direction
            if (m_d != Direction::North) {
                m_d = Direction::North;
                m_accel /= turnSlowdown;
            };
            // get tile
            tile = world.ntile(Vector2i(m_px, m_py), Direction::North);
            // activate car
            m_a = true;

        } else if (act.id == Action::MoveSouth) {
            // switch direction
            if (m_d != Direction::South) {
                m_d = Direction::South;
                m_accel /= turnSlowdown;
            };
            // get tile
            tile = world.ntile(Vector2i(m_px, m_py), Direction::South);
            // activate car
            m_a = true;

        } else if (act.id == Action::MoveWest) {
            // switch direction
            if (m_d != Direction::West) {
                m_d = Direction::West;
                m_accel /= turnSlowdown;
            };
            // get tile
            tile = world.ntile(Vector2i(m_px, m_py), Direction::West);
            // activate car
            m_a = true;

        } else if (act.id == Action::MoveEast) {
            // switch direction
            if (m_d != Direction::East) {
                m_d = Direction::East;
                m_accel /= turnSlowdown;
            };
            // get tile
            tile = world.ntile(Vector2i(m_px, m_py), Direction::East);
            // activate car
            m_a = true;
        };

        // check tile
        if (tile) {
            // check next tile light buffer
            if ((tile->buf.borders[neightborOpposite[m_d]] & BORDER_IN) == 0) {
                m_a = false;
                return false;
            };
        };

        // can move
        return true;
    };

    // move car
    void move() {
        // get current action
        Action& act = queue.front();
        bool pop = false;

        //printf("Action: %s\n", actionName[act.id]);

        // process action
        switch (act.id) {
            case Action::Wait: {
                if (act.param-- == 0)
                    pop = true;
            };
            break;
            case Action::EnterTunnel:
            m_u = true;
            pop = true;
            break;
            case Action::ExitTunnel:
            m_u = false;
            pop = true;
            break;
            case Action::MoveNorth: {
                // move car
                m_dy -= speed();
                if (m_dy <= -64) {
                    m_dy = 0;
                    m_py--;
                    pop = true;
                };
            };
            break;
            case Action::MoveSouth: {
                // move car
                m_dy += speed();
                if (m_dy >= 64) {
                    m_dy = 0;
                    m_py++;
                    pop = true;
                };
            };
            break;
            case Action::MoveWest: {
                // move car
                m_dx -= speed();
                if (m_dx <= -64) {
                    m_dx = 0;
                    m_px--;
                    pop = true;
                };
            };
            break;
            case Action::MoveEast: {
                // move car
                m_dx += speed();
                if (m_dx >= 64) {
                    m_dx = 0;
                    m_px++;
                    pop = true;
                };
            };
            break;
        };

        // pop the element
        if (pop) {
            queue.pop_front();
        };
        updateCoords();
    };

    // render car
    void render(VertexArray& arr) {
        renderSquare(arr, IntRect(m_x * wts / 64, m_y * wts / 64, wts, wts), texCar[m_look], m_d);
    };
    // follow car
    void follow() {
        camera.x = m_x * wts / 64 + 32;
        camera.y = m_y * wts / 64 + 32;
    };

    // get car layer
    bool isUnderground() {
        return m_u;
    };
    // get current car speed
    int speed() {
        // update acceleration
        if (m_accel < 12)
            m_accel++;

        // get maximum speed
        int s = m_speed * m_road / 256;

        // return current speed
        return float(s * m_accel) / 12;
    };

    private:
    // variable
    list<Action> queue;
    Vector2i m_ld; // last destination
    Direction m_d;
    bool m_u; // underground
    bool m_a; // active

    int m_speed; // car speed
    int m_accel; // car real speed
    int m_look;  // car texture
    int m_road;  // road speed

    int m_fuel; // moves to gas station
    int m_rest; // moves to parking

    int m_ox; // occupied x
    int m_oy; // occupied y
    int m_dx; // fraction x
    int m_dy; // fraction y
    int m_px; // tile x
    int m_py; // tile y
    int m_x;  // real x
    int m_y;  // real y
};
