using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Billboard : MonoBehaviour
{
  void FixedUpdate()
  {
    if (Camera.main.transform == null){
      return;
    }
    transform.LookAt(Camera.main.transform);
  }
}
