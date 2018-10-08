#include "Colors.h"
#include "LedController.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WebSocketsServer.h>


#define DEBUG
#ifdef DEBUG
	#define DPRINT(...)    Serial.print(__VA_ARGS__)
	#define DPRINTLN(...)  Serial.println(__VA_ARGS__)
	#define DPRINTF(...)   Serial.printf(__VA_ARGS__)
	#define DINIT()        Serial.begin(115200)
#else
	#define DPRINT(...)
	#define DPRINTLN(...)
	#define DPRINTF(...)
	#define DINIT()
#endif

#define PORT_SOCKET 81
#define PORT_HTTP 80

// Globals
WebSocketsServer webSocket(PORT_SOCKET);
ESP8266WebServer httpServer(PORT_HTTP);
ESP8266HTTPUpdateServer httpUpdater;
unsigned long lastTimeHost = 0;
unsigned long lastTimeRefresh = 0;
// NETWORK: Static IP details...
IPAddress ip(192, 168, 1, 99);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";


RGB C1;
RGB C2;

LedControllerClass LedController;

// Handle events 
void ParsePost()
{
	DPRINTLN(httpServer.uri());
	for (uint8_t i = 0; i<httpServer.args(); i++)
	{
		DPRINTF("%s : %s\n", httpServer.argName(i).c_str(), httpServer.arg(i).c_str());

		if (httpServer.argName(i) == "C1R")
			C1.R = httpServer.arg(i).toInt();
		else if (httpServer.argName(i) == "C1G")
			C1.G = httpServer.arg(i).toInt();
		else if (httpServer.argName(i) == "C1B")
			C1.B = httpServer.arg(i).toInt();

		else if (httpServer.argName(i) == "C2R")
			C2.R = httpServer.arg(i).toInt();
		else if (httpServer.argName(i) == "C2G")
			C2.G = httpServer.arg(i).toInt();
		else if (httpServer.argName(i) == "C2B")
			C2.B = httpServer.arg(i).toInt();
	}

	analogWrite(D2, PWMRANGE - C1.R);
	analogWrite(D1, PWMRANGE - C1.G);
	analogWrite(D3, PWMRANGE - C1.B);

	analogWrite(D6, PWMRANGE - C2.R);
	analogWrite(D7, PWMRANGE - C2.G);
	analogWrite(D5, PWMRANGE - C2.B);

	DPRINTF("P1:%d,%d,%d\n", PWMRANGE - C1.R, PWMRANGE - C1.G, PWMRANGE - C1.B);
	DPRINTF("P2:%d,%d,%d\n", PWMRANGE - C2.R, PWMRANGE - C2.G, PWMRANGE - C2.B);
}

void SendPage()
{
#if 0
	String page = "\
	<head>\
	<title>LEDS Control Panel</title>\
		</head>\
		<body>\
		<h1>LEDS Control Panel</h1>\
		<form action =  \"/\" method = \"POST\">\
			<strong>C1: </strong>\
			R <input name = \"C1R\" type = \"text\" value = " + String(C1.R) + " /> \
			G <input name = \"C1G\" type = \"text\" value = " + String(C1.G) + " /> \
			B <input name = \"C1B\" type = \"text\" value = " + String(C1.B) + " /> \
			<br><br>\
			<strong>C2: </strong>\
			R <input name = \"C2R\" type = \"text\" value = " + String(C2.R) + " /> \
			G <input name = \"C2G\" type = \"text\" value = " + String(C2.G) + " /> \
			B <input name = \"C2B\" type = \"text\" value = " + String(C2.B) + " /> \
			<br><br>\
			<input type = \"submit\" value = \"Send\">\
		</form>\
		</body>\
	";
#endif // 0

	String page = "<!DOCTYPE html>\
<html>\
<head>\
	<title>LEDS Control Panel</title>\
	<style>\
		div.colorPrev {\
			border: 2px solid black;\
			text-align: center;\
			font-size: 20px;\
			font-weight: bold;\
			width: 200px;\
			height: 40px;\
			line-height: 40px;\
			background-color: red;\
			border-radius: 5px;\
			color: white\
		}\
	</style>\
</head>\
<body>\
	<h1>LEDS Control Panel</h1>\
	<form action=\"/\" method=\"POST\">\
		<div style=\"display: flex;flex-wrap: wrap;\">\
			<div style=\"padding: 1em;\">\
				<div class=\"colorPrev\" id=\"C1\">C1</div><br>\
				R <input name=\"C1R\" id=\"C1R\" type=\"range\" min=0 max=1023 value=" + String(C1.R) + " oninput=\"updateValues('C1')\"> <span id=\"C1Rv\">0</span><br>\
				G <input name=\"C1G\" id=\"C1G\" type=\"range\" min=0 max=1023 value=" + String(C1.G) + " oninput=\"updateValues('C1')\"> <span id=\"C1Gv\">0</span><br>\
				B <input name=\"C1B\" id=\"C1B\" type=\"range\" min=0 max=1023 value=" + String(C1.B) + " oninput=\"updateValues('C1')\"> <span id=\"C1Bv\">0</span><br>\
			</div>\
			<div style=\"padding: 1em;\">\
				<div class=\"colorPrev\" id=\"C2\">C2</div><br>\
				R <input name=\"C2R\" id=\"C2R\" type=\"range\" min=0 max=1023 value=" + String(C2.R) + " oninput=\"updateValues('C2')\"> <span id=\"C2Rv\">0</span><br>\
				G <input name=\"C2G\" id=\"C2G\" type=\"range\" min=0 max=1023 value=" + String(C2.G) + " oninput=\"updateValues('C2')\"> <span id=\"C2Gv\">0</span><br>\
				B <input name=\"C2B\" id=\"C2B\" type=\"range\" min=0 max=1023 value=" + String(C2.B) + " oninput=\"updateValues('C2')\"> <span id=\"C2Bv\">0</span><br>\
			</div>\
		</div>\
		<br>\
		<input type=\"submit\" value=\"Send\">\
	</form>\
	<script>\
		updateValues(\"C1\");\
		updateValues(\"C2\");\
		function updateValues(id) {\
		  var col, r, g, b;\
		  r = document.getElementById(id+\"R\").value;\
		  g = document.getElementById(id+\"G\").value;\
		  b = document.getElementById(id+\"B\").value;\
		  col = \"rgb(\" + r/4 + \",\" + g/4 + \",\" + b/4 + \")\";\
		  document.getElementById(id+\"Rv\").innerHTML = r;\
		  document.getElementById(id+\"Gv\").innerHTML = g;\
		  document.getElementById(id+\"Bv\").innerHTML = b;\
		  document.getElementById(id).style.backgroundColor = col;\
		}\
	</script>\
</body>\
</html>";

	httpServer.send(200, "text/html", page);
}

void handleRoot()
{
	ParsePost();
	SendPage();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
	if (type == WStype_DISCONNECTED)
	{
		DPRINTF("[%u] Disconnected!\n", num);
	}
	else if (type == WStype_CONNECTED)
	{
		IPAddress ip = webSocket.remoteIP(num);
		DPRINTF("[%u] Connected from  ", num);
		DPRINT(ip);
		DPRINTF(" url: %s\n", payload);
		webSocket.sendTXT(num, "Connected");
	}
	else if (type == WStype_TEXT)
	{
		//DPRINTF("[%u] Got Text: %s\n", num, payload);
		if (payload[0] == '?')
		{
			String s = LedController.GetInfoMsg();
			webSocket.sendTXT(num, s);
			DPRINTLN(s.c_str());
		}
		else
			LedController.Parse(payload);
	}
}


// Init connections functions
void WifiConnect()
{
	WiFi.config(ip, gateway, subnet);
	WiFi.begin(ssid, password);
	DPRINTLN();

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		DPRINT(".");
	}

	DPRINTLN();
	DPRINTF("Connected to wifi: %s\n", ssid);
	DPRINT("IP address: ");
	DPRINTLN(WiFi.localIP());

	//PrintLUT();
}

void WebSocketConnect()
{
	webSocket.begin();
	webSocket.onEvent(webSocketEvent);
}

void HTTPServerConnect()
{
	httpUpdater.setup(&httpServer);
	httpServer.on("/", handleRoot);
	httpServer.begin();
	DPRINTLN("HTTPServer ready.");
}

void InitPins()
{
	pinMode(D1, OUTPUT);
	pinMode(D2, OUTPUT);
	pinMode(D3, OUTPUT);
	pinMode(D5, OUTPUT);
	pinMode(D6, OUTPUT);
	pinMode(D7, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
}

// Main
void setup()
{
	DINIT();
	WifiConnect();
	WebSocketConnect();
	HTTPServerConnect();
	InitPins();
}

void loop()
{
	if (WiFi.status() != WL_CONNECTED)
	{
		WifiConnect();
		WebSocketConnect();
		HTTPServerConnect();
	}
	else
	{
		webSocket.loop();
		LedController.Loop();
		unsigned long t = millis();
		if (t - lastTimeHost > 10)
		{
			httpServer.handleClient();
			lastTimeHost = millis();
		}
#if 1
		if (t - lastTimeRefresh > 1000)
		{
			lastTimeRefresh = millis();
			static bool l = HIGH;
			digitalWrite(LED_BUILTIN, l);
			l = !l;
		}
#endif // 0

	}
}
