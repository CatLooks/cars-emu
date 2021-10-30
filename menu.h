// -- menu manager -- //

// render menu
int renderMenu() {
    // new vertex array
    VertexArray arr(Quads);

    // render BG
    for (int y = -1; y < ws.y / 64 + 1; y++) {
        for (int x = -1; x < ws.x / 64 + 1; x++) {
            renderSquare(arr, IntRect(x * 64 + menuOffset, y * 64 + menuOffset, 64, 64), texMenuBG);
        };
    };

    // render buttons
    for (int t = 0; t < 10; t++) {
        int x = (ws.x * (t + 1)) / 11;
        renderSquare(arr, IntRect(x - 32, 96, 64, 64), texMenuButton[t]);

        // if active, render overlay
        if (menuSelect == t)
            renderSquare(arr, IntRect(x - 32, 96, 64, 64), texButtonSelect);
    };

    // render buttons
    if (menuSelect >= 0 && menuSelect < 10) {
        renderSquare(arr, IntRect(ws.x * 9 / 11 - 32, 188, ws.x / 11 + 64, 48), palleteBorderColor);
        renderSquare(arr, IntRect(ws.x * 9 / 11 - 32, 238, ws.x / 11 + 64, 48), palleteBorderColor);

        if (menuMaps[menuSelect].valid) {
            renderSquare(arr, IntRect(ws.x * 9 / 11 - 32, 288, ws.x / 11 + 64, 48), palleteBorderColor);

            renderSquare(arr, IntRect(ws.x * 9 / 11 - 28, 192, ws.x / 11 + 56, 40), colorTextMarked);
            renderSquare(arr, IntRect(ws.x * 9 / 11 - 28, 242, ws.x / 11 + 56, 40), colorTextMarked);
            renderSquare(arr, IntRect(ws.x * 9 / 11 - 28, 292, ws.x / 11 + 56, 40), colorTextInvalid);

        } else {
            renderSquare(arr, IntRect(ws.x * 9 / 11 - 28, 192, ws.x / 11 + 56, 40), colorTextMarked);
            renderSquare(arr, IntRect(ws.x * 9 / 11 - 28, 242, ws.x / 11 + 56, 40), colorTextValid);
        };
    };
    renderSquare(arr, IntRect(ws.x - 64, 0, 64, 64), texMenu);
    renderSquare(arr, IntRect(ws.x - 64, ws.y - 64, 64, 64), texReset);

    // display menu
    app.draw(arr, &vtex);

    // render text
    char text[101];
    renderText(menuTitle, ws.x / 2, 20, colorTextDefault, 32, true);

    if (menuSelect >= 0 && menuSelect < 10) {
        if (menuMaps[menuSelect].valid) {
            // render data
            sprintf(text, "Name: %s", menuMaps[menuSelect].name);
            renderText(text, ws.x / 11 - 32, 192, colorTextDefault, 32);
            sprintf(text, "Size: %dx%d", menuMaps[menuSelect].size.x, menuMaps[menuSelect].size.y);
            renderText(text, ws.x / 11 - 32, 242, colorTextDefault, 32);

            // render buttons
            renderText(menuLoad, ws.x * 19 / 22, 192, colorTextDefault, 32, true);
            renderText(menuSave, ws.x * 19 / 22, 242, colorTextDefault, 32, true);
            renderText(menuDelete, ws.x * 19 / 22, 292, colorTextDefault, 32, true);

        } else {
            // render error message
            renderText(menuFail, ws.x / 11 - 32, 192, colorTextInvalid, 32);

            // render buttons
            renderText(menuSave, ws.x * 19 / 22, 192, colorTextDefault, 32, true);
            renderText(menuNew, ws.x * 19 / 22, 242, colorTextDefault, 32, true);
            //printf("%d %d %d %d\n", (int)vtext.getGlobalBounds().left, (int)vtext.getGlobalBounds().top, (int)vtext.getGlobalBounds().width, (int)vtext.getGlobalBounds().height);
        };
    };

    // return quad count
    return arr.getVertexCount() / 4;
};
