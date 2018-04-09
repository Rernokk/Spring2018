using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class WorldChatScript : NetworkBehaviour
{
  [SyncVar(hook = "ClientUpdate")] public string messages;

  void ClientUpdate(string scr)
  {
    if (isServer)
    {
      ChatManager[] managers = GameObject.FindObjectsOfType<ChatManager>();
      foreach (ChatManager m in managers)
      {
        m.chatContent = scr;
      }
    }
  }

  public override void OnStartServer()
  {
    NetworkServer.RegisterHandler(42, HandleIncomingMessage);
  }

  void HandleIncomingMessage(NetworkMessage msg)
  {
    print("Server got message.");
  }

}
