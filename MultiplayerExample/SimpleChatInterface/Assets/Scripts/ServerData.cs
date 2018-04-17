using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Xml.Serialization;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.Networking.NetworkSystem;


public class ServerData : NetworkBehaviour
{
  Dictionary<int, PlayerController> playerConnections;
  Dictionary<string, PlayerData> playerDataSet;
  float timeOfStart = 0f;

  [SerializeField] GameObject blockPrefab;

  void Start()
  {
    if (isServer)
    {
      NetworkServer.RegisterHandler(MsgType.Connect, ServerAcceptConnection);
      playerConnections = new Dictionary<int, PlayerController>();
      ServerLoadData();
      timeOfStart = Time.realtimeSinceStartup;
      for (int i = 0; i < 8; i++)
      {
        GameObject obj = Instantiate(blockPrefab, new Vector3(UnityEngine.Random.Range(-5f, 5f), UnityEngine.Random.Range(-5f, 5f), UnityEngine.Random.Range(-5f, 5f)), Quaternion.Euler(UnityEngine.Random.Range(-90f, 90f), UnityEngine.Random.Range(-90f, 90f), UnityEngine.Random.Range(-90f, 90f)));
        NetworkServer.Spawn(obj);
      }
    }
  }

  void ServerAcceptConnection(NetworkMessage message)
  {
    print("Registering connection from: " + message.conn.connectionId);
    PlayerController holder = message.ReadMessage<PlayerPackage>().obj.GetComponent<PlayerController>();
    if (playerDataSet == null)
    {
      playerDataSet = new Dictionary<string, PlayerData>();
    }
    print("Player Data Length: " + playerDataSet.Count);
    foreach (string pid in playerDataSet.Keys)
    {
      if (pid == holder.playerID)
      {
        print("Found player ID: " + pid);
        holder.level = playerDataSet[pid].level;
        holder.charName = playerDataSet[pid].name;
        holder.playerID = pid;
        break;
      }
    }


    if (holder.playerID == "")
    {
      bool unique = true;
      while (unique)
      {
        for (int i = 0; i < 10; i++)
        {
          holder.playerID += (char)(UnityEngine.Random.Range(65, 90));
        }
        if (!playerDataSet.ContainsKey(holder.playerID))
        {
          unique = false;
        }
      }
    }
    playerConnections.Add(message.conn.connectionId, holder);

  }

  private void Update()
  {
    if (isServer)
    {
      if (Input.GetKeyDown(KeyCode.G))
      {
        ServerBackupData();
        ServerLoadData();
      }
      if (Time.realtimeSinceStartup - timeOfStart > 300)
      {
        timeOfStart = Time.realtimeSinceStartup;
        print("Backing up data.");
        ServerBackupData();
      }
    }
  }

  void ServerBackupData()
  {
    print("Backing up current players.");
    BinaryFormatter bf = new BinaryFormatter();
    FileStream fileSrc = new FileStream("PlayerData.dat", FileMode.Create);
    FileStream ptxt = new FileStream("PlainTextServerData.txt", FileMode.Create);
    StreamWriter writer = new StreamWriter(ptxt);
    DataPackage pak = new DataPackage(playerConnections.Count);
    foreach (PlayerController val in playerConnections.Values)
    {
      PlayerData temp = new PlayerData(val.charName, val.playerID, val.level);
      writer.WriteLine("Name: " + val.charName + ", PID: " + val.playerID + ", Level: " + val.level);
      pak.dat.Add(temp);
    }
    bf.Serialize(fileSrc, pak);
    print("Backup Completed.");
    fileSrc.Close();
    writer.Close();
    ptxt.Close();
  }

  void ServerLoadData()
  {
    print("Deserializing Data.");
    if (!File.Exists("PlayerData.dat"))
    {
      return;
    }
    BinaryFormatter bf = new BinaryFormatter();
    FileStream fileSrc = new FileStream("PlayerData.dat", FileMode.Open);
    DataPackage pak = (DataPackage)bf.Deserialize(fileSrc);
    playerDataSet = new Dictionary<string, PlayerData>();
    foreach (PlayerData dat in pak.dat)
    {
      playerDataSet.Add(dat.playerID, dat);
    }
    fileSrc.Close();

  }
}

[Serializable]
class PlayerData
{
  public int level;
  public string name;
  public string playerID;
  public PlayerData(string name, string playerID, int level)
  {
    this.level = level;
    this.name = name;
    this.playerID = playerID;
  }
}

[Serializable]
class DataPackage
{
  public List<PlayerData> dat;
  public DataPackage(int count)
  {
    dat = new List<PlayerData>();
  }
}