# esp32-ST7796U
A simple web page using the esp ST7796U (inludes user_setup.h)
ESP ST7796U 3.5in 480x320

Front of esp:
<img src="images/2.jpeg" width="400">

back of esp:
<img src="images/1.jpeg" width="400">


# step 1

IN IDE:
install esp32 (by espressif systems) in board manager



install TFT_eSPI (by bodmer) in library manager

install XPT2046_Touchscreen (by Paul Stoffregen) in library manager

# step 2
open your file explorer and go to 
/this pc/local disk/Users/yourname/Documents/Arduino/libraries/TFT_eSPI

Then open the User_setup.h file and paste the user setup from this repo, after, open user_setup_select.h and replace it with the one from this repo

# step 3
get the .ino file and open it in your ide.

feel free to customize it without giving any credits

# what does it do?
the esp will scan for saved networks, edit this with your network info

<br> const char* knownNetworks[][2] = {
<br>  {"network1",     "pass1"},
<br>  {"network2",      "pass2"},
<br>  {"network3",      "pass3"},
<br>  {nullptr, nullptr}
<br>  };


then it will host a web page on 192.168.1.*ip* (index) and /info, you can add as many as you want (limit is esp storage)
```html
  server.on("/", HTTP_GET, []() { //on / mean main when you access its ip
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>server</title>"; // web page name shown
    html += "<style>body{font-family:Arial;background:#111;color:#0f0;text-align:center;padding:50px;}</style>";
    html += "</head><body>";
    html += "<h1>WiFi connected</h1>";
    html += "<p><strong>IP:</strong> " + currentIP + "</p>";
    html += "<p><a href='/info'>Info</a></p>"; //info button
    html += "<p>server on</p>"; //says server is on
    html += "</body></html>";
   server.send(200, "text/html", html); //send 200 beacause it awnsered
  });


server.on("/info", HTTP_GET, []() {
    unsigned long uptime = (millis() - connectTime) / 1000;
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>server info</title>";
    html += "<style>body{font-family:Arial;background:#222;color:#0ff;text-align:center;padding:30px;}</style>";
    html += "</head><body>";
    html += "<h1>Event Info</h1>";
    html += "<p><strong>SSID:</strong> " + currentSSID + "</p>";
    html += "<p><strong>IP:</strong> " + currentIP + "</p>";
    html += "<p><strong>RSSI:</strong> " + String(WiFi.RSSI()) + " dBm</p>";
    html += "<p><strong>UP:</strong> " + String(uptime) + " sec</p>";
    html += "<p><a href='/'>return</a></p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });
