using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Networking;
using UnityEngine.Networking.NetworkSystem;
using System;

public class ChatManager : MonoBehaviour
{
  public string chatContent;
  [SerializeField] Text txtField;
  [SerializeField] int ChatLimit = 10;
  void UpdateChat(NetworkMessage msg)
  {
    string val = msg.ReadMessage<StringMessage>().value;
    transform.Find("Canvas/Panel/Text").GetComponent<Text>().text += val;
    string currentLog = transform.Find("Canvas/Panel/Text").GetComponent<Text>().text;
    string[] splitLog = currentLog.Split('\n');
    if (splitLog.Length > ChatLimit)
    {
      currentLog = "";
      for (int i = 1; i < splitLog.Length - 1; i++)
      {
        currentLog += splitLog[i] + "\n";
      }
    }
    transform.Find("Canvas/Panel/Text").GetComponent<Text>().text = currentLog;
  }

  private void Start()
  {
    NetworkServer.RegisterHandler(42, ServerHandleMessage);
    NetworkManager.singleton.client.RegisterHandler(42, UpdateChat);
    StartCoroutine(SendRandomMessage(1f));
  }

  void Update()
  {
    /*if (Input.GetKeyDown(KeyCode.G))
    {
      ClientSendMessage(txtField);
    }*/
  }

  public void ClientSendMessage(Text textField)
  {
    string msg = textField.text;
    //print("Sending Message: " + msg);
    StringMessage message = new StringMessage(msg);
    NetworkManager.singleton.client.Send(42, message);
  }

  public void ClientSendMessage(string msg)
  {
    //print("Sending Message: " + msg);
    StringMessage message = new StringMessage(msg);
    NetworkManager.singleton.client.Send(42, message);
  }

  public void ServerHandleMessage(NetworkMessage message)
  {
    string receivedMessage = message.ReadMessage<StringMessage>().value;
    //print("Server Received Message: " + receivedMessage);
    StringMessage msg = new StringMessage("Client " + message.conn.connectionId + ": " + receivedMessage + "\n");
    NetworkServer.SendToAll(42, msg);
  }

  IEnumerator SendRandomMessage(float delay)
  {
    yield return new WaitForSeconds(delay);
    string temp = "";
    for (int i = 0; i < UnityEngine.Random.Range(5, 15); i++)
    {
      temp += (char)UnityEngine.Random.Range(65, 90);
    }
    ClientSendMessage(temp);
    StartCoroutine(SendRandomMessage(UnityEngine.Random.Range(1f, 2f)));
  }
}
