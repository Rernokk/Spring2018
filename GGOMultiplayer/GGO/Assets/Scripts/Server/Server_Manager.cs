using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.UI;

public class Server_Manager : NetworkBehaviour
{
  float uptime = 0;
  int clientsConnected = 0;
  int netServerCount = 0;
  private void Start()
  {
    if (!isServer)
    {
      Destroy(gameObject);
      return;
    }

    if (!File.Exists("Data.dat"))
    {
      FileStream f = new FileStream("Data.dat", FileMode.Create);
      f.Close();
      ServerSaveData();
    }

    //Client Connected
    NetworkServer.RegisterHandler(MsgType.Connect, HandleConnection);

    //Client Disconnected
    NetworkServer.RegisterHandler(MsgType.Disconnect, HandleDisconnection);

    //Increments netServerCount when clients press the G key (Persistance test)
    NetworkServer.RegisterHandler(45, CountHandler);

    //Loading Data from File;
    ServerLoadData();
  }

  private void Update()
  {
    uptime += Time.deltaTime;
    UpdateDebugPane();
  }

  void HandleConnection(NetworkMessage msg)
  {
    clientsConnected++;
    ClientData msgName = msg.ReadMessage<ClientData>();
    print("Connected: " + msgName.ClientName + ", " + msg.conn.connectionId);
  }

  void HandleDisconnection(NetworkMessage msg)
  {
    clientsConnected--;
    print("Disconnected : " + msg.conn.connectionId);
    NetworkServer.DestroyPlayersForConnection(msg.conn);
  }

  void CountHandler(NetworkMessage msg)
  {
    netServerCount++;
    ServerSaveData();
    print("Recieved Count Inc. From Client");
  }

  void ServerSaveData()
  {
    BinaryFormatter bf = new BinaryFormatter();
    FileStream stream = new FileStream("Data.dat", FileMode.Create);
    ServerData dat = new ServerData();
    dat.netCount = netServerCount;
    bf.Serialize(stream, dat);
    stream.Close();
  }

  void ServerLoadData()
  {
    if (!File.Exists("Data.dat"))
      return;

    FileStream stream = new FileStream("Data.dat", FileMode.Open);
    BinaryFormatter bf = new BinaryFormatter();
    ServerData dat = (ServerData)bf.Deserialize(stream);
    stream.Close();
    netServerCount = dat.netCount;
  }

  void UpdateDebugPane()
  {
    transform.Find("Canvas/Panel/Debug").GetComponent<Text>().text = "Uptime: " + uptime.ToString() + "\nClients Connected: " + clientsConnected.ToString() + "\nClientClicks " + netServerCount.ToString();
  }
}


[Serializable]
class ServerData
{
  public int netCount = 0;

  public ServerData()
  {
    netCount = 0;
  }
}