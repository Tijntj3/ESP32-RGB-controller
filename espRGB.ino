// Libraries
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

// Macros
#define LED_PIN    13
#define LED_COUNT 288

// Globals
// --WiFi 
const char* ssid     = "ESP32-RGB-controller";
const char* password = "RainbowRainbow";

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WiFiServer server(80);

// --HTTP request
String header;
String redString = "", greenString = "", blueString = "";
int pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;

// --LED strip
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Functions
// --Set static colour for all leds on the strip
void setColour(int red, int green, int blue) {
    for(int i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.show();
}

// Setup
void setup() {
    Serial.begin(115200);
    
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet); 
    Serial.print("Starting server at: ");
    Serial.println(WiFi.softAPIP());
    server.begin();

    strip.begin();
    strip.show();
    strip.setBrightness(64);

    setColour(127, 127, 127);
}

// Loop
void loop() {
    WiFiClient client = server.available();                     // Listen for incoming clients

    if (client) {                                               // If a new client connects,
        Serial.println("New Client.");                          // print a message out in the serial port
        String currentLine = "";                                // make a String to hold incoming data from the client

        while (client.connected()) {                            // loop while the client's connected
            if (client.available()) {                           // if there's bytes to read from the client,
                char c = client.read();                         // read a byte, then
                Serial.write(c);                                // print it out the serial monitor
                header += c;
                if (c == '\n') {                                // if the byte is a newline character
                                                                // if the current line is blank, you got two newline characters in a row.
                                                                // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0) {
                                                                // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        client.println("HTTP/1.1 200 OK");      // and a content-type so the client knows what's coming, then a blank line:
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();
                   
                        client.println("<!DOCTYPE html>");      // Display the HTML web page
                        client.println("<html>");
                        client.println("<head>");
                        client.println("<meta charset=\"UTF-8\">");
                        client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
                        client.println("<style>");
                        client.println(".divide {margin:0;width:100vw;height:20vh;}");
                        client.println(".sldcontrol {-webkit-appearance:none;margin-top:12.5vh;margin-left:20vw;width:60vw;height:1vh;background:#585858;outline:none;}");
                        client.println(".sldcontrol::-webkit-slider-thumb {margin:0;-webkit-appearance:none;appearance:none;width:7vh;height:7vh;border-radius:100%;border-style:none;cursor: pointer;}");
                        client.println(".sldcontrol::-moz-range-thumb {margin:0;width:7vh;height:7vh;border-radius:100%;border-style: none;cursor: pointer;}");
                        client.println("#sldred::-webkit-slider-thumb {background:#FF6961;}");
                        client.println("#sldred::-moz-range-thumb {background:#FF6961;}");
                        client.println("#sldgreen::-webkit-slider-thumb {background:#77DD77;}");
                        client.println("#sldgreen::-moz-range-thumb {background:#77DD77;}");
                        client.println("#sldblue::-webkit-slider-thumb {background:#6CA0DC;}");
                        client.println("#sldblue::-moz-range-thumb {background:#6CA0DC;}");
                        client.println(".dot {margin:0;font-family:Arial;text-align:center;color:rgb(127,127,127);}");
                        client.println("</style>");
                        client.println("<script>");
                        client.println("function setClr() {");
                        client.println("var redslider = document.getElementById(\"sldred\");");
                        client.println("var greenslider = document.getElementById(\"sldgreen\");");
                        client.println("var blueslider = document.getElementById(\"sldblue\");");
                        client.println("var dotClr = document.getElementById(\"dotClr\");");
                        client.println("var dotText = document.getElementById(\"dotText\");");
                        client.println("dotClr.style.color = \"rgb(\" + redslider.value + \",\" + greenslider.value + \",\" + blueslider.value + \")\";");
                        client.println("dotText.style.color = \"rgb(\" + redslider.value + \",\" + greenslider.value + \",\" + blueslider.value + \")\";}");
                        client.println("function sendClr() {");
                        client.println("var req = new XMLHttpRequest;");
                        client.println("var redslider = document.getElementById(\"sldred\");");
                        client.println("var greenslider = document.getElementById(\"sldgreen\");");
                        client.println("var blueslider = document.getElementById(\"sldblue\");");
                        client.println("req.open(\"GET\", \"/?r\" + redslider.value + \"g\" + greenslider.value + \"b\" + blueslider.value + \"&\", true);");
                        client.println("req.send();}");
                        client.println("</script>");
                        client.println("</head>");
                        client.println("<body style=\"margin: 0;background-color:#252526;display: block;\">");
                        client.println("<div class=\"divide\">");
                        client.println("<input class=\"sldcontrol\" id=\"sldred\" type=\"range\" min=\"0\" max=\"255\" value=\"127\" oninput=\"setClr()\">");
                        client.println("</div>");
                        client.println("<div class=\"divide\">");
                        client.println("<input class=\"sldcontrol\" id=\"sldgreen\" type=\"range\" min=\"0\" max=\"255\" value=\"127\" oninput=\"setClr()\">");
                        client.println("</div>");
                        client.println("<div class=\"divide\">");
                        client.println("<input class=\"sldcontrol\" id=\"sldblue\" type=\"range\" min=\"0\" max=\"255\" value=\"127\" oninput=\"setClr()\">");
                        client.println("</div>");
                        client.println("<div class=\"divide\">");
                        client.println("<p class=\"dot\" id=\"dotClr\" style=\"font-size:20vh;line-height:20vh;cursor:pointer;\" onclick=\"sendClr()\">●</p>");
                        client.println("<p class=\"dot\" id=\"dotText\" style=\"font-size:2vh;\">Tijntj3</p>");
                        client.println("</div>");
                        client.println("</body>");
                        client.println("</html>");
                        client.println();                       // The HTTP response ends with another blank line

                        if(header.indexOf("GET /?r") >= 0) {    // Request sample: /?r201g32b255&
                            pos1 = header.indexOf('r');
                            pos2 = header.indexOf('g');
                            pos3 = header.indexOf('b');
                            pos4 = header.indexOf('&');
                            redString = header.substring(pos1+1, pos2);
                            greenString = header.substring(pos2+1, pos3);
                            blueString = header.substring(pos3+1, pos4);
                            setColour(redString.toInt(), greenString.toInt(), blueString.toInt());
                        }
                    break;                                      // Break out of the while loop
                    } 
                    else {                                      // If you got a newline, then clear currentLine
                        currentLine = "";
                    }
                } 
                else if (c != '\r') {                           // If you got anything else but a carriage return character,
                    currentLine += c;                           // Add it to the end of the currentLine
                }
            }
        }
    header = "";                                                // Clear the header variable
    client.stop();                                              // Close the connection
    Serial.println("Client disconnected.");
    Serial.println("");
    }
}