using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.Networking.NetworkSystem;

public class NetManager : NetworkBehaviour {
  public bool isHost = false;
  public bool serverIsReady = false;
  public bool clientIsReady = false;
  NetworkClient myConnection;

  //Server
  int channelID;
  int socketID;
  int socketPort = 4444;
  float timeOfSave = 0f;
  //Client
  int connectionId;

  public void Start(){
    NetworkTransport.Init();
    ConnectionConfig config = new ConnectionConfig();
    channelID = config.AddChannel(QosType.Reliable);
    int maxConnections = 10;
    HostTopology topology = new HostTopology(config, maxConnections);
    socketID = NetworkTransport.AddHost(topology, socketPort);
    print("Socket Open. SocketID is: " + socketID);
  }

  public void StartServer()
  {
    
  }

  public void StartClient()
  {
    print("Client Connecting.");
    byte error;
    connectionId = NetworkTransport.Connect(socketID, "localhost", socketPort, 0, out error);
    print("Client Setup Completed. Connection ID: " + connectionId);
  }

  public void StartLocalClient(){
    myConnection = ClientScene.ConnectLocalServer();
    myConnection.RegisterHandler(42, ChatMessageHandler);
    serverIsReady = true;
    clientIsReady = true;
    print("Host/Client Setup Completed.");
  }

  void ChatMessageHandler(NetworkMessage msg){
    print("Client received message.");
  }

  void ServerMessageHandler(NetworkMessage msg){
    print("Server received message.");
  }

  void Update(){
    if (isServer){
      
    }
  }
}
