using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

[Serializable]
public class ClientData : MessageBase{
  public string ClientName = "";
}

public class Client_Manager : NetworkBehaviour {
  NetworkClient client;
  private void Start()
  {
    if (isServer || isLocalPlayer){
      transform.GetChild(0).GetComponent<MeshRenderer>().material.color = Color.blue;
    }

    if (isServer || !isLocalPlayer){
      transform.Find("Camera").gameObject.SetActive(false);
      //transform.GetChild(1).GetComponent<AudioListener>().enabled = false;
    }

    if (!isServer){
      ClientData data = new ClientData();
      data.ClientName = "Jim";
      client.Send(MsgType.Connect, data);
    }
  }

  private void Update()
  {
    if (!isLocalPlayer)
      return;

    if (Input.GetKey(KeyCode.A))
    {
      transform.position -= transform.right * Time.deltaTime * 3f;
    }
    if (Input.GetKey(KeyCode.D))
    {
      transform.position += transform.right * Time.deltaTime * 3f;
    }

    if (Input.GetKeyDown(KeyCode.G)){
      ClientData msg = new ClientData();
      client.Send(45, msg);
    }
  }

  private void OnDisconnectedFromServer(NetworkDisconnection info)
  {
    Destroy(gameObject);
  }
}
