using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Networking;
using UnityEngine.Networking.NetworkSystem;

public class ChatManager : NetworkBehaviour
{
  public string chatContent;
  [SerializeField] Text txtField;
  void UpdateChat(NetworkMessage msg)
  {
    transform.Find("Canvas/Panel/Text").GetComponent<Text>().text += msg.ReadMessage<StringMessage>().value;
  }

  private void Start()
  {
    if (isServer)
    {
      NetworkServer.RegisterHandler(42, ServerHandleMessage);
    }

    if (isLocalPlayer)
    {
      NetworkManager.singleton.client.RegisterHandler(42, UpdateChat);
    }

    if (!isLocalPlayer)
    {
      gameObject.SetActive(false);
    }
  }

  void Update()
  {
    if (!isLocalPlayer)
    {
      return;
    }

    if (Input.GetKeyDown(KeyCode.G))
    {
      ClientSendMessage(txtField);
    }
  }

  public void ClientSendMessage(Text textField)
  {
    if (!isLocalPlayer)
    {
      return;
    }
    string msg = textField.text;
    print("Sending Message: " + msg);
    StringMessage message = new StringMessage(msg);
    NetworkManager.singleton.client.Send(42, message);
  }

  public void ServerHandleMessage(NetworkMessage message)
  {
    string receivedMessage = message.ReadMessage<StringMessage>().value;
    print("Server Received Message: " + receivedMessage);
    StringMessage msg = new StringMessage("Client " + message.conn.connectionId + ": " + receivedMessage + "\n");
    NetworkServer.SendToAll(42, msg);
  }
}
