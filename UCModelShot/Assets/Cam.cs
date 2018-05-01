using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cam : MonoBehaviour
{

  // Use this for initialization
  void Start()
  {
    Cursor.lockState = CursorLockMode.Locked;
    Cursor.visible = false;
  }

  // Update is called once per frame
  void Update()
  {
    if (Cursor.visible == false)
    {
      transform.parent.Rotate(new Vector3(0, Input.GetAxis("Mouse X") * Time.deltaTime * 180, 0));
      transform.Rotate(new Vector3(-Input.GetAxis("Mouse Y") * Time.deltaTime * 180, 0, 0));
    }

    if (Input.GetKey(KeyCode.W))
    {
      transform.parent.position += transform.forward * Time.deltaTime * 12f;
      if (Input.GetKey(KeyCode.LeftShift))
        transform.parent.position += transform.forward * Time.deltaTime * 12f;
    }
    if (Input.GetKey(KeyCode.S))
    {
      transform.parent.position -= transform.forward * Time.deltaTime * 12f;
      if (Input.GetKey(KeyCode.LeftShift))
        transform.parent.position -= transform.forward * Time.deltaTime * 12f;
    }

    if (Input.GetKeyDown(KeyCode.Escape))
    {
      Cursor.lockState = CursorLockMode.None;
      Cursor.visible = true;
    }
  }
}
