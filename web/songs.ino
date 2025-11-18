// Get all songs from SD card
server.on("/songs", HTTP_GET, []() {
    File root = SD.open("/");
    File file = root.openNextFile();
    
    JsonArray songs = server.sendJsonArray();
    while (file) {
        if (!file.isDirectory() && isAudioFile(file.name())) {
            JsonObject song = songs.createNestedObject();
            song["name"] = file.name();
            song["path"] = "/" + String(file.name());
            song["duration"] = getDuration(file.name()); // Implement this
        }
        file = root.openNextFile();
    }
    root.close();
    server.sendJson();
});

// Play song
server.on("/play", HTTP_POST, []() {
    String path = server.arg("path");
    playAudio(path);
    server.send(200, "text/plain", "Playing");
});

// Pause
server.on("/pause", HTTP_POST, []() {
    pauseAudio();
    server.send(200, "text/plain", "Paused");
});
