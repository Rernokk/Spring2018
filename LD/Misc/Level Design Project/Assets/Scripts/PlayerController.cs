using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
    if (Input.GetKey(KeyCode.W))
    {
      transform.parent.position += transform.forward * Time.deltaTime * 30f;
    }
    if (Input.GetKey(KeyCode.S))
    {
      transform.parent.position -= transform.forward * Time.deltaTime * 30f;
    }
    transform.Rotate(-Input.GetAxis("Mouse Y") * Time.deltaTime * 180, 0, 0);
    transform.parent.Rotate(0, Input.GetAxis("Mouse X") * Time.deltaTime * 180, 0);
  }
}
