// -- window manager -- //

// variables
RenderWindow app;
Vector2i ws;
Vector2i rs;
View view;

// create window
void updateWindow(bool fs) {
    app.create(VideoMode(1280, 720), appName, fs ? Style::Fullscreen : Style::Close);
    app.setVerticalSyncEnabled(true);

    // correct window data
    ws = Vector2i(app.getSize());
    rs = ws;
    view.setSize(Vector2f(ws));
};

// set zoom
void viewCamera(Vector2i camera) {
    view.setCenter(Vector2f(camera));
    app.setView(view);
};
void viewDefault() {
    app.setView(app.getDefaultView());
};
