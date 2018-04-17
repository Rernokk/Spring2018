using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.Networking.NetworkSystem;
using UnityEngine.UI;

[Serializable]
public class PlayerPackage : MessageBase
{
  public GameObject obj;

  public PlayerPackage()
  {
    this.obj = null;
  }

  public PlayerPackage(GameObject obj)
  {
    this.obj = obj;
  }
}

public class PlayerController : NetworkBehaviour
{
  [SerializeField] GameObject myUI;
  [SyncVar(hook = "UpdateLevelHUD")] public int level;
  [SyncVar(hook = "UpdateNameHUD")] public string charName;
  [SyncVar(hook = "SyncPlayerID")] public string playerID;
  //public string playerID;


  public override void OnStartClient()
  {
    base.OnStartClient();
    UpdateNameHUD(charName);
    UpdateLevelHUD(level);
  }

  void Start()
  {
    if (isServer)
    {
    }

    if (!isLocalPlayer)
    {
      return;
    }

    transform.Find("Capsule").GetComponent<MeshRenderer>().material.color = Color.blue;
    myUI = Instantiate(myUI, transform, true);
    if (File.Exists("CharData.dat"))
    {
      print("Reading player ID");
      FileStream src = new FileStream("CharData.dat", FileMode.Open);
      BinaryFormatter bf = new BinaryFormatter();
      string s = (string)bf.Deserialize(src);
      src.Close();
      SyncPlayerID(s);
      //playerID = s;
    }
    else
    {
      CmdChangeName("NotBob");
      CmdLevelUp();
      SyncPlayerID("");
      //playerID = "";
    }
    NetworkManager.singleton.client.connection.Send(MsgType.Connect, new PlayerPackage(gameObject));
    DontDestroyOnLoad(gameObject);
  }

  void Update()
  {
    if (!isLocalPlayer)
    {
      return;
    }
    if (Input.GetKey(KeyCode.D))
    {
      transform.position += transform.right * Time.deltaTime;
    }
    if (Input.GetKey(KeyCode.A))
    {
      transform.position -= transform.right * Time.deltaTime;
    }
    if (Input.GetKeyDown(KeyCode.Z))
    {
      CmdLevelUp();
    }
    if (Input.GetKeyDown(KeyCode.C))
    {
      CmdChangeName("Bob");
    }

    if (Input.GetKeyDown(KeyCode.J))
    {
      //NetworkManager.singleton.ServerChangeScene("AnotherScene");
    }

    if (Input.GetKeyDown(KeyCode.K))
    {
      //NetworkManager.singleton.ServerChangeScene("SampleScene");
    }
  }

  [Command]
  void CmdLevelUp()
  {
    level++;
  }

  [Command]
  void CmdChangeName(string newName)
  {
    charName = newName;
  }

  void UpdateLevelHUD(int val)
  {
    transform.Find("Canvas/Text").GetComponent<Text>().text = charName + " - " + val;
    level = val;
  }

  void UpdateNameHUD(string val)
  {
    transform.Find("Canvas/Text").GetComponent<Text>().text = val + " - " + level;
    charName = val;
  }

  //void SyncPlayerID(NetworkMessage mess)
  void SyncPlayerID(string s)
  {
    if (!isLocalPlayer)
    {
      return;
    }
    //string s = mess.ReadMessage<StringMessage>().value;
    playerID = s;
    BinaryFormatter bf = new BinaryFormatter();
    FileStream src = new FileStream("CharData.dat", FileMode.Create);
    FileStream ptxt = new FileStream("PlainTextMyKey.txt", FileMode.Create);
    StreamWriter writer = new StreamWriter(ptxt);
    writer.WriteLine(s);
    writer.Close();
    ptxt.Close();
    bf.Serialize(src, s);
    src.Close();
  }
}
