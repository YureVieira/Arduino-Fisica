#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server(172,217,30,68);  // numeric IP for Google docs (no DNS)
//char server[] = "docs.google.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(40, 0, 10, 177);
EthernetClient client;

String textFix = "GET /forms/d/e/1FAIpQLSerRYzwgnRWs9HGAa1r-aNQeq2bSuvpuPfvBBn-csLt41tNCg/formResponse?ifq";
String tboxName = "&entry.1256476473=Plano";
String tbox1 = "&entry.185500981=teste";
String tbox2 = "&entry.1516217799=ethernet";
String tbox3 = "&entry.68102006=shield";
String tbox4 = "&entry.1394129725=leonardo";

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 443)) {
    Serial.println("connected");
 String toSend = textFix;//Atribuimos a String auxiliar na nova String que sera enviada
    toSend += tboxName;//Nome do experimento
    toSend += tbox1;// pergunta 1
    toSend += tbox2;// pergunta 2
    toSend += tbox3;// pergunta 3
    toSend += tbox4;// pergunta 4
    toSend += "&submit=Submit HTTP/1.1";//Completamos o metodo GET para nosso formulario.
    Serial.println(toSend);
    client.println(toSend);//Enviamos o GET ao servidor-
    client.println("Host: docs.google.com");//-
    client.println();//-
//    client.stop();//Encerramos a conexao com o servidor
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}

