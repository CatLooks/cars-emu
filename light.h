// -- traffic light manager -- //

// light class
class Light {
    public:
    // constructor
    Light(LightFrame lf1, LightFrame lf2, wt it, int x, int y, bt corner, bool visible) {
        m_lf[0] = lf1;
        m_lf[1] = lf2;
        m_v = visible;
        m_c = corner;
        m_x = x;
        m_y = y;

        // setup initial state
        reset();
    };

    // update border buffer
    void buffer(World& world) {
        // update border buffer
        Tile* tile = world.ftile(Vector2i(m_x, m_y));
        if (tile == null)
            return;

        tile->buf.borders[0] = m_lf[m_s].borders[0];
        tile->buf.borders[1] = m_lf[m_s].borders[1];
        tile->buf.borders[2] = m_lf[m_s].borders[2];
        tile->buf.borders[3] = m_lf[m_s].borders[3];
    };

    // process displayed light
    void tick(World& world) {
        if (m_t)
            m_t--;
        else {
            // update timer
            m_s = !m_s;
            m_t = m_lf[m_s].ticks;

            // update border buffer
            buffer(world);
        };
    };

    // render light
    void render(VertexArray& arr) {
        if (m_v) {
            if (m_c & 8)
                rlc(arr, 0);
            if (m_c & 2)
                rlc(arr, 1);
            if (m_c & 4)
                rlc(arr, 2);
            if (m_c & 1)
                rlc(arr, 3);
        };
    };

    // render editor light
    void renderEdit(VertexArray& arr) {
        // render tile borders
        renderSquare(arr, IntRect(m_x * wts, m_y * wts, wts, wts), texLightTile, (select.x == m_x && select.y == m_y) ? colorTextMarked : colorVoid);

        // render columns
        render(arr);
    };

    // create template
    LightTemplate createTemplate() {
        LightTemplate lt;
        lt.b1 = m_lf[0].borders[0] << 6 | m_lf[0].borders[1] << 4 | m_lf[0].borders[2] << 2 | m_lf[0].borders[3];
        lt.b2 = m_lf[1].borders[0] << 6 | m_lf[1].borders[1] << 4 | m_lf[1].borders[2] << 2 | m_lf[1].borders[3];
        lt.t1 = m_lf[0].ticks;
        lt.t2 = m_lf[1].ticks;
        lt.s1 = m_lf[0].state;
        lt.s2 = m_lf[1].state;
        lt.it = m_i;
        lt.x = m_x & 0xFFFF;
        lt.y = m_y & 0xFFFF;
        lt.c = m_c;
        lt.v = m_v;
        return lt;
    };

    // get coordinates
    Vector2i getCoords() {
        return Vector2i(m_x, m_y);
    };

    // set borders
    void setBorder(bool state, int dir) {
        m_lf[state].borders[dir] = static_cast<Border>((m_lf[state].borders[dir] + 1) % 4);
    };

    // get borders
    Border getBorder(bool state, int dir) {
        return m_lf[state].borders[dir];
    };

    // get light state
    bool getLightColor(bool state) {
        return m_lf[state].state;
    };

    // get light ticks
    wt getLightTicks(bool state) {
        return m_lf[state].ticks;
    };

    // get initial ticks
    wt getInitialTicks() {
        return m_i;
    };

    // get visibility
    bool visible() {
        return m_v;
    };

    // get corner
    bool getCorner(int c) {
        return m_c & c;
    };

    // set corner
    void toggleCorner(int c) {
        m_c ^= c;
    };

    // toggle light color
    void toggleLightColor(bool state) {
        m_lf[state].state = !m_lf[state].state;
    };

    // toggle visibility
    void toggleVisibility() {
        m_v = !m_v;
    };

    // roll light ticks
    void rollTicks(bool state, int delta, bool seconds) {
        int sec = m_lf[state].ticks / 60;
        int mic = m_lf[state].ticks % 60;
        if (seconds) {
            sec += delta;
            if (sec < 0)
                sec = 0;
            if (sec > 999)
                sec = 999;
        } else {
            mic += delta;
            if (mic < 0)
                mic = 0;
            if (mic > 59)
                mic = 59;
        };
        m_lf[state].ticks = sec * 60 + mic;
    };

    // roll initial ticks
    void rollInitialTicks(bool state, int delta, bool seconds) {
        int sec = m_i / 60;
        int mic = m_i % 60;
        if (seconds) {
            sec += delta;
            if (sec < 0)
                sec = 0;
            else if (sec > 999)
                sec = 999;
        } else {
            mic += delta;
            if (mic < 0)
                mic = 0;
            else if (mic > 59)
                mic = 59;
        };
        m_i = sec * 60 + mic;
    };

    // roll coordinates
    void rollCoords(int delta, bool yc) {
        if (yc) {
            m_y += delta;
            if (m_y < 0)
                m_y = 0;
            else if (m_y > 32000)
                m_y = 32000;
        } else {
            m_x += delta;
            if (m_x < 0)
                m_x = 0;
            else if (m_x > 32000)
                m_x = 32000;
        };
    };

    // reset
    void reset() {
        if (m_i) {
            m_t = m_i;
            m_s = 1;
        } else {
            m_t = m_lf[0].ticks;
            m_s = 0;
        };
    };

    private:
    // render column
    void rlc(VertexArray& arr, bt c) {
        if (m_t < lightYellowTicksLeft)
            renderSquare(arr, IntRect(m_x * wts + texLightOffset[c].x * wts / 16, m_y * wts + texLightOffset[c].y * wts / 16, wts / 4, wts), texLight[tlc::Yellow]);
        else
            renderSquare(arr, IntRect(m_x * wts + texLightOffset[c].x * wts / 16, m_y * wts + texLightOffset[c].y * wts / 16, wts / 4, wts), texLight[m_lf[m_s].state ? tlc::Green : tlc::Red]);
    };

    LightFrame m_lf[2];
    bool m_s; // frame state
    wt m_t;  // frame ticks left
    wt m_i;  // initial ticks

    bool m_v; // visible
    int m_x;  // tile x
    int m_y;  // tile y
    bt m_c;   // render corner
};

// create from template
Light createFromTemplate(LightTemplate lt) {
    return Light(LightFrame(lt.b1, lt.t1, lt.s1), LightFrame(lt.b2, lt.t2, lt.s2), lt.it, lt.x, lt.y, lt.c, lt.v);
};
